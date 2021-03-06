# Author Arpit Gupta: glex.qsd@gmail.com

from Queue import PriorityQueue

test = 0
mac = 0
#Hash for storing the Mac and Mgmt Object
global MgmtHash
MgmtHash = {}

## Instruction Class
class Instruction():
    def __init__(self, cltid, devid, instr, arg, tid, event, success,params,devtype):
        self.cltid = cltid
        self.devid = devid
        self.instr = instr
        self.arg = arg
        self.tid = tid
        self.event = event
        self.success = success
        self.params = params
        self.devtype = devtype
        self.port = 0
        self.soc_instr = 0
        self.soc_data=0
        self.instr_send = ""
        print "Initialized the Instruction Object"

# INstruction queue
IQ = PriorityQueue()
