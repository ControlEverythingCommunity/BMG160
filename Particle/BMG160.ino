// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMG160
// This code is designed to work with the BMG160_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Gyro?sku=BMG160_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// BMG160 I2C address is 0x68(104)
#define Addr 0x68

int xGyro = 0, yGyro = 0, zGyro = 0;
void setup() 
{
    // Set variable
    Particle.variable("i2cdevice","BMG160");
    Particle.variable("xGyro",xGyro);
    Particle.variable("yGyro",yGyro);
    Particle.variable("zGyro",zGyro);

    // Initialise I2C communication as MASTER 
    Wire.begin();
    // Initialise Serial Communication
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select Range register
    Wire.write(0x0F);
    // Configure full scale 2000 dps
    Wire.write(0x80);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select Bandwidth register
    Wire.write(0x10);
    // Set Bandwidth = 200 Hz
    Wire.write(0x04);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    unsigned int data[6];
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write(0x02);
    // Stop I2C Transmission
    Wire.endTransmission();
    // Request 6 bytes of data
    Wire.requestFrom(Addr, 6);
    // Read 6 bytes of data
    // xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
    if(Wire.available() == 6)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
        data[4] = Wire.read();
        data[5] = Wire.read();
    }
    delay(300);

    // Convert the data
    xGyro = ((data[1] * 256) + data[0]);
    if (xGyro > 32767)
    {
        xGyro -= 65536;
    }
    yGyro = ((data[3] * 256) + data[2]);
    if (yGyro > 32767)
    {
        yGyro -= 65536;
    }
    zGyro = ((data[5] * 256) + data[4]);
    if (zGyro > 32767)
    {
        zGyro -= 65536;
    }
    
    // Output data to dashboard
    Particle.publish("X-Axis of Rotation :",  String(xGyro));
    Particle.publish("Y-Axis of Rotation :",  String(yGyro));
    Particle.publish("Z-Axis of Rotation :",  String(zGyro));
}

