# Author: Arpit Gupta: glex.qsd@gmail.com


import socket
import threading
import SocketServer
from time import sleep
import globalVars


## Test thread
class Test(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name
    
    def run(self):
        print "Test Thread started"
        sleep(5)  
        #globalVars.socket_g.close()      
        response = "close"        
        globalVars.socket_g.sendall(response)
        print "test",globalVars.test
        globalVars.test = 1
        print "Test Thread exiting"

if __name__ == "__main__":

    
    # Initialize the server for listening to mobile platforms requests

    HOST, PORT = "localhost", 9998
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))
    s.listen(1)
    print "Server running at port:", PORT
    conn, addr = s.accept()
    print 'Connected by', addr
    data = conn.recv(1024)
    print "received:",data
    response = "S"
    conn.sendall(response)
    globalVars.socket_g = conn
    globalVars.test = 0
    schedThread = Test("test")
    schedThread.daemon = True
    schedThread.start()
    
    while True:
        if globalVars.test ==1: 
            print "test is now set"
            break
            
            
    conn.close()
    s.close()
    


