# libmma8451
A small C library for interacting with a [MMA8451 accelerometer](https://www.adafruit.com/product/2019)
using libi2c.

Built from the [reference documentation](https://www.nxp.com/docs/en/data-sheet/MMA8451Q.pdf)
provided by NXP.

The software was built originally for use with the Raspberry Pi but should work with
any Linux system with the I2C kernel module and libi2c installed. The library supports working
with multiple accelerometers at once.

To build and install the library and test application:

    $ make
    $ sudo make install

To run the test application:

    $ mma8451-test /dev/i2c-1 0x1c
    Attempting to open /dev/i2c-1 and talk to I2C device at 0x1c
    Successfully opened device, initializing...
    Successfully initialized, starting capture. (Press Ctrl-C to stop)
    x=-0.612916, y=-9.040505, z=3.524265, samplesPerSecond=1477.832512

On older versions of Raspbian the i2c_bcm2708 kernel module needed to be set to combined mode.
This can be done by doing the following:

    $ sudo sh -c "echo -n 1 > /sys/module/i2c_bcm2708/parameters/combined"