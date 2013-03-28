# Author: Arpit Gupta: glex.qsd@gmail.com


import socket
import threading
import SocketServer
from itertools import count
import InstructionHandling
import globalCloud
import policyCloud
import MgmtClass
import random

def mobileClientCheck(mac):
#Check the global hash table to check if we have already seen the mobileclient
	print "In mobile client check function\n"
	if(mac in globalCloud.MgmtHash):
		return 0	
	else:
		return 1

## Thread to handle incoming requests for device driver instructions from Mobile Client
class ThreadedTCPRequestHandler_mc(SocketServer.BaseRequestHandler):

    def handle(self):
        print "Handler: Mobile Client"
        data = self.request.recv(1024)
        print "Received:",data
        dlist = data.split(',')
        Ndev = len(dlist)-2
        print "Number of devices supported by MC:",Ndev
        
        mac = dlist[0]
        print "mac:",mac
        platform = dlist[1]
        print "platform:",platform
        dev=[]
        for i in range(0,Ndev):
            dev.append(dlist[i+2])
            print "dev:",dev
        globalCloud.mac = mac
        # Now time to do the hash table stuff
        #if(mobileClientCheck(mac)):
        #    print "Mac not in table"
        if(mobileClientCheck(mac)):
            My_Mgmt_Obj = MgmtClass.MgmtObj(mac,dev, platform,0,[])
            globalCloud.MgmtHash[mac] = My_Mgmt_Obj
        
        ## Create ports numbers and send that back to the client
        response = ""
        ports = []
        for i in range(0,len(dev)):
            portn = random.randint(1, 1000)+10000
            ports.append(portn)
            print portn
            if(i==len(dev)-1):
                response = response+str(portn)
            else:
                response = response+str(portn)+","
            	    
		print response  
		
		  
        # Time to create socket on these ports
        socks = []
        for i in range(0,len(ports)):
            HOST, PORT = "localhost",ports[i]
            soc_dev = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            soc_dev.bind((HOST, PORT))
            soc_dev.listen(1)
            socks.append(soc_dev)
		    
	    #globalCloud.MgmtHash[mac].conn = socks		   
	     
            
        self.request.sendall(response)     
        for i in range(0,len(ports)):
            conn, addr = socks[i].accept()
            globalCloud.MgmtHash[mac].conn.append(conn)
            print 'Received Connection from : ', addr
            data = conn.recv(1024)
            print "Received:",data
            
            # For now
            socks[i].close()
            
               

        
## Thread to handle incoming requests for device driver instructions from Cloud Platform
class ThreadedTCPRequestHandler_cp(SocketServer.BaseRequestHandler):

    def handle(self):
        print "Handler: Cloud Platform"
        data = self.request.recv(1024)
        instr_rec = data.split(',')
        print instr_rec
        devid = instr_rec[0]
        if devid=="02":
            devtype = 0
        else:
            devtype = 1
        instr_code = instr_rec[1]
        Nparams = len(instr_rec)-2        
        params=[]
        for i in range(0,Nparams):
            params.append(instr_rec[i+2])
              
        self.tid = next(iid)
        e = threading.Event()
        instr = globalCloud.Instruction(0,devid,instr_code,0,self.tid, e, 0,params,devtype)
        
        ## Logic for policy check
        priority = policyCloud.policyCheck()
        
        ## Logic for instruction Queue insertion
        portd, rinstr = InstructionHandling.queueOps(instr, priority)
        if portd!=-1:
            print "Started server over the port:",portd
            print "test:",rinstr.instr_send
            rinstr.soc_instr.sendall(rinstr.instr_send)
            conn, addr = rinstr.soc_data.accept()
            if(devtype==0):            
                print 'Received Connection from mobile client: ', addr
                data = conn.recv(1024)
                print "Received from mobile client:",data
                self.request.sendall(data)
                print "Received data sent to CP:",data
                rinstr.soc_data.close()            
            else:
                print 'Received Connection from : ', addr
                while(1):
                    data = conn.recv(21)
                    print "Received data from MC"
                    response = "1000,1,1000,0,1000,0\n" 
                    try:
                        self.request.sendall(response)
                        print "Sent to CP"
                        #self.wfile.write(response)
                    except IOError as e:
                        print "WARN: socket closed unexpectedly"
                        #rinstr.soc_data.sendall("close\n")
                        rinstr.soc_data.close()
                        break
        else:
            print "Error: Instruction Execution"
            data = "-1"
            self.request.sendall(data)
            
        
        

        
        

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

    

if __name__ == "__main__":
    threadlist = []
    iid = count()
    
    # Initialize the server for listening to "Mobile Client" requests

    HOST, PORT = "localhost", 9006
    server_mc = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler_mc)
    ip, port = server_mc.server_address
    server_thread_mc = threading.Thread(target=server_mc.serve_forever)
    server_thread_mc.daemon = True
    server_thread_mc.start()
    print "Server running over IP:", ip," port:",port
    
    # Initialize the server for listening to "cloud platforms" requests

    HOST, PORT = "localhost", 9998
    server_cp = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler_cp)
    ip, port = server_cp.server_address
    server_thread = threading.Thread(target=server_cp.serve_forever)
    server_thread.daemon = True
    server_thread.start()
    print "Server running over IP:", ip," port:",port
    
    # Now server has started, start the scheduler thread
    
    schedThread = InstructionHandling.IQscheduler("sched")
    schedThread.daemon = True
    schedThread.start()
    
    # Keep server on until you get a keyboard interrupt "ctrl+c"    
    try:
        server_cp.serve_forever()
        server_mc.serve_forever()
    except KeyboardInterrupt: 
        pass
    finally:
        server_cp.shutdown()
        print "Shutting down cp server"
        server_mc.shutdown()
        print "Shutting down mc server"
        print "bye"

    
    
  
    

    
    
