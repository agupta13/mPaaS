import socket
import threading
from time import sleep

## Instruction Queue Scheduler thread
class device_socket(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name
        self.quit = False
        self.dtype=0
        
    def stop(self):
        print "Stop function called"
        self.quit = True
    def setdevtype(self,dtype):        
        self.dtype = dtype
        
    def run(self):
        print "Dev Socket started"
        HOST, PORT = "localhost", int(self.name)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        data = "Hi Dude"
        try:
            sock.connect((HOST, PORT))
            sock.sendall(data)
            print "Sent:     {}".format(data) 
            while self.quit!= True:
                received = sock.recv(256)               
                if len(received)>0:
                    print "Instruction Received:",received
                    instr = received.split(',')
                    portd=instr[3]
                    sleep(1)
                    HOST, PORT = "localhost", int(portd)
                    sockd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    
                    if self.dtype==0:
                        print "for devtype:",self.dtype
                        datad = "LED_OK\n"
                        try:
                            sockd.connect((HOST, PORT))
                            sockd.sendall(datad)
                        finally:
                            sockd.close()
                            print "data socket closed"
                            #break   
                    elif self.dtype==1:
                        print "for devtype:",self.dtype
                        datad = "1000,1,1000,0,1000,0\n"
                        try:
                            sockd.connect((HOST, PORT))
                            while True:
                                try:
                                    sockd.sendall(datad)
                                except IOError as e:
                                    print "WARN: socket closed unexpectedly"
                                    break
                            
                        finally:
                            sockd.close()
                            print "data socket closed"
                            #break  
                                  

                if received == "close": break
                
                if self.quit == True: break

            sock.close()
            print "Closing dev socket" 
        finally:
            print "temp bye"
            
        print "Dev Socket Thread exiting"

if __name__ == "__main__":
    HOST, PORT = "localhost", 9006
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print "send data in mac,platform,devid,..."
    data = "00:00,android,02"
    try:
        sock.connect((HOST, PORT))
        sock.sendall(data)
        print "Sent:     {}".format(data)  
              
        # Receive data from the server
        received = sock.recv(1024)
        print "Received:", received
        sock.close() # Close management thread
        
        ports=received.split(',')
        print ports
        t =[]
        devtype=[0,1]
        for i in range(0,len(ports)):
            schedThread = device_socket(ports[i])
            schedThread.daemon = True
            schedThread.setdevtype(devtype[i])
            schedThread.start()
            t.append(schedThread)
            print "Thread:", i,"started"
            
            
            
        try:
            while True: sleep(10)
        except (KeyboardInterrupt,SystemExit):            
                print "kbrd interrupt"
                for i in range(0,len(ports)):
                    if t[i].isAlive== True: 
                        t[i].stop()
                        print "Thread:", i,"stopped"                
           
            
    finally:
        print "bye"
           
