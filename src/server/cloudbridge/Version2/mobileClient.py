import socket
import threading


if __name__ == "__main__":
    HOST, PORT = "localhost", 9006
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print "send data in mac,platform,devid,..."
    data = "00:00,android,01,02"
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
                # for now
                sock.close() 
            finally:
                print "temp bye"
            
    finally:
        print "bye"
           