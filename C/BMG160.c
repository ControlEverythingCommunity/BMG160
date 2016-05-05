// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMG160
// This code is designed to work with the BMG160_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Gyro?sku=BMG160_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, BMG160 I2C address is 0x68(104)
	ioctl(file, I2C_SLAVE, 0x68);
	
	// Select range register(0x0F)
	// Configure full scale range, 2000 dps(0x80)
	char config[2] = {0};
	config[0] = 0x0F;
	config[1] = 0x80;
	write(file, config, 2);

	// Select bandwidth register(0x10)
	// Bandwidth 200 Hz(0x04)
	config[0] = 0x10;
	config[1] = 0x04;
	write(file, config, 2);
	
	sleep(1);

	// Read 6 bytes of data(0x02)
	// xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
	char reg[1] = {0x02};
	write(file, reg, 1);
	char data[6] = {0};
	if(read(file, data, 6) != 6)
	{
		printf("Error : Input/output Error \n");
	}
	else
	{
		// Convert the data
		int xGyro = (data[1] * 256) + data[0];
		if(xGyro > 32767)
		{
			xGyro -= 65536;
		}

		int yGyro = (data[3] * 256) + data[2];
		if(yGyro > 32767)
		{
			yGyro -= 65536;
		}

		int zGyro = (data[5] * 256) + data[4];
		if(zGyro > 32767)
		{
			zGyro -= 65536;
		}

		// Output data to screen
		printf("Rotation in X-Axis : %d \n", xGyro);
		printf("Rotation in Y-Axis : %d \n", yGyro);
		printf("Rotation in Z-Axis : %d \n", zGyro);
	}
}