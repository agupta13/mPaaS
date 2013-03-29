# Author Arpit Gupta: glex.qsd@gmail.com

import threading
from itertools import count
import globalCloud
import socket
import sys
import random

bridgeIP = "152.14.93.160"


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
                if ret is -1:
                    instr.success = 0
                else:
                    instr.success = 1
                instr.port = ret
                instr.event.set()
                print "Instruction for thread: "+str(instr.tid)
        
        print "Scheduler Thread exiting"
        
def exec_instr(instr):
    print "Instr exec function called"
    minstr = map_instr(instr) # returns mapped instruction
    
    # Assumes single mobile client for now...we need to update this process
    
    mac = globalCloud.mac
    if(mac not in globalCloud.MgmtHash): 
        print "Error: No MC in hash table"
        return -1
    conn = globalCloud.MgmtHash[mac].conn    
    portd = random.randint(1, 1000)+20000
    data=str(minstr.instr)+",800,200,"+str(portd)
    data = data+"\n" # to comply with Raghav's Android code to parse instruction data
    print "Sending:",data
    
    # Start the listening server before sending the port adresses to avoid concurrency issues
    HOST, PORT = "152.14.93.160",portd
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind((HOST, PORT))
    soc.listen(1)
    
    
    
    
    #conn, addr = soc.accept()
    
    
    # in future use a mapping function to handle this thing
    if instr.devid=="02":
        ind = 0
    else:
        ind = 1 # Assuming only two devices and in order    
    if(len(conn)==1): ind=0 # Safety statement in case u just have accelerometer running
    instr.instr_send = data
    instr.soc_instr = conn[ind]
    instr.soc_data = soc
    
    return portd
    


def map_instr(instr):
    print "Mapping function called"
    minstr = instr  # need more sophisticated processing    
    return minstr
    
def queueOps(instr, p):
    print "Queue Operation Initiated"
    instr.event.clear()
    globalCloud.IQ.put(instr,p)
    print "Queue Operation completed, qsize:"+str(globalCloud.IQ.qsize())
    instr.event.wait()
    print "After Wait Queue Operation completed, qsize:"+str(globalCloud.IQ.qsize())
    ret = instr.port
    print "test:port",instr.port,"instr_send:",instr.instr_send
    return ret,instr
