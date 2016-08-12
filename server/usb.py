
#!/usr/bin/env python
import serial
import numpy as np 
from pprint import pprint
from tkinter import *

	
ser = serial.Serial('/dev/cu.usbmodem1421', 9600)
a=0
b=0
c=0
d=0 
e=0 
f=0
first=""
second= ""
third=""
fourth=""
fifth=""
sixth= ""
list = [first,second,third,fourth,fifth,sixth]

while 1:
	x =0
	pena = ser.readline()
	if pena!="\r\n":
		if pena == "001\r\n" :
			a=1
		elif pena == "000\r\n" :
			a=0
		elif pena == "011\r\n" :
			#second  = raw_input("WINE?")
			b=1
		elif pena == "010\r\n" :
			b=0
		elif pena == "021\r\n" :
			#third  = raw_input("WINE?")
			c=1
		elif pena == "020\r\n" :
			c=0
		
		elif pena == "101\r\n" :
			#fourth  = raw_input("WINE?")

			d=1
		elif pena == "100\r\n" :
			d=0
		elif pena == "111\r\n" :
			#fifth  = raw_input("WINE?")

			e=1
		elif pena == "110\r\n" :
			e=0

		elif pena == "121\r\n" :
			#sixth  = raw_input("WINE?")

			f=1
		elif pena == "120\r\n" :
			f=0
		alist = [str(a),str(b),str(c),str(d),str(e),str(f)]
		grid=np.array([[a, b, c ], [d, e, f ]])
		print(grid)
	f1 =open('f1.txt','w') 
	for x in alist:	
		f1.write(x)	
		#np.savetxt('f1.txt',grid)
	f1.close()					#write to file named f1.txt
		
ser.close()
	# Only executes once the loop exits
