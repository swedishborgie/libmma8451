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

#include "mma8451.h"
#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

mma8451* mma8451_open(char* path, unsigned char addr) {
    mma8451* dev = (mma8451*)calloc(1, sizeof(mma8451));
    unsigned char whoami;

    dev->path = (char*)calloc(1, strlen(path) + 1);
    strcpy(dev->path, path);
    dev->addr = addr;

    dev->file = open(path, O_RDWR);
    if(dev->file < 0) {
        free(dev->path);
        free(dev);
        return NULL;
    }

    if(!mma8451_get_whoami(dev, &whoami)) {
        return 0;
    }

    if(whoami != MMA8451_ID) {
        //Set the errno to operation not supported so we don't get a SUCCESS message.
        errno = EOPNOTSUPP;
        return 0;
    }

    dev->range = MMA8451_RANGE_2G;
    dev->data_size = MMA8451_14BIT_OUTPUT;

    return dev;
}

int mma8451_close(mma8451* dev) {
    if(dev == NULL) {
        return 0;
    }
    close(dev->file);
    free(dev->path);
    free(dev);
    return 1;
}

int mma8451_reset(mma8451* device) {
    mma8451_register_ctrl_reg2 cfg;
    if(!mma8451_get_ctrl_reg2(device, &cfg)) {
        return 0;
    }

    cfg.rst = 1;

    if(!mma8451_set_ctrl_reg2(device, &cfg)) {
        return 0;
    }

    device->range = MMA8451_RANGE_2G;
    device->data_size = MMA8451_14BIT_OUTPUT;

    return 1;
}

int mma8451_get_acceleration(mma8451* device, mma8451_acceleration* data) {
    if(device->data_size == MMA8451_14BIT_OUTPUT) {
        unsigned char tmp[6];
        if(!mma8451_get_i2c_register_block(device->file, device->addr, MMA8451_REGISTER_OUT_X_MSB, (unsigned char*)&tmp, 6)) {
            return 0;
        }

        data->x = (tmp[0] << 6) | (tmp[1] >> 2);
        data->y = (tmp[2] << 6) | (tmp[3] >> 2);
        data->z = (tmp[4] << 6) | (tmp[5] >> 2);

        data->x -= (data->x > MAX_14BIT_VALUE) ? MAX_14BIT_SIGNED : 0;
        data->y -= (data->y > MAX_14BIT_VALUE) ? MAX_14BIT_SIGNED : 0;
        data->z -= (data->z > MAX_14BIT_VALUE) ? MAX_14BIT_SIGNED : 0;

        if(device->range == MMA8451_RANGE_2G) {
            data->x /= (double)RANGE_DIV_2G_14BIT;
            data->y /= (double)RANGE_DIV_2G_14BIT;
            data->z /= (double)RANGE_DIV_2G_14BIT;
        } else if(device->range == MMA8451_RANGE_4G) {
            data->x /= (double)RANGE_DIV_4G_14BIT;
            data->y /= (double)RANGE_DIV_4G_14BIT;
            data->z /= (double)RANGE_DIV_4G_14BIT;
        } else if(device->range == MMA8451_RANGE_8G) {
            data->x /= (double)RANGE_DIV_8G_14BIT;
            data->y /= (double)RANGE_DIV_8G_14BIT;
            data->z /= (double)RANGE_DIV_8G_14BIT;
        }
    } else {
        unsigned char tmp[3];
        if(!mma8451_get_i2c_register_block(device->file, device->addr, MMA8451_REGISTER_OUT_X_MSB, (unsigned char*)&tmp, 3)) {
            return 0;
        }
        
        data->x = tmp[0];
        data->y = tmp[1];
        data->z = tmp[2];

        data->x -= (data->x > MAX_8BIT_VALUE) ? MAX_8BIT_SIGNED : 0;
        data->y -= (data->y > MAX_8BIT_VALUE) ? MAX_8BIT_SIGNED : 0;
        data->z -= (data->z > MAX_8BIT_VALUE) ? MAX_8BIT_SIGNED : 0;

        if(device->range == MMA8451_RANGE_2G) {
            data->x /= (double)RANGE_DIV_2G_8BIT;
            data->y /= (double)RANGE_DIV_2G_8BIT;
            data->z /= (double)RANGE_DIV_2G_8BIT;
        } else if(device->range == MMA8451_RANGE_4G) {
            data->x /= (double)RANGE_DIV_4G_8BIT;
            data->y /= (double)RANGE_DIV_4G_8BIT;
            data->z /= (double)RANGE_DIV_4G_8BIT;
        } else if(device->range == MMA8451_RANGE_8G) {
            data->x /= (double)RANGE_DIV_8G_8BIT;
            data->y /= (double)RANGE_DIV_8G_8BIT;
            data->z /= (double)RANGE_DIV_8G_8BIT;
        }
    }

    return 1;
}

int mma8451_set_range(mma8451* device, mma8451_range_scale range) {
    mma8451_register_xyz_data_cfg cfg;
    if(!mma8451_get_xyz_data_cfg(device, &cfg)) {
        return 0;
    }

    cfg.fs = range;

    if(!mma8451_set_xyz_data_cfg(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_power_mode(mma8451* device, mma8451_power_mode mode) {
    mma8451_register_ctrl_reg2 cfg;
    if(!mma8451_get_ctrl_reg2(device, &cfg)) {
        return 0;
    }

    cfg.mods = mode;

    if(!mma8451_set_ctrl_reg2(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_output_size(mma8451* device, mma8451_output_size size) {
    mma8451_register_ctrl_reg1 cfg;
    if(!mma8451_get_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    cfg.f_read = size;

    if(!mma8451_set_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_data_rate(mma8451* device, mma8451_data_rate rate) {
    mma8451_register_ctrl_reg1 cfg;
    if(!mma8451_get_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    cfg.dr = rate;

    if(!mma8451_set_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_low_noise(mma8451* device, unsigned char low_noise) {
    mma8451_register_ctrl_reg1 cfg;
    if(!mma8451_get_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    cfg.lnoise = (low_noise > 0);

    if(!mma8451_set_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_orientation_detection(mma8451* device, unsigned char orientation) {
    mma8451_register_pl_cfg cfg;
    if(!mma8451_get_pl_cfg(device, &cfg)) {
        return 0;
    }

    cfg.pl_en = (orientation > 0);

    if(!mma8451_set_pl_cfg(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_interrupt_enable(mma8451* device, unsigned char enable) {
    mma8451_register_ctrl_reg4 cfg;
    if(!mma8451_get_ctrl_reg4(device, &cfg)) {
        return 0;
    }

    cfg.int_en_drdy = (enable > 0);

    if(!mma8451_set_ctrl_reg4(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_interrupt_pin1(mma8451* device, unsigned char pin1) {
    mma8451_register_ctrl_reg5 cfg;
    if(!mma8451_get_ctrl_reg5(device, &cfg)) {
        return 0;
    }

    cfg.int_cfg_drdy = (pin1 > 0);

    if(!mma8451_set_ctrl_reg5(device, &cfg)) {
        return 0;
    }

    return 1;
}
int mma8451_set_active(mma8451* device, unsigned char active) {
    mma8451_register_ctrl_reg1 cfg;
    if(!mma8451_get_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    cfg.active = (active > 0);

    if(!mma8451_set_ctrl_reg1(device, &cfg)) {
        return 0;
    }

    return 1;
}

int mma8451_get_status(mma8451* device, mma8451_register_status* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_STATUS, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_get_f_status(mma8451* device, mma8451_register_f_status* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_F_STATUS, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    data->f_cnt = 0;
    data->f_cnt += (data->f_cnt5 << 5);
    data->f_cnt += (data->f_cnt4 << 4);
    data->f_cnt += (data->f_cnt3 << 3);
    data->f_cnt += (data->f_cnt2 << 2);
    data->f_cnt += (data->f_cnt1 << 1);
    data->f_cnt += data->f_cnt0;
    return 1;
}
int mma8451_get_f_setup(mma8451* device, mma8451_register_f_setup* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_F_SETUP, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }

    data->f_mode = 0;
    data->f_mode += (data->f_mode1 << 1);
    data->f_mode += data->f_mode0;

    data->f_wmrk = 0;
    data->f_wmrk += (data->f_wmrk5 << 5);
    data->f_wmrk += (data->f_wmrk4 << 4);
    data->f_wmrk += (data->f_wmrk3 << 3);
    data->f_wmrk += (data->f_wmrk2 << 2);
    data->f_wmrk += (data->f_wmrk1 << 1);
    data->f_wmrk +=  data->f_wmrk0;
    return 1;
}
int mma8451_set_f_setup(mma8451* device, mma8451_register_f_setup* data) {
    data->f_mode1 = (data->f_mode >> 1) & 0x1;
    data->f_mode0 = data->f_mode & 0x1;

    data->f_wmrk5 = (data->f_wmrk >> 5) & 0x1;
    data->f_wmrk4 = (data->f_wmrk >> 4) & 0x1;
    data->f_wmrk3 = (data->f_wmrk >> 3) & 0x1;
    data->f_wmrk2 = (data->f_wmrk >> 2) & 0x1;
    data->f_wmrk1 = (data->f_wmrk >> 1) & 0x1;
    data->f_wmrk0 = data->f_wmrk & 0x1;

    if(!mma8451_set_register(device, MMA8451_REGISTER_F_SETUP, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_trig_cfg(mma8451* device, mma8451_register_trig_cfg* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_TRIG_CFG, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_trig_cfg(mma8451* device, mma8451_register_trig_cfg* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_TRIG_CFG, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_sysmod(mma8451* device, mma8451_register_sysmod* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_SYSMOD, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }

    data->fgt = 0;
    data->fgt += (data->fgt_4 << 4);
    data->fgt += (data->fgt_3 << 3);
    data->fgt += (data->fgt_2 << 2);
    data->fgt += (data->fgt_1 << 1);
    data->fgt +=  data->fgt_0;

    data->mode = 0;
    data->mode += (data->sysmod1 << 1);
    data->mode += data->sysmod0;
    return 1;
}
int mma8451_get_int_source(mma8451* device, mma8451_register_int_source* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_INT_SOURCE, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_get_whoami(mma8451* device, unsigned char* whoami) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_WHO_AM_I, NULL, whoami)) {
        return 0;
    }
    return 1;
}
int mma8451_get_xyz_data_cfg(mma8451* device, mma8451_register_xyz_data_cfg* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_XYZ_DATA_CFG, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }

    data->fs = 0;
    data->fs += (data->fs1 << 1);
    data->fs += (data->fs0);
    return 1;
}
int mma8451_set_xyz_data_cfg(mma8451* device, mma8451_register_xyz_data_cfg* data) {
    data->fs1 = (data->fs >> 1) & 0x1;
    data->fs0 = (data->fs) & 0x1;

    if(!mma8451_set_register(device, MMA8451_REGISTER_XYZ_DATA_CFG, (mma8451_register_generic*)data, 0)) {
        return 0;
    }

    device->range = data->fs;

    return 1;
}
int mma8451_get_hp_filter_cutoff(mma8451* device, mma8451_register_hp_filter_cutoff* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_HP_FILTER_CUTOFF, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_hp_filter_cutoff(mma8451* device, mma8451_register_hp_filter_cutoff* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_HP_FILTER_CUTOFF, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pl_status(mma8451* device, mma8451_register_pl_status* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PL_STATUS, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pl_cfg(mma8451* device, mma8451_register_pl_cfg* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PL_CFG, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_pl_cfg(mma8451* device, mma8451_register_pl_cfg* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_PL_CFG, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pl_count(mma8451* device, unsigned char* count) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PL_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_set_pl_count(mma8451* device, unsigned char count) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_PL_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pl_bf_zcomp(mma8451* device, mma8451_register_pl_bf_zcomp* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PL_BF_ZCOMP, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }

    data->bkfr = 0;
    data->bkfr += (data->bkfr1 << 1);
    data->bkfr += (data->bkfr0);

    data->zlock = 0;
    data->zlock += (data->zlock2 << 2);
    data->zlock += (data->zlock1 << 1);
    data->zlock += (data->zlock0);
    return 1;
}
int mma8451_set_pl_bf_zcomp(mma8451* device, mma8451_register_pl_bf_zcomp* data) {
    data->bkfr1 = (data->bkfr >> 1) & 0x1;
    data->bkfr0 = (data->bkfr) & 0x1;

    data->zlock2 = (data->zlock >> 2) & 0x1;
    data->zlock1 = (data->zlock >> 1) & 0x1;
    data->zlock0 = (data->zlock) & 0x1;

    if(!mma8451_set_register(device, MMA8451_REGISTER_PL_BF_ZCOMP, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_p_l_ths_reg(mma8451* device, mma8451_register_p_l_ths_reg* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_P_L_THS_REG, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    data->p_l_ths = 0;
    data->p_l_ths += (data->p_l_ths4 << 4);
    data->p_l_ths += (data->p_l_ths3 << 3);
    data->p_l_ths += (data->p_l_ths2 << 2);
    data->p_l_ths += (data->p_l_ths1 << 1);
    data->p_l_ths += (data->p_l_ths0);

    data->hys = 0;
    data->hys += (data->hys2 << 2);
    data->hys += (data->hys1 << 1);
    data->hys += (data->hys0);
    return 1;
}
int mma8451_set_p_l_ths_reg(mma8451* device, mma8451_register_p_l_ths_reg* data) {
    data->p_l_ths4 = (data->p_l_ths >> 4) & 0x1;
    data->p_l_ths3 = (data->p_l_ths >> 3) & 0x1;
    data->p_l_ths2 = (data->p_l_ths >> 2) & 0x1;
    data->p_l_ths1 = (data->p_l_ths >> 1) & 0x1;
    data->p_l_ths0 = (data->p_l_ths) & 0x1;

    data->hys2 = (data->hys >> 2) & 0x1;
    data->hys1 = (data->hys >> 1) & 0x1;
    data->hys0 = (data->hys) & 0x1;

    if(!mma8451_set_register(device, MMA8451_REGISTER_P_L_THS_REG, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ff_mt_ths(mma8451* device, mma8451_register_ff_mt_ths* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_FF_MT_THS, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }

    data->ths = 0;
    data->ths += (data->ths6 << 6);
    data->ths += (data->ths5 << 5);
    data->ths += (data->ths4 << 4);
    data->ths += (data->ths3 << 3);
    data->ths += (data->ths2 << 2);
    data->ths += (data->ths1 << 1);
    data->ths += (data->ths0);
    return 1;
}
int mma8451_set_ff_mt_ths(mma8451* device, mma8451_register_ff_mt_ths* data) {
    data->ths6 = (data->ths >> 6) & 0x1;
    data->ths5 = (data->ths >> 5) & 0x1;
    data->ths4 = (data->ths >> 4) & 0x1;
    data->ths3 = (data->ths >> 3) & 0x1;
    data->ths2 = (data->ths >> 2) & 0x1;
    data->ths1 = (data->ths >> 1) & 0x1;
    data->ths0 = (data->ths) & 0x1;

    if(!mma8451_set_register(device, MMA8451_REGISTER_FF_MT_THS, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ff_mt_count(mma8451* device, unsigned char* count) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_FF_MT_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_set_ff_mt_count(mma8451* device, unsigned char count) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_FF_MT_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ff_mt_cfg(mma8451* device, mma8451_register_ff_mt_cfg* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_FF_MT_CFG, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_ff_mt_cfg(mma8451* device, mma8451_register_ff_mt_cfg* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_FF_MT_CFG, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ff_mt_src(mma8451* device, mma8451_register_ff_mt_src* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_FF_MT_SRC, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_get_transient_cfg(mma8451* device, mma8451_register_transient_cfg* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_TRANSIENT_CFG, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_transient_cfg(mma8451* device, mma8451_register_transient_cfg* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_TRANSIENT_CFG, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_transient_scr(mma8451* device, mma8451_register_transient_scr* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_TRANSIENT_SCR, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_get_transient_ths(mma8451* device, mma8451_register_transient_ths* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_TRANSIENT_THS, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    data->ths = 0;
    data->ths += (data->ths6 << 6);
    data->ths += (data->ths5 << 5);
    data->ths += (data->ths4 << 4);
    data->ths += (data->ths3 << 3);
    data->ths += (data->ths2 << 2);
    data->ths += (data->ths1 << 1);
    data->ths += (data->ths0);
    return 1;
}
int mma8451_set_transient_ths(mma8451* device, mma8451_register_transient_ths* data) {
    data->ths6 = (data->ths >> 6) & 0x1;
    data->ths5 = (data->ths >> 5) & 0x1;
    data->ths4 = (data->ths >> 4) & 0x1;
    data->ths3 = (data->ths >> 3) & 0x1;
    data->ths2 = (data->ths >> 2) & 0x1;
    data->ths1 = (data->ths >> 1) & 0x1;
    data->ths0 = (data->ths) & 0x1;

    if(!mma8451_set_register(device, MMA8451_REGISTER_TRANSIENT_THS, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_transient_count(mma8451* device, unsigned char* count) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_TRANSIENT_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_set_transient_count(mma8451* device, unsigned char count) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_TRANSIENT_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_cfg(mma8451* device, mma8451_register_pulse_cfg* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_CFG, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_pulse_cfg(mma8451* device, mma8451_register_pulse_cfg* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_PULSE_CFG, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_src(mma8451* device, mma8451_register_pulse_src* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_SRC, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_thsx(mma8451* device, mma8451_register_pulse_ths* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_THSX, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    data->ths = 0;
    data->ths += (data->ths6 << 6);
    data->ths += (data->ths5 << 5);
    data->ths += (data->ths4 << 4);
    data->ths += (data->ths3 << 3);
    data->ths += (data->ths2 << 2);
    data->ths += (data->ths1 << 1);
    data->ths += (data->ths0);
    return 1;
}
int mma8451_set_pulse_thsx(mma8451* device, mma8451_register_pulse_ths* data) {
    data->ths6 = (data->ths >> 6) & 0x1;
    data->ths5 = (data->ths >> 5) & 0x1;
    data->ths4 = (data->ths >> 4) & 0x1;
    data->ths3 = (data->ths >> 3) & 0x1;
    data->ths2 = (data->ths >> 2) & 0x1;
    data->ths1 = (data->ths >> 1) & 0x1;
    data->ths0 = (data->ths) & 0x1;
    if(!mma8451_set_register(device, MMA8451_REGISTER_PULSE_THSX, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_thsy(mma8451* device, mma8451_register_pulse_ths* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_THSY, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    data->ths = 0;
    data->ths += (data->ths6 << 6);
    data->ths += (data->ths5 << 5);
    data->ths += (data->ths4 << 4);
    data->ths += (data->ths3 << 3);
    data->ths += (data->ths2 << 2);
    data->ths += (data->ths1 << 1);
    data->ths += (data->ths0);
    return 1;
}
int mma8451_set_pulse_thsy(mma8451* device, mma8451_register_pulse_ths* data) {
    data->ths6 = (data->ths >> 6) & 0x1;
    data->ths5 = (data->ths >> 5) & 0x1;
    data->ths4 = (data->ths >> 4) & 0x1;
    data->ths3 = (data->ths >> 3) & 0x1;
    data->ths2 = (data->ths >> 2) & 0x1;
    data->ths1 = (data->ths >> 1) & 0x1;
    data->ths0 = (data->ths) & 0x1;
    if(!mma8451_set_register(device, MMA8451_REGISTER_PULSE_THSY, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_thsz(mma8451* device, mma8451_register_pulse_ths* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_THSZ, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    data->ths = 0;
    data->ths += (data->ths6 << 6);
    data->ths += (data->ths5 << 5);
    data->ths += (data->ths4 << 4);
    data->ths += (data->ths3 << 3);
    data->ths += (data->ths2 << 2);
    data->ths += (data->ths1 << 1);
    data->ths += (data->ths0);
    return 1;
}
int mma8451_set_pulse_thsz(mma8451* device, mma8451_register_pulse_ths* data) {
    data->ths6 = (data->ths >> 6) & 0x1;
    data->ths5 = (data->ths >> 5) & 0x1;
    data->ths4 = (data->ths >> 4) & 0x1;
    data->ths3 = (data->ths >> 3) & 0x1;
    data->ths2 = (data->ths >> 2) & 0x1;
    data->ths1 = (data->ths >> 1) & 0x1;
    data->ths0 = (data->ths) & 0x1;
    if(!mma8451_set_register(device, MMA8451_REGISTER_PULSE_THSZ, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_tmlt(mma8451* device, unsigned char* tmlt) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_TMLT, NULL, tmlt)) {
        return 0;
    }
    return 1;
}
int mma8451_set_pulse_tmlt(mma8451* device, unsigned char tmlt) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_PULSE_TMLT, NULL, tmlt)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_ltcy(mma8451* device, unsigned char* ltcy) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_LTCY, NULL, ltcy)) {
        return 0;
    }
    return 1;
}
int mma8451_set_pulse_ltcy(mma8451* device, unsigned char ltcy) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_PULSE_LTCY, NULL, ltcy)) {
        return 0;
    }
    return 1;
}
int mma8451_get_pulse_wind(mma8451* device, unsigned char* wind) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_PULSE_WIND, NULL, wind)) {
        return 0;
    }
    return 1;
}
int mma8451_set_pulse_wind(mma8451* device, unsigned char wind) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_PULSE_WIND, NULL, wind)) {
        return 0;
    }
    return 1;
}
int mma8451_get_aslp_count(mma8451* device, unsigned char* count) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_ASLP_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_set_aslp_count(mma8451* device, unsigned char count) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_ASLP_COUNT, NULL, count)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ctrl_reg1(mma8451* device, mma8451_register_ctrl_reg1* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_CTRL_REG1, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    data->aslp_rate = 0;
    data->aslp_rate += (data->aslp_rate1 << 1);
    data->aslp_rate += (data->aslp_rate0);

    data->dr = 0;
    data->dr += (data->dr2 << 2);
    data->dr += (data->dr1 << 1);
    data->dr += (data->dr0);
    return 1;
}
int mma8451_set_ctrl_reg1(mma8451* device, mma8451_register_ctrl_reg1* data) {
    data->aslp_rate1 = (data->aslp_rate >> 1) & 0x1;
    data->aslp_rate0 = (data->aslp_rate) & 0x1;

    data->dr2 = (data->dr >> 2) & 0x1;
    data->dr1 = (data->dr >> 1) & 0x1;
    data->dr0 = (data->dr) & 0x1;
    if(!mma8451_set_register(device, MMA8451_REGISTER_CTRL_REG1, (mma8451_register_generic*)data, 0)) {
        return 0;
    }

    device->data_size = data->f_read;

    return 1;
}
int mma8451_get_ctrl_reg2(mma8451* device, mma8451_register_ctrl_reg2* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_CTRL_REG2, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }

    data->smods = 0;
    data->smods += (data->smods1 << 1);
    data->smods += (data->smods0);

    data->mods = 0;
    data->mods += (data->mods1 << 1);
    data->mods += (data->mods0);
    return 1;
}
int mma8451_set_ctrl_reg2(mma8451* device, mma8451_register_ctrl_reg2* data) {
    data->smods1 = (data->smods >> 1) & 0x1;
    data->smods0 = (data->smods) & 0x1;

    data->mods1 = (data->mods >> 1) & 0x1;
    data->mods0 = (data->mods) & 0x1;
    if(!mma8451_set_register(device, MMA8451_REGISTER_CTRL_REG2, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ctrl_reg3(mma8451* device, mma8451_register_ctrl_reg3* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_CTRL_REG3, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_ctrl_reg3(mma8451* device, mma8451_register_ctrl_reg3* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_CTRL_REG3, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ctrl_reg4(mma8451* device, mma8451_register_ctrl_reg4* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_CTRL_REG4, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_ctrl_reg4(mma8451* device, mma8451_register_ctrl_reg4* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_CTRL_REG4, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_ctrl_reg5(mma8451* device, mma8451_register_ctrl_reg5* data) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_CTRL_REG5, (mma8451_register_generic*)data, NULL)) {
        return 0;
    }
    return 1;
}
int mma8451_set_ctrl_reg5(mma8451* device, mma8451_register_ctrl_reg5* data) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_CTRL_REG5, (mma8451_register_generic*)data, 0)) {
        return 0;
    }
    return 1;
}
int mma8451_get_off_x(mma8451* device, unsigned char* offset) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_OFF_X, NULL, offset)) {
        return 0;
    }
    return 1;
}
int mma8451_set_off_x(mma8451* device, unsigned char offset) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_OFF_X, NULL, offset)) {
        return 0;
    }
    return 1;
}
int mma8451_get_off_y(mma8451* device, unsigned char* offset) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_OFF_Y, NULL, offset)) {
        return 0;
    }
    return 1;
}
int mma8451_set_off_y(mma8451* device, unsigned char offset) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_OFF_Y, NULL, offset)) {
        return 0;
    }
    return 1;
}
int mma8451_get_off_z(mma8451* device, unsigned char* offset) {
    if(!mma8451_get_register(device, MMA8451_REGISTER_OFF_Z, NULL, offset)) {
        return 0;
    }
    return 1;
}
int mma8451_set_off_z(mma8451* device, unsigned char offset) {
    if(!mma8451_set_register(device, MMA8451_REGISTER_OFF_Z, NULL, offset)) {
        return 0;
    }
    return 1;
}

int mma8451_get_register(mma8451* device, mma8451_register reg, mma8451_register_generic* data, unsigned char* byteData) {
    unsigned char value;
    if(!mma8451_get_i2c_register(device->file, device->addr, reg, &value)) {
        snprintf((char*)&device->last_error, MMA8451_ERROR_SIZE, "Unable to get register %hhu: %s : %u", reg, strerror(errno), errno);
        return 0;
    }

    if(data != NULL) {
        data->bit7 = (value & 0x80) > 0;
        data->bit6 = (value & 0x40) > 0;
        data->bit5 = (value & 0x20) > 0;
        data->bit4 = (value & 0x10) > 0;
        data->bit3 = (value & 0x08) > 0;
        data->bit2 = (value & 0x04) > 0;
        data->bit1 = (value & 0x02) > 0;
        data->bit0 = (value & 0x01) > 0;
    }

    if(byteData != NULL) {
        *byteData = value;
    }

    return 1;
}

int mma8451_set_register(mma8451* device, mma8451_register reg, mma8451_register_generic* data, unsigned char byteData) {
    unsigned char value = 0;

    if(data != NULL) {
        value += (data->bit7 & 0x1) << 7;
        value += (data->bit6 & 0x1) << 6;
        value += (data->bit5 & 0x1) << 5;
        value += (data->bit4 & 0x1) << 4;
        value += (data->bit3 & 0x1) << 3;
        value += (data->bit2 & 0x1) << 2;
        value += (data->bit1 & 0x1) << 1;
        value += (data->bit0 & 0x1);
    } else {
        value = byteData;
    }

    if(!mma8451_set_i2c_register(device->file, device->addr, reg, value)) {
        snprintf((char*)&device->last_error, MMA8451_ERROR_SIZE, "Unable to set register %hhu: %s : %u", reg, strerror(errno), errno);
        return 0;
    }
    return 1;
}

int mma8451_set_i2c_register(int file, unsigned char addr, unsigned char reg, unsigned char value) {
    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;

    outbuf[0] = reg;
    outbuf[1] = value;

    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 0;
    }

    return 1;
}


int mma8451_get_i2c_register(int file, unsigned char addr, unsigned char reg, unsigned char *val) {
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = sizeof(inbuf);
    messages[1].buf   = &inbuf;

    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 0;
    }
    *val = inbuf;

    return 1;
}

int mma8451_get_i2c_register_block(int file, unsigned char addr, unsigned char reg, unsigned char *buf, unsigned int cnt) {
    unsigned char outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = cnt;
    messages[1].buf   = buf;

    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 0;
    }
    return 1;
}