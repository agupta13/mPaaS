#!/usr/bin/python
class MgmtObj():
	def __init__(self, mac,devid, platform_req, port,conn):
		self.mac = mac
		self.devid = devid
		self.platform_req = platform_req
		self.port = port
		self.conn_state = True	#Default switch to true when the device is initialized. Might change later
		self.conn = conn
		print "Mgmt Object Initialized for:",self.mac
		
