/**
 * @copyright (C) 2017 Melexis N.V.
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
 *
 */
#include "MLX90640_I2C_Driver.h"

// #include <hardware/i2c.h>
// i2c_inst_t *i2c = i2c0;
// constexpr int i2c_timeout_us = 10000; // 10s
static i2c_if* i2c;

void MLX90640_I2CInit(i2c_if* iface) {
    i2c = iface;
}

int MLX90640_I2CGeneralReset(void) {
    uint8_t cmd[2] = {0x00, 0x06};
    bool ok = i2c->write(cmd[0], &cmd[1], 1, false);
    if (!ok) { return -1; }
    return 0;
}

int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress,
                     uint16_t nMemAddressRead, uint16_t *data) {
    int cnt = 0;
    int i = 0;
    uint8_t cmd[2] = {0, 0};
    uint8_t i2cData[1664] = {0};
    uint16_t *p;

    p = data;
    cmd[0] = startAddress >> 8;
    cmd[1] = startAddress & 0x00FF;

    bool ok = i2c->write(slaveAddr, cmd, 2, true);
    if (!ok) { return -1; }

    ok = i2c->read(slaveAddr, i2cData, 2 * nMemAddressRead, false);
    if (!ok) { return -1; }

    for (cnt = 0; cnt < nMemAddressRead; cnt++) {
        i = cnt << 1;
        *p++ = ((uint16_t)i2cData[i] << 8) + (uint16_t)i2cData[i + 1];
    }

    return 0;
}

void MLX90640_I2CFreqSet(int freq) { i2c->setFreq(freq); }

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data) {
    uint8_t cmd[4] = {0, 0, 0, 0};
    static uint16_t dataCheck;

    cmd[0] = writeAddress >> 8;
    cmd[1] = writeAddress & 0x00FF;
    cmd[2] = data >> 8;
    cmd[3] = data & 0x00FF;

    bool ok = i2c->write(slaveAddr, cmd, 4, false);

    if (!ok) { return -1; }

    MLX90640_I2CRead(slaveAddr, writeAddress, 1, &dataCheck);

    if (dataCheck != data) { return -2; }

    return 0;
}
