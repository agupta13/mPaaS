import socket
import threading
from time import sleep


if __name__ == "__main__":
    HOST, PORT = "localhost", 9006
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print "send data in mac,platform,devid,..."
    data = "00:00,android,01"
    try:
        sock.connect((HOST, PORT))
        sock.sendall(data)
        print "Sent:     {}".format(data)  
              
        # Receive data from the server
        received = sock.recv(1024)
        print "Received:", received
        sock.close()
        
        ports=received.split(',')
        print ports
        for i in range(0,len(ports)):
            HOST, PORT = "localhost", int(ports[i])
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            data = "Hi Dude"
            try:
                sock.connect((HOST, PORT))
                sock.sendall(data)
                print "Sent:     {}".format(data) 
                while True:
                    received = sock.recv(256)
                    if len(received)>0:
                        print received
                        instr = received.split(',')
                        portd=instr[3]
                        sleep(2)
                        HOST, PORT = "localhost", int(portd)
                        sockd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                        datad = "All's Well"
                        try:
                            sockd.connect((HOST, PORT))
                            sockd.sendall(datad)
                        finally:
                            sockd.close()
                        

                    if received == "close":
                        break
                # for now
                sock.close() 
            finally:
                print "temp bye"
            
    finally:
        print "bye"
           
