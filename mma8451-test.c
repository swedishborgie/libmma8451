/*
 * libmma8451 - Library for controlling and reading from MMA8451 accelerometers.
 * Copyright (C) 2017  Michael Powers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/*
 * This is a simple example of how to open and read from an
 * accelerometer using the libmma8451 library.
 */
#include "mma8451.h"
#include <stdio.h>
#include <time.h>

/**
 * Prints the usage statement for this application.
 */
void printUsage() {
    printf("Usage: mma8451-test [device path] [i2c address]\n");
    printf("  e.g. mma8451 /dev/i2c-1 0x1c\n\n");
}

/**
 * Initializes the accelerometer and gets it ready to capture.
 * @param dev Device to initialize.
 * @return 1 on success, 0 on failure.
 */
int initializeDevice(mma8451* dev) {
    //Reset the device back to defaults.
	mma8451_reset(dev);

    //Wait one second, it takes the device a short amount of time
    //to come back.
    sleep(1);

    //Set the bit width of the acceleration values.
    if(!mma8451_set_output_size(dev, MMA8451_8BIT_OUTPUT)) return 0;
    //Set the noise mode.
    if(!mma8451_set_low_noise(dev, 1)) return 0;
    //Set the power mode.
    if(!mma8451_set_power_mode(dev, MMA8451_POWER_MODE_HIGH_RES)) return 0;
    //Turn on orientation detection.
    if(!mma8451_set_orientation_detection(dev, 1)) return 0;
    //Set the accuracy range.
    if(!mma8451_set_range(dev, MMA8451_RANGE_2G)) return 0;
    //Enable interrupts.
    if(!mma8451_set_interrupt_enable(dev, 1)) return 0;
    //Set interrupts for pin1 on the accelerometer.
    if(!mma8451_set_interrupt_pin1(dev, 1)) return 0;
    //Start capturing.
    if(!mma8451_set_active(dev, 1)) return 0;

    return 1;
}

/**
 * Main function, starts the program and attempts to open the accelerometer
 * and read from it.
 */
int main(int argc, char** argv) {
    struct timespec start;
    struct timespec sample;
    unsigned int samples = 0;
    char* path;
    unsigned char address;

    //Make sure we have the right number of command line arguments.
    if(argc != 3) {
        printUsage();
        return -1;
    }

    //Parse the arguments.
    path = argv[1];
    address = (unsigned char)strtol(argv[2], NULL, 0);

    printf("Attempting to open %s and talk to I2C device at 0x%02x\n", path, address);
    mma8451* dev = mma8451_open(path, address);
    if(dev == NULL) {
        perror("Unable to open device.");
        return -1;
    }

    printf("Successfully opened device, initializing...\n");
    if(!initializeDevice(dev)) {
        perror("Unable to initialize device.");
        return -2;
    }

    printf("Successfully initialized, starting capture. (Press Ctrl-C to stop)\n");
    clock_gettime(CLOCK_REALTIME, &start);

    printf("\n");
    while(1) {
        mma8451_acceleration data;
        long long duration;
        double samplesPerSecond;

        //Actually read the acceleration data from the accelerometer.
        if(!mma8451_get_acceleration(dev, &data)) {
            return 0;
        }

        clock_gettime(CLOCK_REALTIME, &sample);
        samples++;

        //Calculate the samples per second.
        duration = ((sample.tv_sec - start.tv_sec) * 1000);
        duration += (sample.tv_nsec - start.tv_nsec) / 1.0e6;
        samplesPerSecond = (duration > 0) ? samples / ((double)duration / 1000) : 0;

        //Every 100th sample, print a message showing the latest measurements.
        if(samples % 100 == 0) {
            //Move up a line and clear to the beginning of the line.
            printf("%c[1A%c[K", 0x1b, 0x1b);
            //Print the sample.
            printf("x=%f, y=%f, z=%f, samplesPerSecond=%f\n", data.x, data.y, data.z, samplesPerSecond);
        }
    }

    mma8451_close(dev);
    return 0;
}