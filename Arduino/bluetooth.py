import serial
import time

print("Start")
bluetooth = serial.Serial("COM17", 9600)
print("Connected")

i = 1000
while True:
  print(i)
  bluetooth.write(str.encode(f"{i}/{i}/{i}/{i}/{i}/{i}/"))
  i += 5    
  if i >= 2000:
        i = 1000
  time.sleep(0.04)
