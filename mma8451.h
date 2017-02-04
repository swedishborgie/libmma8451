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

/**
 * This is the identifier the MMA8451 returns when asked for its MMA8451_REGISTER_WHO_AM_I
 * register.
 */
#define MMA8451_ID 0x1A
/**
 * Constant for acceleration due to gravity.
 */
#define GRAVITY_ACCEL 9.80665
/**
 * The divider for gravity in 2G mode for 14-bit mode.
 */
#define RANGE_DIV_2G_14BIT (0x1000 / GRAVITY_ACCEL)
/**
 * The divider for gravity in 4G mode for 14-bit mode.
 */
#define RANGE_DIV_4G_14BIT (0x800 / GRAVITY_ACCEL)
/**
 * The divider for gravity in 8G mode for 14-bit mode.
 */
#define RANGE_DIV_8G_14BIT (0x400 / GRAVITY_ACCEL)
/**
 * The divider for gravity in 2G mode for 8-bit mode.
 */
#define RANGE_DIV_2G_8BIT (0x40 / GRAVITY_ACCEL)
/**
 * The divider for gravity in 4G mode for 8-bit mode.
 */
#define RANGE_DIV_4G_8BIT (0x20 / GRAVITY_ACCEL)
/**
 * The divider for gravity in 8G mode for 8-bit mode.
 */
#define RANGE_DIV_8G_8BIT (0x10 / GRAVITY_ACCEL)
/**
 * The maximum size of an error message returned by this library.
 */
#define MMA8451_ERROR_SIZE 500
/**
 * The maximum value for a 14-bit signed sensor value.
 */
#define MAX_14BIT_SIGNED 0x4000
/**
 * The maximum value for a 14-bit sensor value.
 */
#define MAX_14BIT_VALUE  0x1FFF
/**
 * The maximum value for a 8-bit signed sensor value.
 */
#define MAX_8BIT_VALUE 0x7F
/**
 * The maximum value for a 8-bit sensor value.
 */
#define MAX_8BIT_SIGNED 0xFF

/**
 * This enumeration contains all of the different MMA8451 register ID's.
 */
typedef enum mma8451_register {
	MMA8451_REGISTER_STATUS = 0x0,
	MMA8451_REGISTER_F_STATUS = 0x0,
	MMA8451_REGISTER_OUT_X_MSB = 0x1,
	MMA8451_REGISTER_OUT_X_LSB = 0x2,
	MMA8451_REGISTER_OUT_Y_MSB = 0x3,
	MMA8451_REGISTER_OUT_Y_LSB = 0x4,
	MMA8451_REGISTER_OUT_Z_MSB = 0x5,
	MMA8451_REGISTER_OUT_Z_LSB = 0x6,
	MMA8451_REGISTER_RESERVED_1 = 0x7,
	MMA8451_REGISTER_RESERVED_2 = 0x8,
	MMA8451_REGISTER_F_SETUP = 0x9,
	MMA8451_REGISTER_TRIG_CFG = 0xA,
	MMA8451_REGISTER_SYSMOD = 0xB,
	MMA8451_REGISTER_INT_SOURCE = 0xC,
	MMA8451_REGISTER_WHO_AM_I = 0xD,
	MMA8451_REGISTER_XYZ_DATA_CFG = 0xE,
	MMA8451_REGISTER_HP_FILTER_CUTOFF = 0xF,
	MMA8451_REGISTER_PL_STATUS = 0x10,
	MMA8451_REGISTER_PL_CFG = 0x11,
	MMA8451_REGISTER_PL_COUNT = 0x12,
	MMA8451_REGISTER_PL_BF_ZCOMP = 0x13,
	MMA8451_REGISTER_P_L_THS_REG = 0x14,
	MMA8451_REGISTER_FF_MT_CFG = 0x15,
	MMA8451_REGISTER_FF_MT_SRC = 0x16,
	MMA8451_REGISTER_FF_MT_THS = 0x17,
	MMA8451_REGISTER_FF_MT_COUNT = 0x18,
	MMA8451_REGISTER_RESERVED_3 = 0x19,
	MMA8451_REGISTER_RESERVED_4 = 0x1A,
	MMA8451_REGISTER_RESERVED_5 = 0x1B,
	MMA8451_REGISTER_RESERVED_6 = 0x1C,
	MMA8451_REGISTER_TRANSIENT_CFG = 0x1D,
	MMA8451_REGISTER_TRANSIENT_SCR = 0x1E,
	MMA8451_REGISTER_TRANSIENT_THS = 0x1F,
	MMA8451_REGISTER_TRANSIENT_COUNT = 0x20,
	MMA8451_REGISTER_PULSE_CFG = 0x21,
	MMA8451_REGISTER_PULSE_SRC = 0x22,
	MMA8451_REGISTER_PULSE_THSX = 0x23,
	MMA8451_REGISTER_PULSE_THSY = 0x24,
	MMA8451_REGISTER_PULSE_THSZ = 0x25,
	MMA8451_REGISTER_PULSE_TMLT = 0x26,
	MMA8451_REGISTER_PULSE_LTCY = 0x27,
	MMA8451_REGISTER_PULSE_WIND = 0x28,
	MMA8451_REGISTER_ASLP_COUNT = 0x29,
	MMA8451_REGISTER_CTRL_REG1 = 0x2A,
	MMA8451_REGISTER_CTRL_REG2 = 0x2B,
	MMA8451_REGISTER_CTRL_REG3 = 0x2C,
	MMA8451_REGISTER_CTRL_REG4 = 0x2D,
	MMA8451_REGISTER_CTRL_REG5 = 0x2E,
	MMA8451_REGISTER_OFF_X = 0x2F,
	MMA8451_REGISTER_OFF_Y = 0x30,
	MMA8451_REGISTER_OFF_Z = 0x31
} mma8451_register;

/**
 * An enumeration containing the different supported FIFO modes.
 */
typedef enum mma8451_fifo_mode {
	/**
	 * Disables the FIFO.
	 */
	MMA8451_FIFO_MODE_DISABLED = 0,
	/**
	 * Enables the FIFO in ring buffer mode.
	 */
	MMA8451_FIFO_MODE_RING_BUFFER = 1,
	/**
	 * Enables the FIFO, buffer stops when filled.
	 */
	MMA8451_FIFO_MODE_STOP_BUFFER = 2,
	/**
	 * Enables the FIFO, triggers when the buffer fills past the high water mark.
	 */
	MMA8451_FIFO_MODE_TRIGGER = 3
} mma8451_fifo_mode;

/**
 * An enumeration containing the supported system modes.
 */
typedef enum mma8451_system_mode {
	/**
	 * Standby mode.
	 */
	MMA8451_SYSTEM_MODE_STANDBY = 0,
	/**
	 * Wake mode.
	 */
	MMA8451_SYSTEM_MODE_WAKE = 1,
	/**
	 * Sleep mode.
	 */
	MMA8451_SYSTEM_MODE_SLEEP = 2
} mma8451_system_mode;

/**
 * An enumeration containing the supported acceleration scales.
 */
typedef enum mma8451_range_scale {
	/**
	 * Range from -2g to 2g.
	 */
	MMA8451_RANGE_2G = 0,
	/**
	 * Range from -4g to 4g.
	 */
	MMA8451_RANGE_4G = 1,
	/**
	 * Range from -8g to 8g.
	 */
	MMA8451_RANGE_8G = 2,
	/**
	 * Reserved for future use.
	 */
	MMA8451_RANGE_RESERVED = 3
} mma8451_range_scale;

/**
 * An enumeration containing the supported bit scales.
 */
typedef enum mma8451_output_size {
	/**
	 * Sensor values are returned in 8-bit sensor values.
	 */
	MMA8451_8BIT_OUTPUT = 1,
	/**
	 * Sensor values are returned in 14-bit sensor values.
	 */
	MMA8451_14BIT_OUTPUT = 0
} mma8451_output_size;

/**
 * An enumeration containing the supported sleep rates.
 */
typedef enum mma8451_aslp_rate {
	/**
	 * Sleeping at 50hz.
	 */
	MMA8451_ASLP_RATE_50HZ = 0,
	/**
	 * Sleeping at 12.5hz.
	 */
	MMA8451_ASLP_RATE_12_5HZ = 1,
	/**
	 * Sleeping at 6.25hz.
	 */
	MMA8451_ASLP_RATE_6_25HZ = 2,
	/**
	 * Sleeping at 1.56hz.
	 */
	MMA8451_ASLP_RATE_1_56HZ = 3,
} mma8451_aslp_rate;

/**
 * An enumeration containing the supported data rates.
 */
typedef enum mma8451_data_rate {
	/**
	 * Samples taken at 800hz.
	 */
	MMA8451_DATA_RATE_800HZ = 0,
	/**
	 * Samples taken at 400hz.
	 */
	MMA8451_DATA_RATE_400HZ = 1,
	/**
	 * Samples taken at 200hz.
	 */
	MMA8451_DATA_RATE_200HZ = 2,
	/**
	 * Samples taken at 100hz.
	 */
	MMA8451_DATA_RATE_100HZ = 3,
	/**
	 * Samples taken at 50hz.
	 */
	MMA8451_DATA_RATE_50HZ = 4,
	/**
	 * Samples taken at 12.5hz.
	 */
	MMA8451_DATA_RATE_12_5HZ = 5,
	/**
	 * Samples taken 6.25hz.
	 */
	MMA8451_DATA_RATE_6_25HZ = 6,
	/**
	 * Samples taken at 1.56hz.
	 */
	MMA8451_DATA_RATE_1_56HZ = 7
} mma8451_data_rate;

/**
 * An enumeration containing supported power modes.
 */
typedef enum mma8451_power_mode {
	/**
	 * Normal high power mode.
	 */
	MMA8451_POWER_MODE_NORMAL = 0,
	/**
	 * Low noise, low power mode.
	 */
	MMA8451_POWER_MODE_LNOISE_LPOWER = 1,
	/**
	 * High resolution mode.
	 */
	MMA8451_POWER_MODE_HIGH_RES = 2,
	/**
	 * Lower power mode.
	 */
	MMA8451_POWER_MODE_LOW_POWER = 3
} mma8451_power_mode;

/**
 * This structure contains the bits for the STATUS register.
 */
typedef struct mma8451_register_status {
	/**
	 * X, Y, and Z axis data overwrite.
	 * 0 - No data overwrite has occurred.
	 * 1 - Previous x, y, or z data was overwritten before it was read.
	 */
	unsigned char zyxow;
	/**
	 * Z-axis data overwrite.
	 * 0 - No data overwrite has occurred.
	 * 1 - Previous Z-axis data was overwritten before it was read.
	 */
	unsigned char zow;
	/**
	 * Y-axis data overwrite.
	 * 0 - No data overwrite has occurred.
	 * 1 - Previous Y-axis data was overwritten before it was read.
	 */
	unsigned char yow;
	/**
	 * X-axis data overwrite.
	 * 0 - No data overwrite has occurred.
	 * 1 - Previous X-axis data was overwritten before it was read.
	 */
	unsigned char xow;
	/**
	 * X, Y, or Z axis has new data ready.
	 * 0 - No new data available.
	 * 1 - New set of data is available.
	 */
	unsigned char zyxdr;
	/**
	 * Z-axis daa has new data ready.
	 * 0 - No new data available.
	 * 1 - New set of data is availb
	 */
	unsigned char zdr;
	/**
	 * Y-axis daa has new data ready.
	 * 0 - No new data available.
	 * 1 - New set of data is availb
	 */
	unsigned char ydr;
	/**
	 * X-axis daa has new data ready.
	 * 0 - No new data available.
	 * 1 - New set of data is availb
	 */
	unsigned char xdr;
} mma8451_register_status;

/**
 * This stucture contains the bits for the F_STATUS register.
 */
typedef struct mma8451_register_f_status {
	/**
	 * Whether or not the FIFO has overflowed.
	 * 0 - No overflow
	 * 1 - Overflow detected.
	 */
	unsigned char f_ovf;
	/**
	 * Whether or not a FIFO watermark event has occurred.
	 * 0 - No FIFO watermark event detected.
	 * 1 - Fifo watermark detected, sample count grater than watermark value.
	 */
	unsigned char f_wmrk_flag;
	/**
	 * Bit 5 of the the FIFO sample counter.
	 */
	unsigned char f_cnt5;
	/**
	 * Bit 4 of the the FIFO sample counter.
	 */
	unsigned char f_cnt4;
	/**
	 * Bit 3 of the the FIFO sample counter.
	 */
	unsigned char f_cnt3;
	/**
	 * Bit 2 of the the FIFO sample counter.
	 */
	unsigned char f_cnt2;
	/**
	 * Bit 1 of the the FIFO sample counter.
	 */
	unsigned char f_cnt1;
	/**
	 * Bit 0 of the the FIFO sample counter.
	 */
	unsigned char f_cnt0;
	/**
	 * The FIFO sample counter, contains the number from f_cnt5 through f_cnt0.
	 */
	unsigned char f_cnt;
} mma8451_register_f_status;

/**
 * This structure contains the bits for the F_SETUP register.
 */
typedef struct mma8451_register_f_setup {
	/**
	 * Bit 1 of the f_mode flag.
	 */
	unsigned char f_mode1;
	/**
	 * Bit 0 of the f_mode flag.
	 */
	unsigned char f_mode0;
	/**
	 * Bit 5 of the f_wmrk counter.
	 */
	unsigned char f_wmrk5;
	/**
	 * Bit 4 of the f_wmrk counter.
	 */
	unsigned char f_wmrk4;
	/**
	 * Bit 3 of the f_wmrk counter.
	 */
	unsigned char f_wmrk3;
	/**
	 * Bit 2 of the f_wmrk counter.
	 */
	unsigned char f_wmrk2;
	/**
	 * Bit 1 of the f_wmrk counter.
	 */
	unsigned char f_wmrk1;
	/**
	 * Bit 0 of the f_wmrk counter.
	 */
	unsigned char f_wmrk0;
	/**
	 * The FIFO buffer overflow mode, the default is MMA8451_FIFO_MODE_DISABLED.
	 */
	mma8451_fifo_mode f_mode;
	/**
	 * FIFO event sample count watermark. Sets the number of samples required to
	 * trigger an interrupt.
	 */
	unsigned char f_wmrk;
} mma8451_register_f_setup;

/**
 * This structure contains the bits for the TRIG_CFG register.
 */
typedef struct mma8451_register_trig_cfg {
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Transient interrupt trigger bit.
	 */
	unsigned char trig_trans;
	/**
	 * Landscape/portrait orientation interrupt trigger bit.
	 */
	unsigned char trig_lndprt;
	/**
	 * Pulse interrupt trigger bit.
	 */
	unsigned char trig_pulse;
	/**
	 * Freefall motion trigger bit.
	 */
	unsigned char trig_ff_mt;
	/**
	 * Unused.
	 */
	unsigned char unused3;
	/**
	 * Unused.
	 */
	unsigned char unused4;
} mma8451_register_trig_cfg;

/**
 * This structure contains the bits for the SYSMOD register.
 */
typedef struct mma8451_register_sysmod {
	/**
	 * FIFO Gate Error
	 * 0 - No FIFO Gate error
	 * 1 - FIFO gate error.
	 */
	unsigned char fgerr;
	/**
	 * Bit 4 of fgt.
	 */
	unsigned char fgt_4;
	/**
	 * Bit 3 of fgt.
	 */
	unsigned char fgt_3;
	/**
	 * Bit 2 of fgt.
	 */
	unsigned char fgt_2;
	/**
	 * Bit 1 of fgt.
	 */
	unsigned char fgt_1;
	/**
	 * Bit 0 of fgt.
	 */
	unsigned char fgt_0;
	/**
	 * Bit 1 of sysmod.
	 */
	unsigned char sysmod1;
	/**
	 * Bit 0 of sysmod.
	 */
	unsigned char sysmod0;
	/**
	 * Number of ODR time units since fgerr was asserted. Reset when cleared.
	 */
	unsigned char fgt;
	/**
	 * System mode, defaults to STANDBY.
	 */
	mma8451_system_mode mode;
} mma8451_register_sysmod;

/**
 * This structure contains the bits for the INT_SOURCE register.
 */
typedef struct mma8451_register_int_source {
	/**
	 * Auto sleep/wake interrupt status bit.
	 */
	unsigned char src_aslp;
	/**
	 * FIFO interrupt status bit.
	 */
	unsigned char src_fifo;
	/**
	 * Transient interrupt status bit.
	 */
	unsigned char src_trans;
	/**
	 * Landscape / portrait orientation interrupt status bit.
	 */
	unsigned char src_lndprt;
	/**
	 * Pulse interrupt status bit.
	 */
	unsigned char src_pulse;
	/**
	 * Freefall motion interrupt status bit.
	 */
	unsigned char src_ff_mt;
	/**
	 * Unused.
	 */
	unsigned char unused;
	/**
	 * Data read interrupt bit status.
	 */
	unsigned char src_drdy;
} mma8451_register_int_source;

/**
 * This structure contains the bits for the XYZ_DATA_CFG register.
 */
typedef struct mma8451_register_xyz_data_cfg {
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Unused.
	 */
	unsigned char unused3;
	/**
	 * Enable high pass filter.
	 */
	unsigned char hpf_out;
	/**
	 * Unused.
	 */
	unsigned char unused4;
	/**
	 * Unused.
	 */
	unsigned char unused5;
	/**
	 * Bit 1 of the scale.
	 */
	unsigned char fs1;
	/**
	 * Bit 0 of the scale.
	 */
	unsigned char fs0;
	/**
	 * The range scale to use, 2G, 4G, or 8G.
	 */
	mma8451_range_scale fs;
} mma8451_register_xyz_data_cfg;

/**
 * This structure contains the bits for the HP_FILTER_CUTOFF register.
 */
typedef struct mma8451_register_hp_filter_cutoff {
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Bypass high pass filter for pulse processing.
	 */
	unsigned char pulse_hpf_byp;
	/**
	 * Enable low-pass filter for the pulse processing function.
	 */
	unsigned char pulse_lpf_en;
	/**
	 * Unused.
	 */
	unsigned char unused3;
	/**
	 * Unused.
	 */
	unsigned char unused4;
	/**
	 * Bit 1 of the cutoff frequency selection 
	 */
	unsigned char sel1;
	/**
	 * Bit 0 of the cutoff frequency selection
	 */
	unsigned char sel0;
} mma8451_register_hp_filter_cutoff;

/**
 * This structure contains the bits for the PL_STATUS register.
 */
typedef struct mma8451_register_pl_status {
	/**
	 * Landscape / portrait status change flag.
	 */
	unsigned char newlp;
	/**
	 * Z-tilt angle lockout
	 */
	unsigned char lo;
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Unused.
	 */
	unsigned char unused3;
	/**
	 * Bit 1 of Landscape / portrait orientation.
	 */
	unsigned char lapo1;
	/**
	 * Bit 0 of Landscape / portrait orientation.
	 */
	unsigned char lapo0;
	/**
	 * Back or front orientation.
	 */
	unsigned char bafro;
} mma8451_register_pl_status;

/**
 * This structure contains the bits for the PL_CFG register.
 */
typedef struct mma8451_register_pl_cfg {
	/**
	 * Debounce counter mode selection
	 * 0 - Decrements debounce whenever condition of interest is no longer valid.
	 * 1 - Clears counter whenever condition of interest is no longer valid.
	 */
	unsigned char dbcntm;
	/**
	 * Portrait/Landscape detection enable.
	 */
	unsigned char pl_en;
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Unused.
	 */
	unsigned char unused3;
	/**
	 * Unused.
	 */
	unsigned char unused4;
	/**
	 * Unused.
	 */
	unsigned char unused5;
	/**
	 * Unused.
	 */
	unsigned char unused6;
} mma8451_register_pl_cfg;

/**
 * This structure contains the bits for the PL_BF_ZCOMP register.
 */
typedef struct mma8451_register_pl_bf_zcomp {
	/**
	 * Bit 1 of the back front trip angle.
	 */
	unsigned char bkfr1;
	/**
	 * Bit 0 of the back front tip angle.
	 */
	unsigned char bkfr0;
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Unused.
	 */
	unsigned char unused3;
	/**
	 * Bit 2 of the z-lock angle threshold.
	 */
	unsigned char zlock2;
	/**
	 * Bit 1 of the z-lock angle threshold.
	 */
	unsigned char zlock1;
	/**
	 * Bit 0 of the z-lock angle threshold.
	 */
	unsigned char zlock0;
	/**
	 * Back front trip angle threshold. Default is 75 degrees (1). Step is 5 degrees.
	 */
	unsigned char bkfr;
	/**
	 * Z-lock angle threshold. Range is from 14 degrees to 43 degrees. Step size is 4 degrees.
	 */
	unsigned char zlock;
} mma8451_register_pl_bf_zcomp;

/**
 * This structure contains the bits for the P_L_THS_REG register.
 */
typedef struct mma8451_register_p_l_ths_reg {
	/**
	 * Bit 4 of p_l_ths.
	 */
	unsigned char p_l_ths4;
	/**
	 * Bit 3 of p_l_ths.
	 */
	unsigned char p_l_ths3;
	/**
	 * Bit 2 of p_l_ths.
	 */
	unsigned char p_l_ths2;
	/**
	 * Bit 1 of p_l_ths.
	 */
	unsigned char p_l_ths1;
	/**
	 * Bit 0 of p_l_ths.
	 */
	unsigned char p_l_ths0;
	/**
	 * Bit 2 of hys.
	 */
	unsigned char hys2;
	/**
	 * Bit 1 of hys.
	 */
	unsigned char hys1;
	/**
	 * Bit 0 of hys.
	 */
	unsigned char hys0;
	/**
	 * Portrait / landscape trip threshold angle from 15 degrees to 75 degrees.
	 * Default is 45 degrees.
	 */
	unsigned char p_l_ths;
	/**
	 * This angle is added to the threshold angle for a smoother transition from portrait
	 * to landscape and visa-versa. The angle ranges from 0 to +/- 24 degrees. Default is 100.
	 */
	unsigned char hys;
} mma8451_register_p_l_ths_reg;

/**
 * This structure contains the bits for the FF_MT_CFG register.
 */
typedef struct mma8451_register_ff_mt_cfg {
	/**
	 * Event latch enable.
	 */
	unsigned char ele;
	/**
	 * Motion detect / freefall detect flag selection.
	 */
	unsigned char oae;
	/**
	 * Event flag enable on z.
	 */
	unsigned char zefe;
	/**
	 * Event flag enable on y.
	 */
	unsigned char yefe;
	/**
	 * Event flag enable on x.
	 */
	unsigned char xefe;
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Unused.
	 */
	unsigned char unused3;
} mma8451_register_ff_mt_cfg;

/**
 * This structure contains the bits for the FF_MT_SRC register.
 */
typedef struct mma8451_register_ff_mt_src {
	/**
	 * Event active flag.
	 */
	unsigned char ea;
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Z motion flag.
	 */
	unsigned char zhe;
	/**
	 * Z motion polarity.
	 */
	unsigned char zhp;
	/**
	 * Y motion flag.
	 */
	unsigned char yhe;
	/**
	 * Y motion polarity.
	 */
	unsigned char yhp;
	/**
	 * X motion flag. 
	 */
	unsigned char xhe;
	/**
	 * X motion polarity.
	 */
	unsigned char xhp;
} mma8451_register_ff_mt_src;

/**
 * This structure contains the bits for the FF_MT_THS register.
 */
typedef struct mma8451_register_ff_mt_ths {
	/**
	 * Debounce counter mode selection.
	 * 0 - Increments or decrements debounce.
	 * 1 - Increments or clears counter.
	 */
	unsigned char dbcntm;
	/**
	 * Bit 6 of ths.
	 */
	unsigned char ths6;
	/**
	 * Bit 5 of ths.
	 */
	unsigned char ths5;
	/**
	 * Bit 4 of ths.
	 */
	unsigned char ths4;
	/**
	 * Bit 3 of ths.
	 */
	unsigned char ths3;
	/**
	 * Bit 2 of ths.
	 */
	unsigned char ths2;
	/**
	 * Bit 1 of ths.
	 */
	unsigned char ths1;
	/**
	 * Bit 0 of ths.
	 */
	unsigned char ths0;
	/**
	 * Freefall motion threshold.
	 */
	unsigned char ths;
} mma8451_register_ff_mt_ths;

/**
 * This structure contains the bits for the TRANSIENT_CFG register.
 */
typedef struct mma8451_register_transient_cfg {
	/**
	 * Unused.
	 */
	unsigned char unused1;
	/**
	 * Unused.
	 */
	unsigned char unused2;
	/**
	 * Unused.
	 */
	unsigned char unused3;
	/**
	 * Transient event flag. 
	 */
	unsigned char ele;
	/**
	 * Event flag enable on z.
	 */
	unsigned char ztefe;
	/**
	 * Event flag enable on y.
	 */
	unsigned char ytefe;
	/**
	 * Event flag enable on x.
	 */
	unsigned char xtefe;
	/**
	 * Bypass high-pass filter.
	 */
	unsigned char hpf_byp;
} mma8451_register_transient_cfg;

/**
 * This structure contains the bits for the TRANSIENT_SCR register.
 */
typedef struct mma8451_register_transient_scr {
	/**
	 * Unused.
	 */
	unsigned char unused;
	/**
	 * Event active flag.
	 */
	unsigned char ea;
	/**
	 * Z transient event.
	 */
	unsigned char ztranse;
	/**
	 * Polarity of the Z transient event that triggered interrupt.
	 */
	unsigned char z_trans_pol;
	/**
	 * Y transient event.
	 */
	unsigned char ytranse;
	/**
	 * Polarity of the Y transient event that triggered interrupt.
	 */
	unsigned char y_trans_pol;
	/**
	 * X transient event.
	 */
	unsigned char xtranse;
	/**
	 * Polarity of the X transient event that triggered interrupt.
	 */
	unsigned char x_trans_pol;
} mma8451_register_transient_scr;

/**
 * This structure contains the bits for the TRANSIENT_THS register.
 */
typedef struct mma8451_register_transient_ths {
	unsigned char dbcntm;
	unsigned char ths6;
	unsigned char ths5;
	unsigned char ths4;
	unsigned char ths3;
	unsigned char ths2;
	unsigned char ths1;
	unsigned char ths0;
	unsigned char ths;
} mma8451_register_transient_ths;

/**
 * This structure contains the bits for the PULSE_CFG register.
 */
typedef struct mma8451_register_pulse_cfg {
	unsigned char dpa;
	unsigned char ele;
	unsigned char zdpefe;
	unsigned char zspefe;
	unsigned char ydpefe;
	unsigned char yspefe;
	unsigned char xdpefe;
	unsigned char xspefe;
} mma8451_register_pulse_cfg;

/**
 * This structure contains the bits for the PULSE_SRC register.
 */
typedef struct mma8451_register_pulse_src {
	unsigned char ea;
	unsigned char axz;
	unsigned char axy;
	unsigned char axx;
	unsigned char dpe;
	unsigned char polz;
	unsigned char poly;
	unsigned char polx;
} mma8451_register_pulse_src;

/**
 * This structure contains the bits for the PULSE_THS register.
 */
typedef struct mma8451_register_pulse_ths {
	/**
	 * Unused.
	 */
	unsigned char unused;
	unsigned char ths6;
	unsigned char ths5;
	unsigned char ths4;
	unsigned char ths3;
	unsigned char ths2;
	unsigned char ths1;
	unsigned char ths0;
	unsigned char ths;
} mma8451_register_pulse_ths;

/**
 * This structure contains the bits for the CTRL_REG1 register.
 */
typedef struct mma8451_register_ctrl_reg1 {
	/**
	 * Bit 1 of the aslp_rate.
	 */
	unsigned char aslp_rate1;
	/**
	 * Bit 0 of the aslp_rate.
	 */
	unsigned char aslp_rate0;
	/**
	 * Bit 2 of dr.
	 */
	unsigned char dr2;
	/**
	 * Bit 1 of dr.
	 */
	unsigned char dr1;
	/**
	 * Bit 0 of dr.
	 */
	unsigned char dr0;
	/**
	 * Whether or not reduced noise mode is enabled.
	 */
	unsigned char lnoise;
	/**
	 * Fast read mode, enabling turns on 8-bit samples. 
	 */
	unsigned char f_read;
	/**
	 * Whether or not the device is active (1) or in standby (0).
	 */
	unsigned char active;
	/**
	 * Sets the sleep rate.
	 */
	mma8451_aslp_rate aslp_rate;
	/**
	 * Sets the data rate.
	 */
	mma8451_data_rate dr;
} mma8451_register_ctrl_reg1;

/**
 * This structure contains the bits for the CTRL_REG2 register.
 */
typedef struct mma8451_register_ctrl_reg2 {
	/**
	 * Self test enable.
	 */
	unsigned char st;
	/**
	 * Software reset.
	 */
	unsigned char rst;
	/**
	 * Unused.
	 */
	unsigned char unused;
	/**
	 * Bit 1 of smods.
	 */
	unsigned char smods1;
	/**
	 * Bit 0 of smods.
	 */
	unsigned char smods0;
	/**
	 * Auto sleep enable.
	 */
	unsigned char slpe;
	/**
	 * Bit 1 of mods.
	 */
	unsigned char mods1;
	/**
	 * Bit 0 of mods.
	 */
	unsigned char mods0;
	/**
	 * Sleep mode power scheme selection.
	 */
	mma8451_power_mode smods;
	/**
	 * Active mode power scheme selection.
	 */
	mma8451_power_mode mods;
} mma8451_register_ctrl_reg2;

/**
 * This structure contains the bits for the CTRL_REG3 register.
 */
typedef struct mma8451_register_ctrl_reg3 {
	/**
	 * Whether or not to bypass the FIFO gate.
	 */
	unsigned char fifo_gate;
	/**
	 * Whether or not to use the transient function.
	 */
	unsigned char wake_trans;
	/**
	 * Whether or not the use the orientation function.
	 */
	unsigned char wake_lndprt;
	/**
	 * Whether or not to use the pulse functino.
	 */
	unsigned char wake_pulse;
	/**
	 * Whether or not to use the freefall function.
	 */
	unsigned char wake_ff_mt;
	/**
	 * Unused.
	 */
	unsigned char unused;
	/**
	 * Interrupt plarity 0 for active low, 1 for active high.
	 */
	unsigned char ipol;
	/**
	 * 0 for push pull, 1 for open drain.
	 */
	unsigned char pp_od;
} mma8451_register_ctrl_reg3;

/**
 * This structure contains the bits for the CTRL_REG4 register.
 */
typedef struct mma8451_register_ctrl_reg4 {
	/**
	 * Auto sleep interrupt enable.
	 */
	unsigned char int_en_aslp;
	/**
	 * FIFO interrupt enable.
	 */
	unsigned char int_en_fifo;
	/**
	 * Transient interrupt enable.
	 */
	unsigned char int_en_trans;
	/**
	 * Orientation interrupt enable.
	 */
	unsigned char int_en_lndprt;
	/**
	 * Pulse interrupt enable.
	 */
	unsigned char int_en_pulse;
	/**
	 * Freefall interrupt enable.
	 */
	unsigned char int_en_ff_mt;
	/**
	 * Unused.
	 */
	unsigned char unused;
	/**
	 * Data ready interrupt enable.
	 */
	unsigned char int_en_drdy;
} mma8451_register_ctrl_reg4;

/**
 * This structure contains the bits for the CTRL_REG5 register.
 */
typedef struct mma8451_register_ctrl_reg5 {
	unsigned char int_cfg_aslp;
	unsigned char int_cfg_fifo;
	unsigned char int_cfg_trans;
	unsigned char int_cfg_lndprt;
	unsigned char int_cfg_pulse;
	unsigned char int_cfg_ff_mt;
	/**
	 * Unused.
	 */
	unsigned char unused;
	unsigned char int_cfg_drdy;
} mma8451_register_ctrl_reg5;

/**
 * This structure contains a single XYZ sample from the accelerometer.
 */
typedef struct mma8451_acceleration {
	/**
	 * X component of the acceleration due to gravity.
	 */
	double x;
	/**
	 * Y component of the acceleration due to gravity.
	 */
	double y;
	/**
	 * Z component of the acceleration due to gravity.
	 */
	double z;
} mma8451_acceleration;

/**
 * This structure represents a generic register containing eight bits.
 * This is used to inject bits into the other structures.
 */
typedef struct mma8451_register_generic {
	/**
	 * Bit 7 of the register.
	 */
	unsigned char bit7;
	/**
	 * Bit 6 of the register.
	 */
	unsigned char bit6;
	/**
	 * Bit 5 of the register.
	 */
	unsigned char bit5;
	/**
	 * Bit 4 of the register.
	 */
	unsigned char bit4;
	/**
	 * Bit 3 of the register.
	 */
	unsigned char bit3;
	/**
	 * Bit 2 of the register.
	 */
	unsigned char bit2;
	/**
	 * Bit 1 of the register.
	 */
	unsigned char bit1;
	/**
	 * Bit 0 of the register.
	 */
	unsigned char bit0;
} mma8451_register_generic;

/**
 * This structure contains information about an attached MMA8451 accelerometer.
 */
typedef struct mma8451 {
	/**
	 * The filesystem path to the device.
	 */
	char* path;
	/**
	 * A file pointer to the device.
	 */
	int file;
	/**
	 * The I2C device address.
	 */
	unsigned char addr;
	/**
	 * The configured device range.
	 */
	mma8451_range_scale range;
	/**
	 * The configured data size.
	 */
	mma8451_output_size data_size;
	/**
	 * The last error message for this device.
	 */
	char last_error[MMA8451_ERROR_SIZE];
} mma8451;

//High level functions
/**
 * This function opens an MMA8451 accelerometer.
 * @param path The path to the device.
 * @param addr the I2C address of the device.
 * @return Either a MMA8451 structure or NULL if there was an error.
 */
mma8451* mma8451_open(char* path, unsigned char addr);
/**
 * This function closes an open reference to an MMA8451 accelerometer.
 * @param device Device to close, also cleans up memory.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_close(mma8451* device);
/**
 * This function attempts to reset the device.
 * @param device Device to reset.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_reset(mma8451* device);
/**
 * This function sets the configured range scale.
 * @param device Device to change the scale on.
 * @param range The range to use.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_set_range(mma8451* device, mma8451_range_scale range);
/**
 * This function sets the configured power mode.
 * @param device Device to change.
 * @param mode Power mode to set.
 * @return 1 if successful, 0 if failure. 
 */
int mma8451_set_power_mode(mma8451* device, mma8451_power_mode mode);
/**
 * This function sets the configured data rate.
 * @param device Device to change.
 * @param rate Data rate.
 * @return 1 is successful, 0 if failure.
 */
int mma8451_set_data_rate(mma8451* device, mma8451_data_rate rate);
/**
 * This function sets the configured output size.
 * @param device Device to change.
 * @param size Output size to set.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_set_output_size(mma8451* device, mma8451_output_size size);
/**
 * This function turns low noise mode on and off.
 * @param device Device to change.
 * @param low_noise Low noise mode 1 for on, 0 for off.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_set_low_noise(mma8451* device, unsigned char low_noise);
/**
 * This function turns on and off orientation detection.
 * @param device Device to change.
 * @param orientation Orientation detection on or off.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_set_orientation_detection(mma8451* device, unsigned char orientation);
/**
 * This function enables and disables the interrupt.
 * @param device Device to change.
 * @param enable Enable or disable the interrupt.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_set_interrupt_enable(mma8451* device, unsigned char enable);
/**
 * This function sets interrupt pin 1 or interrupt pin 2.
 * @param device Device to change.
 * @param Whether or not to set INT1 (1), or INT2 (0).
 * @return 1 if successful, 0 if failure.
 */
int mma8451_set_interrupt_pin1(mma8451* device, unsigned char pin1);
/**
 * This function sets whether or not the accelerometer is active or not.
 * @param device Device to change.
 * @param active Whether or not the device is active.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_set_active(mma8451* device, unsigned char active);
/**
 * This function gets a single accelerometer sample.
 * @param device Device to read from.
 * @param data Data to fill.
 * @return 1 if successful, 0 if failure.
 */
int mma8451_get_acceleration(mma8451* device, mma8451_acceleration* data);

//Medium level functions (register reads/writes)
int mma8451_get_status(mma8451* device, mma8451_register_status* data);
int mma8451_get_f_status(mma8451* device, mma8451_register_f_status* data);
int mma8451_get_f_setup(mma8451* device, mma8451_register_f_setup* data);
int mma8451_set_f_setup(mma8451* device, mma8451_register_f_setup* data);
int mma8451_get_trig_cfg(mma8451* device, mma8451_register_trig_cfg* data);
int mma8451_set_trig_cfg(mma8451* device, mma8451_register_trig_cfg* data);
int mma8451_get_sysmod(mma8451* device, mma8451_register_sysmod* data);
int mma8451_get_int_source(mma8451* device, mma8451_register_int_source* data);
int mma8451_get_whoami(mma8451* device, unsigned char* whoami);
int mma8451_get_xyz_data_cfg(mma8451* device, mma8451_register_xyz_data_cfg* data);
int mma8451_set_xyz_data_cfg(mma8451* device, mma8451_register_xyz_data_cfg* data);
int mma8451_get_hp_filter_cutoff(mma8451* device, mma8451_register_hp_filter_cutoff* data);
int mma8451_set_hp_filter_cutoff(mma8451* device, mma8451_register_hp_filter_cutoff* data);
int mma8451_get_pl_status(mma8451* device, mma8451_register_pl_status* data);
int mma8451_get_pl_cfg(mma8451* device, mma8451_register_pl_cfg* data);
int mma8451_set_pl_cfg(mma8451* device, mma8451_register_pl_cfg* data);
int mma8451_get_pl_count(mma8451* device, unsigned char* count);
int mma8451_set_pl_count(mma8451* device, unsigned char count);
int mma8451_get_pl_bf_zcomp(mma8451* device, mma8451_register_pl_bf_zcomp* data);
int mma8451_set_pl_bf_zcomp(mma8451* device, mma8451_register_pl_bf_zcomp* data);
int mma8451_get_p_l_ths_reg(mma8451* device, mma8451_register_p_l_ths_reg* data);
int mma8451_set_p_l_ths_reg(mma8451* device, mma8451_register_p_l_ths_reg* data);
int mma8451_get_ff_mt_cfg(mma8451* device, mma8451_register_ff_mt_cfg* data);
int mma8451_set_ff_mt_cfg(mma8451* device, mma8451_register_ff_mt_cfg* data);
int mma8451_get_ff_mt_src(mma8451* device, mma8451_register_ff_mt_src* data);
int mma8451_get_ff_mt_ths(mma8451* device, mma8451_register_ff_mt_ths* data);
int mma8451_set_ff_mt_ths(mma8451* device, mma8451_register_ff_mt_ths* data);
int mma8451_get_ff_mt_count(mma8451* device, unsigned char* count);
int mma8451_set_ff_mt_count(mma8451* device, unsigned char count);
int mma8451_get_transient_cfg(mma8451* device, mma8451_register_transient_cfg* data);
int mma8451_set_transient_cfg(mma8451* device, mma8451_register_transient_cfg* data);
int mma8451_get_transient_scr(mma8451* device, mma8451_register_transient_scr* data);
int mma8451_get_transient_ths(mma8451* device, mma8451_register_transient_ths* data);
int mma8451_set_transient_ths(mma8451* device, mma8451_register_transient_ths* data);
int mma8451_get_transient_count(mma8451* device, unsigned char* count);
int mma8451_set_transient_count(mma8451* device, unsigned char count);
int mma8451_get_pulse_cfg(mma8451* device, mma8451_register_pulse_cfg* data);
int mma8451_set_pulse_cfg(mma8451* device, mma8451_register_pulse_cfg* data);
int mma8451_get_pulse_src(mma8451* device, mma8451_register_pulse_src* data);
int mma8451_get_pulse_thsx(mma8451* device, mma8451_register_pulse_ths* data);
int mma8451_set_pulse_thsx(mma8451* device, mma8451_register_pulse_ths* data);
int mma8451_get_pulse_thsy(mma8451* device, mma8451_register_pulse_ths* data);
int mma8451_set_pulse_thsy(mma8451* device, mma8451_register_pulse_ths* data);
int mma8451_get_pulse_thsz(mma8451* device, mma8451_register_pulse_ths* data);
int mma8451_set_pulse_thsz(mma8451* device, mma8451_register_pulse_ths* data);
int mma8451_get_pulse_tmlt(mma8451* device, unsigned char* tmlt);
int mma8451_set_pulse_tmlt(mma8451* device, unsigned char tmlt);
int mma8451_get_pulse_ltcy(mma8451* device, unsigned char* ltcy);
int mma8451_set_pulse_ltcy(mma8451* device, unsigned char ltcy);
int mma8451_get_pulse_wind(mma8451* device, unsigned char* wind);
int mma8451_set_pulse_wind(mma8451* device, unsigned char wind);
int mma8451_get_aslp_count(mma8451* device, unsigned char* count);
int mma8451_set_aslp_count(mma8451* device, unsigned char count);
int mma8451_get_ctrl_reg1(mma8451* device, mma8451_register_ctrl_reg1* data);
int mma8451_set_ctrl_reg1(mma8451* device, mma8451_register_ctrl_reg1* data);
int mma8451_get_ctrl_reg2(mma8451* device, mma8451_register_ctrl_reg2* data);
int mma8451_set_ctrl_reg2(mma8451* device, mma8451_register_ctrl_reg2* data);
int mma8451_get_ctrl_reg3(mma8451* device, mma8451_register_ctrl_reg3* data);
int mma8451_set_ctrl_reg3(mma8451* device, mma8451_register_ctrl_reg3* data);
int mma8451_get_ctrl_reg4(mma8451* device, mma8451_register_ctrl_reg4* data);
int mma8451_set_ctrl_reg4(mma8451* device, mma8451_register_ctrl_reg4* data);
int mma8451_get_ctrl_reg5(mma8451* device, mma8451_register_ctrl_reg5* data);
int mma8451_set_ctrl_reg5(mma8451* device, mma8451_register_ctrl_reg5* data);
int mma8451_get_off_x(mma8451* device, unsigned char* offset);
int mma8451_set_off_x(mma8451* device, unsigned char offset);
int mma8451_get_off_y(mma8451* device, unsigned char* offset);
int mma8451_set_off_y(mma8451* device, unsigned char offset);
int mma8451_get_off_z(mma8451* device, unsigned char* offset);
int mma8451_set_off_z(mma8451* device, unsigned char offset);

//Low level i2c functions.
/**
 * This function gets a register from the accelerometer.
 * @param device Device to get the register from.
 * @param reg Register to retreieve.
 * @param data Structure to inject data into, optional. If not set byteData must be set.
 * @param byteData An optional unsigned char pointer to retrieve register data.
 * @return 1 for success, 0 for failure.
 */
int mma8451_get_register(mma8451* device, mma8451_register reg, mma8451_register_generic* data, unsigned char* byteData);
/**
 * This function sets a register in the accelerometer.
 * @param device Device to set the register to.
 * @param reg Register to set.
 * @param data Structure to set, can be null, but if null byteData must be set.
 * @param byteData An optional unsigned char pointer to set.
 * @return 1 for success, 0 for failure.
 */
int mma8451_set_register(mma8451* device, mma8451_register reg, mma8451_register_generic* data, unsigned char byteData);
/**
 * This function sets an I2C register.
 * @param file File pointer to I2C bus.
 * @param addr I2C address to send to.
 * @param reg Register to set.
 * @param value Value to write.
 * @return 1 for success, 0 for failure.
 */
int mma8451_set_i2c_register(int file, unsigned char addr, unsigned char reg, unsigned char value);
/**
 * This function gets an I2C register.
 * @param file File pointer to I2C bus.
 * @param addr I2C address to get from.
 * @param reg Register to get from.
 * @param val Pointer to value to retrieve.
 * @return 1 for success, 0 for failure.
 */
int mma8451_get_i2c_register(int file, unsigned char addr, unsigned char reg, unsigned char *val);
/**
 * This function gets a register block from an I2C device.
 * @param file File pointer to I2C device.
 * @param addr I2C address.
 * @param reg Register to start reading from.
 * @param buf Buffer to read into.
 * @param cnt Number of bytes to retrieve.
 * @return 1 for success, 0 for failure.
 */
int mma8451_get_i2c_register_block(int file, unsigned char addr, unsigned char reg, unsigned char *buf, unsigned int cnt);