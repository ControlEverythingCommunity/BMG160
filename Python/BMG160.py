# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# BMG160
# This code is designed to work with the BMG160_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Gyro?sku=BMG160_I2CS#tabs-0-product_tabset-2

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# BMG160 address, 0x68(104)
# Select range register, 0x0F(15)
#		0x80(128)	Configure full scale range = 2000dps
bus.write_byte_data(0x68, 0x0F, 0x80)
# BMG160 address, 0x68(104)
# Select bandwidth register, 0x10(16)
#		0x04(04)	Set bandwidth = 200Hz
bus.write_byte_data(0x68, 0x10, 0x04)

time.sleep(0.5)

# BMG160 address, 0x68(104)
# Read data back from 0x02(02), 6 bytes
# X-Axis LSB, X-Axis MSB, Y-Axis LSB, Y-Axis MSB, Z-Axis LSB, Z-Axis MSB
data = bus.read_i2c_block_data(0x68, 0x02, 6)

# Convert the data
xGyro = data[1] * 256 + data[0]
if xGyro > 32767 :
    xGyro -= 65536

yGyro = data[3] * 256 + data[2]
if yGyro > 32767 :
	yGyro -= 65536

zGyro = data[5] * 256 + data[4]
if zGyro > 32767 :
	zGyro -= 65536

# Output data to screen
print "X-Axis of Rotation : %d" %xGyro
print "Y-Axis of Rotation : %d" %yGyro
print "Z-Axis of Rotation : %d" %zGyro
