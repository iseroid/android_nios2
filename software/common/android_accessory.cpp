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


#include <string.h>
#include "nios2_common.h"
#include "android_accessory.h"

#define USB_ACCESSORY_VENDOR_ID         0x18D1
#define USB_ACCESSORY_PRODUCT_ID        0x2D00

#define USB_ACCESSORY_ADB_PRODUCT_ID    0x2D01
#define ACCESSORY_STRING_MANUFACTURER   0
#define ACCESSORY_STRING_MODEL          1
#define ACCESSORY_STRING_DESCRIPTION    2
#define ACCESSORY_STRING_VERSION        3
#define ACCESSORY_STRING_URI            4
#define ACCESSORY_STRING_SERIAL         5

#define ACCESSORY_GET_PROTOCOL          51
#define ACCESSORY_SEND_STRING           52
#define ACCESSORY_START                 53

static void usbEventHandler_( usb_device *device, usb_eventType event, void *cb_arg )
{
	AndroidAccessory *obj = reinterpret_cast<AndroidAccessory *>(cb_arg);
	(obj->usbEventHandler)( device, event );
}


AndroidAccessory::AndroidAccessory(const char *manufacturer,
                                   const char *model,
                                   const char *description,
                                   const char *version,
                                   const char *uri,
                                   const char *serial) : manufacturer(manufacturer),
                                                         model(model),
                                                         description(description),
                                                         version(version),
                                                         uri(uri),
                                                         serial(serial),
                                                         connected(false),
														 adkDevice(NULL)
{

}

void AndroidAccessory::powerOn(void)
{
	USB::setEventHandler( usbEventHandler_, this );
	USB::init();
}

/**
 * Handles events from the USB layer.
 *
 * @param device USB device that generated the event.
 * @param event USB event.
 */
void AndroidAccessory::usbEventHandler(usb_device * device, usb_eventType event)
{
	switch( event ) {
	case USB_CONNECT:
        Serial.print("\nDevice addressed... ");
        Serial.print("Requesting device descriptor.\n");
		if( isAccessoryDevice( device ) ) {
            Serial.print("found android acessory device\n");
			connected = configureAndroid( device );
		}
		else {
            Serial.print("found possible device. swithcing to serial mode\n");
            switchDevice( device );
		}
		break;

	case USB_DISCONNECT:
		Serial.print("usb_dis\n" );
		if( device == adkDevice ) {
			Serial.print( "disconnect\n" );
			adkDevice = NULL;
			connected = false;
		}

		break;

	default:
		Serial.print( "usb_other\n" );
		// ignore
		break;
	}
}

bool AndroidAccessory::isAccessoryDevice( usb_device *device )
{
    return device->idVendor == USB_ACCESSORY_VENDOR_ID &&
        (device->idProduct == USB_ACCESSORY_PRODUCT_ID || device->idProduct == USB_ACCESSORY_ADB_PRODUCT_ID);
}

int AndroidAccessory::getProtocol( usb_device *device )
{
    uint16_t protocol = -1;

	USB::controlRequest( device,
                USB_SETUP_DEVICE_TO_HOST | USB_SETUP_TYPE_VENDOR | USB_SETUP_RECIPIENT_DEVICE,
                ACCESSORY_GET_PROTOCOL, 0, 0, 0, 2, (uint8_t *)&protocol );
    return protocol;
}

void AndroidAccessory::sendString( usb_device *device, int16_t index, const char *str )
{
	int ret;
	ret = USB::controlRequest( device, 
                USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_VENDOR | USB_SETUP_RECIPIENT_DEVICE,
                ACCESSORY_SEND_STRING,
				0,
				0,
				index,
                strlen(str) + 1,
				(uint8_t *)str );
}


bool AndroidAccessory::switchDevice( usb_device *device )
{
    int protocol = getProtocol( device );

    if( protocol == 1 ) {
        Serial.print( "device supports protcol 1\n" );
    } else {
        Serial.print( "could not read device protocol version\n" );
        return false;
    }

    sendString( device, ACCESSORY_STRING_MANUFACTURER, manufacturer);
    sendString( device, ACCESSORY_STRING_MODEL, model);
    sendString( device, ACCESSORY_STRING_DESCRIPTION, description);
    sendString( device, ACCESSORY_STRING_VERSION, version);
    sendString( device, ACCESSORY_STRING_URI, uri);
    sendString( device, ACCESSORY_STRING_SERIAL, serial);

	int ret;
	ret = USB::controlRequest( device,
                USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_VENDOR | USB_SETUP_RECIPIENT_DEVICE,
                ACCESSORY_START, 0, 0, 0, 0, NULL );

    return true;
}

bool AndroidAccessory::configureAndroid( usb_device *device )
{
	int bytesRead;

	// Read the length of the configuration descriptor.
	bytesRead = USB::getConfigurationDescriptor( device, 0, sizeof(descBuff), descBuff ); ///? 0?
	if( bytesRead < 0 ) {
		return false;
	}

	uint8_t *p = descBuff;
	uint8_t descriptorLength;
	uint8_t descriptorType;

	usb_endpointDescriptor * endpoint;

	while( p < (descBuff + bytesRead) ) {
		descriptorLength = p[0];
		descriptorType = p[1];

		switch( descriptorType ) {
		case USB_DESCRIPTOR_CONFIGURATION:
			break;
		case USB_DESCRIPTOR_INTERFACE:
			break;
		case USB_DESCRIPTOR_ENDPOINT:
			endpoint = (usb_endpointDescriptor *)p;
			if( (endpoint->bEndpointAddress & 0x80) ) {
				if( bulk_in.address == 0 ) {
					bulk_in.address = endpoint->bEndpointAddress & 0x7F;
					bulk_in.attributes = endpoint->bmAttributes;
					bulk_in.maxPacketSize = endpoint->wMaxPacketSize;
					bulk_in.sendToggle = bmSNDTOG0;
					bulk_in.receiveToggle = bmRCVTOG0;
				}
			} else {
				if( bulk_out.address == 0 ) {
					bulk_out.address = endpoint->bEndpointAddress;
					bulk_out.attributes = endpoint->bmAttributes;
					bulk_out.maxPacketSize = endpoint->wMaxPacketSize;
					bulk_out.sendToggle = bmSNDTOG0;
					bulk_out.receiveToggle = bmRCVTOG0;
				}
			}
			break;
		default:
			break;
		}

		p += descriptorLength;
	}

	if( bulk_in.address == 0 || bulk_out.address == 0 ) {
		return false;
	}
    Serial.println( bulk_in.address, HEX );
    Serial.println( bulk_out.address, HEX );

	USB::initDevice( device, 0 ); ///? 0?
	adkDevice = device;

    return true;
}

bool AndroidAccessory::isConnected(void)
{
	USB::poll();

	return connected;
}

int16_t AndroidAccessory::read(void *buff, int16_t len, uint16_t nakLimit)
{
	if( nakLimit <= 1 ) {
		return USB::bulkRead( adkDevice, len, (uint8_t *)buff, 1 );
	}
	return USB::bulkRead( adkDevice, len, (uint8_t *)buff, 0 );
}

int16_t AndroidAccessory::write(void *buff, int16_t len)
{
	len = USB::bulkWrite( adkDevice, len, (uint8_t *)buff );
    return len;
}

