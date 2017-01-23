/*--------------------------------------------------------------------------
File   : ble_periph_app.h

Author : Hoang Nguyen Hoan          Dec 26, 2016

Desc   : Nordic SDK based BLE peripheral application creation helper

Copyright (c) 2016, I-SYST inc., all rights reserved

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
#ifndef __BLE_PERIPH_APP_H__
#define __BLE_PERIPH_APP_H__

#define APP_TIMER_PRESCALER             	0   /**< Value of the RTC1 PRESCALER register. */

typedef enum _BleAppMode {
	BLEAPP_MODE_LOOP,		// just main loop, No scheduler, no RTOS
	BLEAPP_MODE_APPSCHED,	// use app_cheduler
	BLEAPP_MODE_RTOS		// use RTOS
} BLEAPP_MODE;

// Service connection security types
typedef enum {
	BLEAPP_SECTYPE_NONE,				// open, no security
	BLEAPP_SECTYPE_STATICKEY_NO_MITM,	// Bonding static pass key without Man In The Middle
	BLEAPP_SECTYPE_STATICKEY_MITM,		// Bonding static pass key with MITM
	BLEAPP_SECTYPE_LESC_MITM,			// LE secure encryption
	BLEAPP_SECTYPE_SIGNED_NO_MITM,		// AES signed encryption without MITM
	BLEAPP_SECTYPE_SIGNED_MITM,			// AES signed encryption with MITM
} BLEAPP_SECTYPE;

#define BLEAPP_SECEXCHG_NONE		0
#define BLEAPP_SECEXCHG_KEYBOARD	(1<<0)
#define BLEAPP_SECEXCHG_DISPLAY		(1<<1)
#define BLEAPP_SECEXCHG_OOB			(1<<2)

#define BLEAPP_DEVNAME_MAX_SIZE			10
#define BLEAPP_INFOSTR_MAX_SIZE			20

typedef void (*PRIVINITCB)();

#pragma pack(push, 4)

typedef struct _BleAppConfig {
	BLEAPP_MODE AppMode;
	const char *pDevName;		// Device name
	const char *pModelName;		// Model name
	const char *pManufName;		// Manufacturer name
	const char *pSerialNoStr;	// Serial number string
	const char *pFwVerStr;		// Firmware version string
	const char *pHwVerStr;		// Hardware version string
	uint16_t VendorID;			// PnP Bluetooth/USB vendor id
	uint16_t ProductId;			// PnP Product ID
	const uint8_t *pManData;	// Manufacture specific data to advertise
	int ManDataLen;				// Length of manufacture specific data
	BLEAPP_SECTYPE SecType;		// Secure connection type
	uint8_t SecExchg;			// Sec key exchange
	const ble_uuid_t *pAdvUuids;// Service uuids to advertise
	int NbAdvUuid;				// Total number of uuids
	uint32_t AdvInterval;		// In msec
	uint32_t AdvTimeout;		// In sec
	uint32_t AdvSlowInterval;	// Slow advertising interval, if > 0, fallback to
								// slow interval on adv timeout and advertise until connected
	int ConnLedPort;
	int ConnLedPin;
	softdevice_evt_schedule_func_t SDEvtHandler;	// Require for BLEAPP_MODE_RTOS
} BLEAPP_CFG;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

// ***
// Require implementations per app
//
void BlePeriphAppInitUserData();
void BlePeriphAppInitServices();
void BlePeriphAppSrvcEvtDispatch(ble_evt_t * p_ble_evt);

/**
 * BLE App initialization
 */
bool BlePeriphAppInit(const BLEAPP_CFG *pBleAppCfg, bool bEraseBond);
void BlePeriphAppProcessEvt();
void BlePeriphAppEnterDfu();
void BlePeriphAppStart();

#ifdef __cplusplus
}
#endif

#endif // __BLE_PERIPH_APP_H__

