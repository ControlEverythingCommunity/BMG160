// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMG160
// This code is designed to work with the BMG160_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Gyro?sku=BMG160_I2CS#tabs-0-product_tabset-2#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class BMG160
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, BMG160 I2C address is 0x68(104)
		I2CDevice device = bus.getDevice(0x68);

		// Select range register
		// Configure full scale range, 2000 dps
		device.write(0x0F, (byte)0x80);
		// Select bandwidth register
		// Bandwidth 200 Hz
		device.write(0x10, (byte)0x04);
		Thread.sleep(500);

		// Read 6 bytes of data
		// xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
		byte[] data = new byte[6];
		device.read(0x02, data, 0, 6);

		// Convert data
		int xGyro = ((data[1] & 0xFF) * 256 + (data[0] & 0xFF));
		if(xGyro > 32767)
		{
			xGyro -= 65536;
		}

		int yGyro = ((data[3] & 0xFF) * 256 + (data[2] & 0xFF));
		if(yGyro > 32767)
		{
			yGyro -= 65536;
		}

		int zGyro = ((data[5] & 0xFF) * 256 + (data[4] & 0xFF));
		if(zGyro > 32767)
		{
			zGyro -= 65536;
		}

		// Output data to screen
		System.out.printf("X-Axis of Rotation : %d %n", xGyro);
		System.out.printf("Y-axis of Rotation : %d %n", yGyro);
		System.out.printf("Z-axis of Rotation : %d %n", zGyro);
	}
}
