##  System Flow Diagram

Below is the complete data flow between the **C Server** and **Python Client**:

             ┌────────────────────────────┐
             │        C SERVER            │
             │    (random generator)      │
             └─────────────┬──────────────┘
                           │
                           │ 1. Generate Random Number
                           ▼
                 ┌──────────────────┐
                 │   RANDOM VALUE   │
                 │     e.g. 742     │
                 └─────────┬────────┘
                           │
                           │ 2. Compute Integrity
                           │    - Checksum (1-byte sum)
                           │    - CRC32 (IEEE)
                           ▼
  ┌───────────────────────────────────────────────────────┐
  │   PACKET FORMATION                                    │
  │   "value,checksum,crc32\n"                            │
  │   Example: "742,157,289723491\n"                      │
  └──────────────────────────┬─────────────────────────────┘
                             │
                             │ 3. Send Packet Over TCP
                             ▼
                  ╔══════════════════════╗
                  ║    TCP SOCKET LINK   ║
                  ╚══════════════════════╝
                             │
                             │ 4. Receive Packet
                             ▼
             ┌────────────────────────────┐
             │       PYTHON CLIENT        │
             │     (data validation)      │
             └─────────────┬──────────────┘
                           │
                           │ 5. Recompute integrity:
                           │    - checksum(value)
                           │    - crc32(value)
                           ▼
                 ┌──────────────────┐
                 │ VERIFY MATCH ?   │
                 └──────────────────┘
                     │          │
               YES   │          │   NO
                     ▼          ▼
          ┌────────────────┐  ┌─────────────────┐
          │  Show “OK”     │  │ Show “BAD”      │
          │  (valid data)  │  │ (corruption)    │
          └────────────────┘  └─────────────────┘



#  License
Free to use for learning, research, and embedded development.

# 🛠 Build Logic (Makefile Overview)

make
→ Compiles the C program using optimized flags
→ Ensures warnings are visible

make run
→ Automatically spawns two terminals
→ Server runs independently on Port 5000
→ Client connects, listens, validates checksum & CRC values
→ Both sides operate continuously without manual intervention

make clean
→ Ensures reproducible builds
→ Removes stale binaries

make rebuild
→ Recommended after modifying:

CRC logic

Checksum logic

Socket communication

Random generation logic



