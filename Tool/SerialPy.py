import serial
import numpy as np
import matplotlib.pyplot as plt
import time, datetime

def log_to_file(msg):
    global filename
    logFile = open(filename, 'a')
    logFile.write(msg)
    logFile.close()

def format_1():
    global cnt
    value =  int.from_bytes( ser.read(4),byteorder='big')
    value_max = int.from_bytes(ser.read(2),byteorder='big')
    idx_max = int.from_bytes(ser.read(1),byteorder='big')
    count_valid = int.from_bytes(ser.read(1),byteorder='big')
    count_adc   = int.from_bytes(ser.read(1),byteorder='big')
    logData = str(value) + " " + str(value_max) + " " + str(idx_max) + " " + str(count_valid) + " " + str(count_adc)
    print(logData)
    log_to_file(logData + "\n")
    plt.plot(cnt,value, label='IR',color='#FF6600', marker='o')

def format_2():
    global cnt
    value_max =  int.from_bytes( ser.read(2),byteorder='big')
    idx_max = int.from_bytes(ser.read(1),byteorder='big')
    distance = int.from_bytes(ser.read(1),byteorder='big')
    logData = str(value_max) + " " + str(idx_max) + " " + str(distance)
    print(logData)
    log_to_file(logData + "\n")
    plt.plot(cnt,value_max, label='IR',color='#FF6600', marker='o')

input("Press Enter to start.")

ser = serial.Serial('COM19', # Device name varies
baudrate = 9600,
bytesize = 8,
parity = 'N',
stopbits = 1)

global cnt
cnt = 20

global filename
name = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
filename = "%s.txt" % name

logData = "value_max " +  "idx_max " + "distance"
print(logData)
log_to_file(logData + "\n")

while (cnt < 140):
    # format_1()
    format_2()

    cnt +=1
    if ((cnt%10==0) & (cnt < 140)):
        ser.close()
        input(str(cnt/10) + " is done. Press Enter to continue...")
        ser = serial.Serial('COM19', # Device name varies
        baudrate = 9600,
        bytesize = 8,
        parity = 'N',
        stopbits = 1)

ser.close()
plt.show()