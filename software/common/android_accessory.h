/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Ported for NiosII by ISE  2012/01/31
 */

#ifndef _android_accessory_h_
#define _android_accessory_h_

#include "usb.h"
#include "max3421e_constants.h"

class AndroidAccessory {
private:
    const char *manufacturer;
    const char *model;
    const char *description;
    const char *version;
    const char *uri;
    const char *serial;

    bool connected;
	usb_device * adkDevice;
	usb_endpoint bulk_in;
	usb_endpoint bulk_out;

    uint8_t descBuff[256];

    bool isAccessoryDevice( usb_device *device );

    int getProtocol( usb_device *device );
    void sendString( usb_device *device, int16_t index, const char *str );
    bool switchDevice( usb_device *device );
    bool configureAndroid( usb_device *device );

public:
    AndroidAccessory(const char *manufacturer,
                     const char *model,
                     const char *description,
                     const char *version,
                     const char *uri,
                     const char *serial);

    void powerOn(void);
	void usbEventHandler(usb_device * device, usb_eventType event);

    bool isConnected(void);
    int16_t read(void *buff, int16_t len, uint16_t nakLimit = USB_NAK_LIMIT);
    int16_t write(void *buff, int16_t len);
};

#endif /* _android_accessory_h_ */
