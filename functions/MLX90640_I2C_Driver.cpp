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

static i2c_if* i2c;

namespace mlx90640 {
void I2CInit(i2c_if* iface) {
    i2c = iface;
}

int I2CBusReset(void) {
    bool ok = i2c->busReset();
    if (!ok) { return -1; }
    return 0;
}

int I2CRead(uint16_t regAddr, uint16_t nMemAddressRead, uint16_t *data) {
    int cnt = 0;
    int i = 0;
    uint8_t cmd[2] = {0, 0};
    uint8_t i2cData[1664] = {0};

    cmd[0] = regAddr >> 8;
    cmd[1] = regAddr & 0x00FF;

    bool ok = i2c->write(cmd, 2, true);
    if (!ok) { return -1; }

    ok = i2c->read(i2cData, 2 * nMemAddressRead, false);
    if (!ok) { return -1; }

    uint16_t *p = data;
    for (cnt = 0; cnt < nMemAddressRead; cnt++) {
        i = cnt << 1;
        *p++ = ((uint16_t)i2cData[i] << 8) + (uint16_t)i2cData[i + 1];
    }

    return 0;
}

int I2CWrite(uint16_t regAddr, uint16_t data) {
    uint8_t cmd[4] = {0, 0, 0, 0};
    static uint16_t dataCheck;

    cmd[0] = regAddr >> 8;
    cmd[1] = regAddr & 0x00FF;
    cmd[2] = data >> 8;
    cmd[3] = data & 0x00FF;

    bool ok = i2c->write(cmd, 4, false);

    if (!ok) { return -1; }

    I2CRead(regAddr, 1, &dataCheck);

    if (dataCheck != data) { return -2; }

    return 0;
}
} /* namespace mlx90640 */
