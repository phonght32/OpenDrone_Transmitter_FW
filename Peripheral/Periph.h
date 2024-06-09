// MIT License

// Copyright (c) 2024 phonght32

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __PERIPH_H__
#define __PERIPH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "err_code.h"

typedef struct {
	uint16_t left_joystick_x;
	uint16_t left_joystick_y;
	uint16_t right_joystick_x;
	uint16_t right_joystick_y;
} periph_operator_data_t;

err_code_t PeriphSensor_Init(void);
err_code_t PeriphSensor_GetJoystickScale(periph_operator_data_t *data);

err_code_t PeriphRadio_Init(void);
err_code_t PeriphRadio_Send(uint8_t *data);
err_code_t PeriphRadio_ClearTransmitIrqFlags(void);

err_code_t PeriphDisplay_Init(void);
err_code_t PeriphDisplay_ShowJoystickData(uint16_t left_joystick_x, uint16_t left_joystick_y, uint16_t right_joystick_x, uint16_t right_joystick_y);
err_code_t PeriphDisplay_ShowStabilizerMessage(uint8_t throttle, uint8_t roll, uint8_t pitch, uint8_t yaw);


#ifdef __cplusplus
}
#endif

#endif /* __PERIPH_H__ */
