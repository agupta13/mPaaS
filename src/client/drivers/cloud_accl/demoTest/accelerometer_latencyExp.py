# Author: Arpit Gupta: glex.qsd@gmail.com

import os, sys, string, time, datetime


scenario = 1
screen = 1
sleep_time = 10
data_dir = "~/mPaaS/data_analysis/"

com_make = "make clean && make"
com_cleanmsg = "sudo dmesg -c"
com_set = "make load && make perm"
com_runON = "make echo1"
com_runOFF = "make echo0"
com_save = "dmesg > "+data_dir+"data_"+str(scenario)+"_"+str(screen)+".txt"
com_cleanup = "make unload && make clean"

print "make"
os.system(com_make)
print "message clean"
os.system(com_cleanmsg)
print "Set up"
os.system(com_set)

print "System ready for experiments"

print "Enable reading"
os.system(com_runON)
time.sleep(sleep_time)
print "Disable reading"
os.system(com_runOFF)
time.sleep(2)
print "save message"
os.system(com_save)
print "clean up"
os.system(com_cleanup)
os.system(com_cleanmsg)
print "All Done !"
