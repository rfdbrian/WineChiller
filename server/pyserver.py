import BaseHTTPServer
import os
from SimpleHTTPServer import SimpleHTTPRequestHandler
#print("AA")
#import usb

Handler = SimpleHTTPRequestHandler
Server = BaseHTTPServer.HTTPServer

server_address = ('127.0.0.1',8000)

Handler.protocol_version = "HTTP/1.0"
httpd = Server(server_address,Handler)
print("YO!!!")
f1 = open('f1.php', 'r')
for line in f1:
	print(line)
httpd.serve_forever()

