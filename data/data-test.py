import serial
import numpy as np
ser = serial.Serial('/dev/ttyACM0', 9600)
data_list = []
try:
    while True:
        data = ser.readline().decode().replace('\r\n','').split('\t')
        data = list(map(float, data))
        data_list.append(data)
        print(data)
except KeyboardInterrupt:
    print("Hello")

data_list = np.array(data_list)
np.savetxt('test-save.txt', data_list)

