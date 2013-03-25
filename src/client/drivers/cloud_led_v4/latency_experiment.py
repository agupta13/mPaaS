# Author: Arpit Gupta: glex.qsd@gmail.com

import os, sys, string, time, datetime

N=10

com_make = "make clean && make"
com_cleanmsg = "sudo dmesg -c"
com_set = "make load && make perm"
com_runON = "make echo1"
com_runOFF = "make echo0"
com_save = "dmesg > msg.txt"
com_cleanup = "make unload && make clean"

print "make"
os.system(com_make)
print "message clean"
os.system(com_cleanmsg)
print "Set up"
os.system(com_set)

print "System ready for experiments"

for i in range(0,N):
    print "N:",i
    os.system(com_runON)
    time.sleep(0.5)
    os.system(com_runOFF)
    time.sleep(0.5)
print "save message"
os.system(com_save)
print "clean up"
os.system(com_cleanup)
os.system(com_cleanmsg)
print "All Done !"
