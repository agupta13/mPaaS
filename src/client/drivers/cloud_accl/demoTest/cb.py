import socket
import threading
import SocketServer

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):

    def handle(self):
        data = self.request.recv(1024)
        cur_thread = threading.current_thread()
        print "Received: {}".format(data)
        while(1):
            # Decided not to involve any floating numbers for now, need to add support for negative numbers.
            # Sign supported byt the second integer binary 0=> +ve and vice versa. 
            # Support for delay feature
            response = "1000,1,1000,0,1000,0\n" 
            try:
                self.request.sendall(response)
                #self.wfile.write(response)
            except IOError as e:
                print "WARN: socket closed unexpectedly"
                break

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

if __name__ == "__main__":
    # Port 0 means to select an arbitrary unused port
    HOST, PORT = "localhost", 9998
    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.daemon = True
    server_thread.start()
    print "Server loop running in thread:", server_thread.name


    # Keep server on until you get a keyboard interrupt "ctrl+c"    
    try:
        server.serve_forever()
    except KeyboardInterrupt: 
        pass
    finally:
        server.shutdown()
        print "bye"
