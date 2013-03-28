import socket
import sys
from time import sleep

HOST, PORT = "localhost", 9006
data = " ".join(sys.argv[1:])
debug = 1

def process_result(result):
	#Get port number for connecting to temporal port
	instruction,port_temp = result.split(',')
	print instruction
	print port_temp

	print "Mobile is performing the instruction...."
	sleep(5)
	#Create socket here. Send data to that temporal socket
	socket_temp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	socket_temp.connect((HOST, int(port_temp)))
	#Hardcode result status here
	data = "Success"
	socket_temp.sendall(data)
	return 0
# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
data = "00:00,23,01,"
print "send data in mac,devid,platform,port"

try:
    # Connect to server and send data
    sock.connect((HOST, PORT))
    sock.sendall(data + "\n")
    print "Sent:     {}".format(data)
    
    # Receive data from the server and shut down
    received = sock.recv(1024)
    print "Received: {}".format(received)
    received = format(received)
    
    #Data received from CloudBidge is in format Sucess/Failure, port number, instruction type
    success,instr_port,instr = received.split(',') 
    print success
    print instr_port
    print instr

    #Close management socket
    sock.close()

    #create a socket for this service and connect to it.
    print "Opening service connection with Cloud Bridge"
    sock_instr = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock_instr.connect((HOST, int(instr_port)))
    #data = "Is this instruction server for"+instr
    #sock_instr.sendall(data + "\n")
    #print "Sent:     {}".format(data)
    received = sock_instr.recv(1024)
    print "Received: {}".format(received)
    process_result(received)
    #sock_instr.sendall(data + "\n")
    sock_instr.close()
finally:
    print "bye"
