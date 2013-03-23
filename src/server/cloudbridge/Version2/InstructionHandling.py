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
    mac = globalCloud.mac
    conn = globalCloud.MgmtHash[mac].conn
    print conn
    porti = random.randint(1, 1000)+20000
    data=str(minstr.instr)+","+str(porti)
    print "Sending:",data
    # in future use a library to handle this thing
    if instr.devid=="02":
        ind = 0
    else:
        ind = 0
    
    conn[ind].sendall(data)
    return porti

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
