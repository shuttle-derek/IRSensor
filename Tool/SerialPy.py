import serial
import numpy as np
import matplotlib.pyplot as plt
import time, datetime

def log_to_file(msg):
    global filename
    logFile = open(filename, 'a')
    logFile.write(msg)
    logFile.close()


input("Press Enter to start.")

ser = serial.Serial('COM19', # Device name varies
baudrate = 9600,
bytesize = 8,
parity = 'N',
stopbits = 1)

cnt = 20;

global filename
name = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
filename = "%s.txt" % name

logData = "value " + "value_max " +  "idx_max " + "count_valid " + "count_adc"
print(logData)
log_to_file(logData + "\n")

while (cnt < 140):
    value =  int.from_bytes( ser.read(4),byteorder='big')
    value_max = int.from_bytes(ser.read(2),byteorder='big')
    idx_max = int.from_bytes(ser.read(1),byteorder='big')
    count_valid = int.from_bytes(ser.read(1),byteorder='big')
    count_adc   = int.from_bytes(ser.read(1),byteorder='big')
    # print(value, ',', count_valid, ',', count_adc)
    logData = str(value) + " " + str(value_max) + " " + str(idx_max) + " " + str(count_valid) + " " + str(count_adc)
    print(logData)
    log_to_file(logData + "\n")
    plt.plot(cnt,value, label='IR',color='#FF6600', marker='o')
    cnt +=1
    if ((cnt%10==0) & (cnt < 140)):
        ser.close()
        input(str(cnt/10) + " cm is done. Press Enter to continue...")
        ser = serial.Serial('COM19', # Device name varies
        baudrate = 9600,
        bytesize = 8,
        parity = 'N',
        stopbits = 1)

ser.close()
plt.show()