/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2015 Semtech

Description: End device comissioning parameters

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#ifndef __LORA_COMISSIONING_H__
#define __LORA_COMISSIONING_H__

/*!
 * When set to 1 the application uses the Over-the-Air activation procedure
 * When set to 0 the application uses the Personalization activation procedure
 */
#define OVER_THE_AIR_ACTIVATION                     false

/*!
 * Indicates if the end-device is to be connected to a private or public network
 */
#define LORAWAN_PUBLIC_NETWORK                      true

#if( OVER_THE_AIR_ACTIVATION != 0 )

/*!
 * IEEE Organizationally Unique Identifier ( OUI ) (big endian)
 */
#define IEEE_OUI                                    0x11, 0x22, 0x33

/*!
 * Mote device IEEE EUI (big endian)
 */
#define LORAWAN_DEVICE_EUI                          { IEEE_OUI, 0x44, 0x55, 0x66, 0x77, 0x88 }

/*!
 * Application IEEE EUI (big endian)
 */
#define LORAWAN_APPLICATION_EUI                     { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

/*!
 * AES encryption/decryption cipher application key
 */
#define LORAWAN_APPLICATION_KEY                     { 0xF8,0x11,0xD8,0x1E,0x14,0xE5,0x11,0xD9,0xF7,0x7A,0x7D,0xCF,0x8E,0xC6,0x07,0x77 }

    //{ 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }

#else

/*!
 * Current network ID
 */
#define LORAWAN_NETWORK_ID                          ( uint32_t )0

/*!
 * Device address on the network (big endian)
 */
#define LORAWAN_DEVICE_ADDRESS                      ( uint32_t )0x12345678

/*!
 * AES encryption/decryption cipher network session key
 */
#define LORAWAN_NWKSKEY {0x39,0xDE,0x3B,0xC9, 0x42,0x2D,0x6D,0x27,0x6E,0x56,0xF1,0xD1,0x3A,0xF4,0xBA,0x1D}
//OTAA: {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }


/*!
 * AES encryption/decryption cipher application session key
 */
#define LORAWAN_APPSKEY {0x21,0x56,0xF7,0x92,0xE8,0xFC,0x8C,0x74,0x4B,0xA4,0x0F,0xCD,0xE5,0xA0,0xE3,0x78}

// OTAA: { 0xF8,0x11,0xD8,0x1E,0x14,0xE5,0x11,0xD9,0xF7,0x7A,0x7D,0xCF,0x8E,0xC6,0x07,0x77 }

// { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C }

#endif

#endif // __LORA_COMISSIONING_H__
