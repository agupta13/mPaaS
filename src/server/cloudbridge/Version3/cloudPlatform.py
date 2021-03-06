import socket
import sys
devtype=1

if devtype==0:
    HOST, PORT = "localhost", 9998
    # devid,instr,params...
    data = "02,led_on,10,10"

    # Create a socket (SOCK_STREAM means a TCP socket)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to server and send data
        sock.connect((HOST, PORT))
        sock.sendall(data)
        
        # Receive data from the server and shut down
        received = sock.recv(1024)
    finally:
        sock.close()

    print "Sent:     {}".format(data)
    print "Received: {}".format(received)
    
if devtype==1:
    HOST, PORT = "localhost", 9998
    # devid,instr,params...
    data = "03,ON,10,10"

    # Create a socket (SOCK_STREAM means a TCP socket)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to server and send data
        sock.connect((HOST, PORT))
        sock.sendall(data)
        ctr=0
        # Receive data from the server and shut down
        try:
            while True:
                received = sock.recv(21)
                print "received:",received
                ctr+=1
                if(ctr==10):   
                    print received
                    ctr=0
                    break
        except (KeyboardInterrupt,SystemExit):
            print "Kbrd Interrupt"
    finally:
        sock.close()

    print "Sent:     {}".format(data)
    print "Received: {}".format(received)
