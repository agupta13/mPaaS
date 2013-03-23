# Author Arpit Gupta: glex.qsd@gmail.com

from Queue import PriorityQueue

test = 0
#Hash for storing the Mac and Mgmt Object
global MgmtHash
MgmtHash = {}

## Instruction Class
class Instruction():
    def __init__(self, cltid, devid, instr, arg, tid, event, success):
        self.cltid = cltid
        self.devid = devid
        self.instr = instr
        self.arg = arg
        self.tid = tid
        self.event = event
        self.success = success
        print "Initialized the Instruction Object"

# INstruction queue
IQ = PriorityQueue()
