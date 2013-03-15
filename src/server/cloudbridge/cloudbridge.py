# Author: Arpit Gupta: glex.qsd@gmail.com


import socket
import threading
import SocketServer
from itertools import count
import InstructionHandling
import globalCloud
import policyCloud


        

        
## Thread to handle incoming requests for device driver instructions from Cloud Platform
class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):

    def handle(self):
        data = self.request.recv(1024)
        cur_thread = threading.current_thread()        
        self.tid = next(iid)
        e = threading.Event()
        instr = globalCloud.Instruction(0,0,0,0,self.tid, e, 0)
        
        # Logic for policy check
        priority = policyCloud.policyCheck()
        
        # Logic for instruction Queue insertion
        success = InstructionHandling.queueOps(instr, priority)
        
        if success == 0:
            response = "S"
        else:
            response = "F"
        
        self.request.sendall(response)

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

    

if __name__ == "__main__":
    threadlist = []
    #IQ = PriorityQueue()
    iid = count()
    
    # Initialize the server for listening to mobile platforms requests

    HOST, PORT = "localhost", 9998
    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address
    server_thread = threading.Thread(target=server.serve_forever)
    threadlist.append(server_thread)
    server_thread.daemon = True
    server_thread.start()
    print "Server running for thread:", server_thread.name
   
    
    # Now server has started, start the scheduler thread
    
    schedThread = InstructionHandling.IQscheduler("sched")
    schedThread.daemon = True
    schedThread.start()
    

    # Keep server on until you get a keyboard interrupt "ctrl+c"    
    try:
        server.serve_forever()
    except KeyboardInterrupt: 
        pass
    finally:
        server.shutdown()
        print "bye"
    
