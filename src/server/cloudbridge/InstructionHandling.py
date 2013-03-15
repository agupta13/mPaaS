# Author Arpit Gupta: glex.qsd@gmail.com

import threading
from itertools import count
import globalCloud
import socket
import sys


## Instruction Queue Scheduler thread
class IQscheduler(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name
    
    def run(self):
        print "Scheduler Thread started"
        while(1):
            if(globalCloud.IQ.qsize>0):
                print "Qsize greater than zero, qsize: "+str(globalCloud.IQ.qsize())
                instr = globalCloud.IQ.get()
                ret = exec_instr(instr)
                if ret is 0:
                    instr.success = 1
                else:
                    instr.success = 0
                instr.event.set()
                print "Instruction for thread: "+str(instr.tid)
        
        print "Scheduler Thread exiting"
        
def exec_instr(instr):
    print "Instr exec function called"
    minstr = map_instr(instr) # returns mapped instruction
    
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    HOST, PORT = "localhost", 9198
    data = ""+str(minstr.instr)
    print "Sending:     {}".format(data)
    try:
        # Connect to server and send data
        sock.connect((HOST, PORT))
        sock.sendall(data + "\n")
        
        # Receive data from the server and shut down
        received = sock.recv(1024)
    finally:
        sock.close()
    print "Received: {}".format(received)
    return 0

def map_instr(instr):
    print "Mapping function called"
    minstr = instr  # need more sophisticated processing
    
    return minstr
    
def queueOps(instr, p):
    print "Queue Operation completed"
    instr.event.clear()
    globalCloud.IQ.put(instr,p)
    print "Queue Operation completed, qsize:"+str(globalCloud.IQ.qsize())
    instr.event.wait()
    if instr.success is 1:
        ret = 0
    else:
        ret = -1
    print "After Wait Queue Operation completed, qsize:"+str(globalCloud.IQ.qsize())
    return ret
