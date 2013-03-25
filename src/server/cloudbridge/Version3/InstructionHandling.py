# Author Arpit Gupta: glex.qsd@gmail.com

import threading
from itertools import count
import globalCloud
import socket
import sys
import random


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
    porti = random.randint(1, 1000)+20000
    data=str(minstr.instr)+",800,200,"+str(porti)
    data = data+"\n" # to comply with Raghav's Android code to parse instruction data
    print "Sending:",data
    # in future use a mapping function to handle this thing
    if instr.devid=="02":
        ind = 0
    else:
        ind = 1 # Assuming only two devices and in order    
    if(len(conn)==1): ind=0 # Safety statement in case u just have accelerometer running
    try:
        conn[ind].sendall(data)
    except conn[ind].error as msg:
        print "Socket error:",msg
        return -1
        
        
         
    return porti # Returns the port number where data is expected from MC

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
    return ret
