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
#ifndef _MLX90640_API_H_
#define _MLX90640_API_H_

#include <stdint.h>

#define SCALEALPHA 0.000001

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int16_t kVdd;
    int16_t vdd25;
    float KvPTAT;
    float KtPTAT;
    uint16_t vPTAT25;
    float alphaPTAT;
    int16_t gainEE;
    float tgc;
    float cpKv;
    float cpKta;
    uint8_t resolutionEE;
    uint8_t calibrationModeEE;
    float KsTa;
    float ksTo[5];
    int16_t ct[5];
    uint16_t alpha[768];
    uint8_t alphaScale;
    int16_t offset[768];
    int8_t kta[768];
    uint8_t ktaScale;
    int8_t kv[768];
    uint8_t kvScale;
    float cpAlpha[2];
    int16_t cpOffset[2];
    float ilChessC[3];
    uint16_t brokenPixels[5];
    uint16_t outlierPixels[5];
} paramsMLX90640;

namespace mlx90640 {
int DumpEE(uint16_t *eeData);
int SynchFrame();
int TriggerMeasurement();
int GetFrameData(uint16_t *frameData);
int ExtractParameters(uint16_t *eeData, paramsMLX90640 *mlx90640);
float GetVdd(uint16_t *frameData, const paramsMLX90640 *params);
float GetTa(uint16_t *frameData, const paramsMLX90640 *params);
void GetImage(uint16_t *frameData, const paramsMLX90640 *params,
                       float *result);
void CalculateTo(uint16_t *frameData, const paramsMLX90640 *params,
                          float emissivity, float tr, float *result);
int SetResolution(uint8_t resolution);
int GetCurResolution();
int SetRefreshRate(uint8_t refreshRate);
int GetRefreshRate();
int GetSubPageNumber(uint16_t *frameData);
int GetCurMode();
int SetInterleavedMode();
int SetChessMode();
void BadPixelsCorrection(uint16_t *pixels, float *to, int mode,
                                  paramsMLX90640 *params);
}

#ifdef __cplusplus
}
#endif

#endif
