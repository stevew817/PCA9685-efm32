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

/* Include definitions and configuration */
#include "PCA9685.h"

/* Include emlib headers */
#include "em_device.h"
#include "em_i2c.h"

/* Configuration-based structures */
static const uint8_t device_chain[] = PCA9685_DEVICE_ADDRESSES;
static const uint16_t mask_chain[] = PCA9685_OUTPUT_USED;

bool PCA9685_init(void)
{
  I2C_TransferReturn_TypeDef ret;
  I2C_TransferSeq_TypeDef transfer = {
    .flags = I2C_FLAG_WRITE_WRITE
  };

  uint8_t reg = PCA9685_REG_MODE1;
  uint8_t mode[2] = {0x21, 0x04}; // Set Auto-Increment on, enable all-call, totem-pole output 0 when off

  transfer.buf[0].data = &reg;
  transfer.buf[0].len = 1;
  transfer.buf[1].data = mode;
  transfer.buf[1].len = sizeof(mode);

  for(unsigned int i = 0; i < sizeof(device_chain); i++) {
    transfer.addr = device_chain[i];

    ret = I2C_TransferInit(PCA9685_I2C_INSTANCE, &transfer);
    while (ret == i2cTransferInProgress)
    {
      ret = I2C_Transfer(PCA9685_I2C_INSTANCE);
    }

    if(ret != i2cTransferDone) {
      return false;
    }
  }

  return true;
}

bool PCA9685_write(uint32_t chno, uint16_t pwmvalue)
{
  unsigned int devno = chno >> 4;
  uint8_t ch = chno & 0xF;
  uint8_t reg = PCA9685_REG_LED_BASE + (chno * 4);

  uint8_t values[4] = {(pwmvalue & 0xFF), ((pwmvalue >> 8) & 0xF), 0, 0}; /* Regulation is done with LED_ON = 0 and LED_OFF = PWM value */

  I2C_TransferReturn_TypeDef ret;
  I2C_TransferSeq_TypeDef transfer = {
    .addr = device_chain[devno],
    .flags = I2C_FLAG_WRITE_WRITE,
    .buf = {
      {.data = &reg,
       .len = 1},
      {.data = values,
       .len = 4},
    }
  };

  if(devno >= sizeof(device_chain)) {
    /* out of range */
    return false;
  }

  if((mask_chain[devno] & (1 << ch)) == 0) {
    /* channel masked */
    return false;
  }

  ret = I2C_TransferInit(PCA9685_I2C_INSTANCE, &transfer);
  while (ret == i2cTransferInProgress)
  {
    ret = I2C_Transfer(PCA9685_I2C_INSTANCE);
  }

  if(ret != i2cTransferDone) {
    return false;
  }

  return true;
}

bool PCA9685_read(uint32_t chno, uint16_t *pwmvalue)
{
  unsigned int devno = chno >> 4;
  uint8_t ch = chno & 0xF;
  uint8_t reg = PCA9685_REG_LED_BASE + (chno * 4);

  uint8_t values[4]; /* Regulation is done with LED_ON = 0 and LED_OFF = PWM value */

  I2C_TransferReturn_TypeDef ret;
  I2C_TransferSeq_TypeDef transfer = {
    .addr = device_chain[devno],
    .flags = I2C_FLAG_WRITE_READ,
    .buf = {
      {.data = &reg,
       .len = 1},
      {.data = values,
       .len = 4},
    }
  };

  if(devno >= sizeof(device_chain)) {
    /* out of range */
    return false;
  }

  if((mask_chain[devno] & (1 << ch)) == 0) {
    /* channel masked */
    return false;
  }

  ret = I2C_TransferInit(PCA9685_I2C_INSTANCE, &transfer);
  while (ret == i2cTransferInProgress)
  {
    ret = I2C_Transfer(PCA9685_I2C_INSTANCE);
  }

  if(ret != i2cTransferDone) {
    return false;
  }

  *pwmvalue = values[0] | (values[1] << 8);

  return true;
}

bool PCA9685_write_range(uint32_t chstart, uint32_t chend, uint16_t pwmvalue)
{
  uint8_t devno, ch;
  for(unsigned int channel = chstart; channel <= chend; channel++) {
    ch = channel & 0xF;
    devno = channel >> 4;
    if((mask_chain[devno] & (1 << ch)) == 0) {
      /* channel masked */
      continue;
    }
    if(!PCA9685_write(channel, pwmvalue)) {
      return false;
    }
  }
}

bool PCA9685_sleep(bool sleep)
{
  I2C_TransferReturn_TypeDef ret;
  I2C_TransferSeq_TypeDef transfer = {
    .flags = I2C_FLAG_WRITE_WRITE
  };

  uint8_t reg = PCA9685_REG_MODE1;
  uint8_t mode = sleep ? 0x31 : 0xA1;

  transfer.buf[0].data = &reg;
  transfer.buf[0].len = 1;
  transfer.buf[1].data = &mode;
  transfer.buf[1].len = 1;

  for(unsigned int i = 0; i < sizeof(device_chain); i++) {
    transfer.addr = device_chain[i];

    ret = I2C_TransferInit(PCA9685_I2C_INSTANCE, &transfer);
    while (ret == i2cTransferInProgress)
    {
      ret = I2C_Transfer(PCA9685_I2C_INSTANCE);
    }

    if(ret != i2cTransferDone) {
      return false;
    }
  }

  return true;
}