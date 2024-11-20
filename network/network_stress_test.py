import socket
import time
import sys
import signal

if len(sys.argv) != 4:
    print("Usage: {} <host> <port> <interval_ms>".format(sys.argv[0]))
    sys.exit(1)

HOST = sys.argv[1]
PORT = int(sys.argv[2])
INTERVAL_MS = float(sys.argv[3])

SENT = 0
RECEIVED = 0

def signal_handler(sig, frame):
    print("\nSent: {}, Received: {}, Lost: {}".format(SENT, RECEIVED, SENT - RECEIVED))
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print("Starting stress test: sending ping every {} ms to {}:{}".format(INTERVAL_MS, HOST, PORT))

while True:
    start_time = time.time()
    try:
        sock.sendto(b'ping', (HOST, PORT))
        SENT += 1
        sock.settimeout(0.001)  # 1 ms timeout for receiving pong
        data, _ = sock.recvfrom(1024)
        if data == b'pong':
            RECEIVED += 1
    except socket.timeout:
        pass
    elapsed_time = (time.time() - start_time) * 1000
    sleep_time = INTERVAL_MS - elapsed_time
    if sleep_time > 0:
        time.sleep(sleep_time / 1000.0)