import socket
import sys

HOST, PORT = "localhost", 9998
data = "Hi"

# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Connect to server and send data
    sock.connect((HOST, PORT))
    print "Sent:     {}".format(data)
    sock.sendall(data + "\n")
    
    # Receive data from the server and shut down
    while True:
        received = sock.recv(256)
        
        if received == "close":
            break
    
finally:
    sock.close()

print "Received: {}".format(received)

