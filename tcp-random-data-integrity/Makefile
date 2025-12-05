# -------------------------------------------------------------
#  Fully Automated Makefile
#  No manual commands needed.
# -------------------------------------------------------------

CC       = gcc
CFLAGS   = -Wall -O2
TARGET   = server
SERVER_SRC = server.c
CLIENT   = client.py

# Default target
all: build run_server

# -------------------------------------------------------------
# Build the C server
# -------------------------------------------------------------
build:
	@echo "🔧 Building C server..."
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(TARGET)
	@echo "✔ Build complete."

# -------------------------------------------------------------
# Run only the server
# -------------------------------------------------------------
run_server:
	@echo "🚀 Starting TCP C Server..."
	./$(TARGET)

# -------------------------------------------------------------
# Run only the Python client
# -------------------------------------------------------------
run_client:
	@echo "🐍 Starting Python Client..."
	python3 $(CLIENT)

# -------------------------------------------------------------
# Run both (server + client) in two terminals automatically
# -------------------------------------------------------------
run:
	@echo "🚀 Launching Server and Client..."
	@gnome-terminal -- bash -c "./$(TARGET); exec bash" &
	@gnome-terminal -- bash -c "python3 $(CLIENT); exec bash" &
	@echo "✔ Server and Client launched in separate terminals."

# -------------------------------------------------------------
# Clean the build output
# -------------------------------------------------------------
clean:
	@echo "🧹 Cleaning build files..."
	rm -f $(TARGET)
	@echo "✔ Cleanup done."

# -------------------------------------------------------------
# Full rebuild
# -------------------------------------------------------------
rebuild: clean all

