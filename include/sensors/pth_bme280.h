/*--------------------------------------------------------------------------
File   : pth_bme280.h

Author : Hoang Nguyen Hoan          			Feb. 12, 2017

Desc   : BME280 environment sensor implementation
			- Temperature, Humidity, Barometric pressure

Copyright (c) 2017, I-SYST inc., all rights reserved

Permission to use, copy, modify, and distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright
notice and this permission notice appear in all copies, and none of the
names : I-SYST or its contributors may be used to endorse or
promote products derived from this software without specific prior written
permission.

For info or contributing contact : hnhoan at i-syst dot com

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------
Modified by          Date              Description

----------------------------------------------------------------------------*/
#ifndef __PTH_BME280_H__
#define __PTH_BME280_H__

#include <stdint.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "iopincfg.h"
#include "pth_sensor.h"

// Device address depending on SDO wiring
#define BME280_I2C_DEV_ADDR0			0x76	// SDO to GND
#define BME280_I2C_DEV_ADDR1			0x77	// SDO to VCC

#define BME280_REG_HUM_LSB				0xFE
#define BME280_REG_HUM_MSB				0xFD
#define BME280_REG_TEMP_XLSB			0xFC
#define BME280_REG_TEMP_LSB				0xFB
#define BME280_REG_TEMP_MSB				0xFA
#define BME280_REG_PRESS_XLSB			0xF9
#define BME280_REG_PRESS_LSB			0xF8
#define BME280_REG_PRESS_MSB			0xF7
#define BME280_REG_CONFIG				0xF5
#define BME280_REG_CTRL_MEAS			0xF4
#define BME280_REG_STATUS				0xF3
#define BME280_REG_CTRL_HUM				0xF2
#define BME280_REG_CALIB_26_41_START	0xE1
#define BME280_REG_RESET				0xE0
#define BME280_REG_ID					0xD0
#define BME280_REG_CALIB_00_25_START	0x88

#define BME280_ID						0x60

#define BME280_REG_CTRL_MEAS_MODE_MASK		3
#define BME280_REG_CTRL_MEAS_MODE_SLEEP		0
#define BME280_REG_CTRL_MEAS_MODE_FORCED	1
#define BME280_REG_CTRL_MEAS_MODE_NORMAL	3

#define BME280_REG_RESET_VAL			0xB6

#pragma pack(push, 1)
typedef struct {
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
	uint8_t dig_H1;
	int16_t dig_H2;
	uint8_t dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	int8_t dig_H6;
} BME280_CALIB_DATA;
#pragma pack(pop)

#ifdef __cplusplus

class PthBme280 : public PTHSensor {
public:
	PthBme280() : vCalibTFine(0), vRegWrMask(0xFF) {}
	virtual ~PthBme280() {}
	virtual bool Init(const PTHSENSOR_CFG &CfgData, DeviceIntrf *pIntrf);

	/**
	 * @brief Set operating mode
	 *
	 * @param OpMode : Operating mode
	 * 					- PTHSENSOR_OPMODE_SLEEP
	 * 					- PTHSENSOR_OPMODE_SINGLE
	 * 					- PTHSENSOR_OPMODE_CONTINUOUS
	 * @param Freq : Sampling frequency in Hz for continuous mode
	 *
	 * @return true- if success
	 */
	virtual bool SetMode(PTHSENSOR_OPMODE OpMode, uint32_t Freq);

	/**
	 * @brief	Start sampling data
	 *
	 * @return	true - success
	 */
	virtual bool StartSampling();
	virtual bool Enable();
	virtual void Disable();
	virtual void Reset();
	bool ReadPTH(PTHSENSOR_DATA &PthData);
	float ReadTemperature() {
		PTHSENSOR_DATA pthdata;
		ReadPTH(pthdata);
		return (float)pthdata.Temperature / 100.0;
	}

	float ReadPressure() {
		PTHSENSOR_DATA pthdata;
		ReadPTH(pthdata);
		return (float)pthdata.Pressure / 100.0;
	}

	float ReadHumidity() {
		PTHSENSOR_DATA pthdata;
		ReadPTH(pthdata);
		return (float)pthdata.Humidity / 100.0;
	}

private:

	uint32_t CompenPress(int32_t RawPress);
	int32_t CompenTemp(int32_t RawTemp);
	uint32_t CompenHum(int32_t RawHum);

	int32_t vCurTemp;
	uint32_t vCurBarPres;
	uint32_t vCurRelHum;
	int32_t vCalibTFine;	// For internal calibration use only
	BME280_CALIB_DATA vCalibData;
	uint8_t vCtrlReg;
	uint8_t vRegWrMask;
};

extern "C" {
#endif	// __cplusplus

#ifdef __cplusplus
}

#endif	// __cplusplus

#endif	// __PHT_BME280_H__
