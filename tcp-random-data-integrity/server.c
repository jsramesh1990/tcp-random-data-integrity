#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 5000

// ---------------- CRC32 TABLE ----------------
unsigned int crc_table[256];

void init_crc32() {
    unsigned int poly = 0xEDB88320;
    for (unsigned int i = 0; i < 256; i++) {
        unsigned int crc = i;
        for (unsigned int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ poly;
            else
                crc >>= 1;
        }
        crc_table[i] = crc;
    }
}

unsigned int crc32(const unsigned char *data, size_t len) {
    unsigned int crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        unsigned char index = (crc ^ data[i]) & 0xFF;
        crc = (crc >> 8) ^ crc_table[index];
    }
    return crc ^ 0xFFFFFFFF;
}

// ---------------- SIMPLE CHECKSUM ----------------
unsigned int checksum(const unsigned char *data, size_t len) {
    unsigned int sum = 0;
    for (size_t i = 0; i < len; i++)
        sum += data[i];
    return sum & 0xFF; // 1-byte checksum
}

// ---------------- MAIN SERVER ----------------
int main() {
    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    init_crc32();
    srand(time(NULL));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket"); return 1; }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind"); return 1;
    }
    if (listen(sockfd, 1) < 0) {
        perror("Listen"); return 1;
    }

    printf("Server running on port %d...\n", PORT);
    client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len);

    if (client_sock < 0) { perror("Accept"); return 1; }
    printf("Client connected.\n");

    while (1) {
        int value = rand() % 1000;

        char value_str[32];
        snprintf(value_str, sizeof(value_str), "%d", value);

        unsigned int sum = checksum((unsigned char*)value_str, strlen(value_str));
        unsigned int crc = crc32((unsigned char*)value_str, strlen(value_str));

        char out[128];
        snprintf(out, sizeof(out), "%d,%u,%u\n", value, sum, crc);

        send(client_sock, out, strlen(out), 0);

        printf("Sent => Value: %d | Checksum: %u | CRC32: %u\n", value, sum, crc);

        usleep(500000);
    }

    close(client_sock);
    close(sockfd);
    return 0;
}

