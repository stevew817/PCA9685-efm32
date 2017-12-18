/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2017 Steven Cooreman
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _PCA9685_H_
#define _PCA9685_H_

#include <stdint.h>
#include <stdbool.h>

/* Configuration values */

/** Define PCA9685_DEVICE_ADDRESSES to the array of I2C addresses on which a
 *  PCA9685 is located. Individual PWM channel numbers will be 0-15 on the
 *  first address in the array, 16-31 on the second, and so on. */
#define PCA9685_DEVICE_ADDRESSES  {0xA4, 0xA6, 0xA8}

/** Define PCA9685_OUTPUT_USED to an array of bitmasks of equal length as
 *  PCA9685_DEVICE_ADDRESSES. If a bit in the mask is set to 1, that means
 *  the LED output is enabled. If it is set to 0, then operations using this
 *  API will not take effect. */
#define PCA9685_OUTPUT_USED       {0xFFFF, 0xFFFF, 0xFFFF}

/** Define PCA9685_I2C_INSTANCE to the I2C instance to be used for communicating
 *  with the PCA9685 chain. This library expects the I2C peripheral to be set up
 *  and enabled (including I/O) before calling any of the library's functions.*/
#define PCA9685_I2C_INSTANCE      (I2C0)

/* PCA9685 constants */
#define PCA9685_REG_MODE1         (0x00)
#define PCA9685_REG_MODE2         (0x01)
#define PCA9685_REG_SUBADDR1      (0x02)
#define PCA9685_REG_SUBADDR2      (0x03)
#define PCA9685_REG_SUBADDR3      (0x04)
#define PCA9685_REG_ALLCALLADR    (0x05)
#define PCA9685_REG_LED_BASE      (0x06)
#define PCA9685_REG_ALLLED_BASE   (0xFA)
#define PCA9685_REG_PRESCALE      (0xFE)
#define PCA9685_REG_TESTMODE      (0xFF)

/* PCA9685 function prototypes */

/** Set up and initialize the PCA9685 chain.
 *  All enabled outputs are set to zero.
 *
 * @return True if successful, false if one of the addresses in the chain is
 *         offline.
 */
bool PCA9685_init(void);

/** Set the PWM value of one of the channels in the chain
 *
 * @param   chno      Zero-based index of the channel in the chain to write to.
 * @param   pwmvalue  Value to write to the given channel.
 * @return  True if successful, false if the chip is offline or the channel is
 *          disabled.
 */
bool PCA9685_write(uint32_t chno, uint16_t pwmvalue);

/** Read back the PWM value of one of the channels in the chain
 *
 * @param       chno      Zero-based index of the channel in the chain to read.
 * @param[out]  pwmvalue  Value to write to the given channel.
 * @return  True if successful, false if the chip is offline or the channel is
 *          disabled.
 */
bool PCA9685_read(uint32_t chno, uint16_t *pwmvalue);

/** Set the PWM value of a range of channels
 *
 * @param   chstart   Zero-based index of the first channel to write to.
 * @param   chend     Zero-based index of the last channel to write to.
 * @param   pwmvalue  Value to write to the given channels.
 * @return  True if successful, false if the chip is offline or the channel is
 *          disabled.
 */
bool PCA9685_write_range(uint32_t chstart, uint32_t chend, uint16_t pwmvalue);

/** Put all the PCA9685 instances to sleep.
 *
 * @param   sleep   True to enter sleep mode, false to resume from sleep.
 * @return  true if successful, false if one of the addresses in the chain is
 *          offline.
 */
bool PCA9685_sleep(bool sleep);

#endif /* _PCA9685_H_ */
