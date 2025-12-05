import socket
import zlib

HOST = "127.0.0.1"
PORT = 5000

def checksum(data: bytes):
    return sum(data) & 0xFF

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

print("Client connected.")

while True:
    data = s.recv(1024)
    if not data:
        break

    line = data.decode().strip()

    try:
        val_str, sum_str, crc_str = line.split(",")

        # Convert
        value = int(val_str)
        recv_sum = int(sum_str)
        recv_crc = int(crc_str)

        # Compute checksum locally
        calc_sum = checksum(val_str.encode())
        calc_crc = zlib.crc32(val_str.encode()) & 0xFFFFFFFF

        # Verify data integrity
        chks = "OK" if calc_sum == recv_sum else "BAD"
        crcs = "OK" if calc_crc == recv_crc else "BAD"

        print(f"Value={value} | Checksum={chks} | CRC={crcs}")

    except ValueError:
        print("Invalid packet:", line)

