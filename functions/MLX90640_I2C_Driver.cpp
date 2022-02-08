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
i2c_inst_t *i2c = i2c0;
constexpr int i2c_timeout_us = 10000; // 10s

void MLX90640_I2CInit() {
    // i2c.stop();
}

int MLX90640_I2CGeneralReset(void) {
    int ack;
    uint8_t cmd[2] = {0x00, 0x06};

    // i2c.stop();
    // wait_us(5);
    ack = i2c_write_timeout_us(i2c, cmd[0], &cmd[1], 1, false, i2c_timeout_us);
    // ack = i2c.write(cmd[0], &cmd[1], 1, 0);

    if (ack == PICO_ERROR_GENERIC || ack == PICO_ERROR_TIMEOUT) { return -1; }
    // i2c.stop();

    // wait_us(50);

    return 0;
}

int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress,
                     uint16_t nMemAddressRead, uint16_t *data) {
    int ack = 0;
    int cnt = 0;
    int i = 0;
    uint8_t cmd[2] = {0, 0};
    uint8_t i2cData[1664] = {0};
    uint16_t *p;

    p = data;
    cmd[0] = startAddress >> 8;
    cmd[1] = startAddress & 0x00FF;

    // i2c.stop();
    // wait_us(5);
    ack = i2c_write_timeout_us(i2c, slaveAddr, cmd, 2, true, i2c_timeout_us);

    if (ack == PICO_ERROR_GENERIC || ack == PICO_ERROR_TIMEOUT) { return -1; }

    ack = i2c_read_timeout_us(i2c, slaveAddr, i2cData, 2 * nMemAddressRead,
                              false, i2c_timeout_us);

    if (ack == PICO_ERROR_GENERIC || ack == PICO_ERROR_TIMEOUT) { return -1; }
    // i2c.stop();

    for (cnt = 0; cnt < nMemAddressRead; cnt++) {
        i = cnt << 1;
        *p++ = (uint16_t)i2cData[i] * 256 + (uint16_t)i2cData[i + 1];
    }

    return 0;
}

void MLX90640_I2CFreqSet(int freq) { i2c_set_baudrate(i2c, freq); }

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data) {
    int ack = 0;
    uint8_t cmd[4] = {0, 0, 0, 0};
    static uint16_t dataCheck;

    cmd[0] = writeAddress >> 8;
    cmd[1] = writeAddress & 0x00FF;
    cmd[2] = data >> 8;
    cmd[3] = data & 0x00FF;

    ack = i2c_write_timeout_us(i2c, slaveAddr, cmd, 4, false, i2c_timeout_us);

    if (ack == PICO_ERROR_GENERIC || ack == PICO_ERROR_TIMEOUT) { return -1; }

    MLX90640_I2CRead(slaveAddr, writeAddress, 1, &dataCheck);

    if (dataCheck != data) { return -2; }

    return 0;
}
