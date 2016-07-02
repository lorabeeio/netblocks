/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    ( C )2014 Semtech

Description: -

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainers: www.netblocks.eu
SX1272 LoRa RF module : http://www.netblocks.eu/xrange-sx1272-lora-datasheet/

*/
#include "sx1272-hal.h"

const RadioRegisters_t XRange::RadioRegsInit[] = 
{                                                 
    { MODEM_FSK , REG_LNA                , 0x23 },
    { MODEM_FSK , REG_RXCONFIG           , 0x1E },
    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },
    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },
    { MODEM_FSK , REG_OSC                , 0x07 },
    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },
    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },
    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },
    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },
    { MODEM_FSK , REG_PACKETCONFIG1      , 0xD8 },
    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },
    { MODEM_FSK , REG_IMAGECAL           , 0x02 },
    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },
    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },
    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },  
        
};


XRange::XRange( void ( *txDone )( ), void ( *txTimeout ) ( ), void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ), 
                            void ( *rxTimeout ) ( ), void ( *rxError ) ( ), void ( *fhssChangeChannel ) ( uint8_t channelIndex ), void ( *cadDone ) ( bool ChannelActivityDetected ),
                            PinName mosi, PinName miso, PinName sclk, PinName nss, PinName reset,
                            PinName dio0, PinName dio1, PinName dio2, PinName dio3, PinName dio4, PinName dio5,
                            PinName antSwitch )
                            : SX1272( txDone, txTimeout, rxDone, rxTimeout, rxError, fhssChangeChannel, cadDone, mosi, miso, sclk, nss, reset, dio0, dio1, dio2, dio3, dio4, dio5),
                            antSwitch( antSwitch )
{
    Reset( );
       
    IoInit( );
    
    SetOpMode( RF_OPMODE_SLEEP );
    
    IoIrqInit( dioIrq );
    
    RadioRegistersInit( );

    SetModem( MODEM_FSK );

    this->settings.State = IDLE ;
}

XRange::XRange( void ( *txDone )( ), void ( *txTimeout ) ( ), void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ), 
                            void ( *rxTimeout ) ( ), void ( *rxError ) ( ), void ( *fhssChangeChannel ) ( uint8_t channelIndex ), void ( *cadDone ) ( bool ChannelActivityDetected ) ) 
                        :   SX1272( txDone, txTimeout, rxDone, rxTimeout, rxError, fhssChangeChannel, cadDone, 
                                                        PB_15 /*MOSI*/, PB_14 /*MISO*/, PB_13 /*SCLK*/, PB_12 /*NSS*/, PB_2 /*RESET*/,
                                                        PA_10 /*DIO0*/, PA_9 /*DIO1*/, PC_13 /*DIO2*/, PB_0 /*DIO3*/, PB_1 /*DIO4*/,PB_10 /*DIO5*/ ),
                            antSwitch( PB_11 )                                                      
{
    Reset( );
       
    IoInit( );
    
    SetOpMode( RF_OPMODE_SLEEP );
    IoIrqInit( dioIrq );
    
    RadioRegistersInit( );

    SetModem( MODEM_FSK );

    this->settings.State = IDLE ;
}


void XRange::IoInit( void )
{
    AntSwInit( );
    SpiInit( );
}

void XRange::RadioRegistersInit( ){
    uint8_t i = 0;
    for( i = 0; i < sizeof( RadioRegsInit ) / sizeof( RadioRegisters_t ); i++ )
    {
        SetModem( RadioRegsInit[i].Modem );
        Write( RadioRegsInit[i].Addr, RadioRegsInit[i].Value );
    }    
}

void XRange::SpiInit( void )
{
    nss = 1;    
    spi.format( 8,0 );   
    uint32_t frequencyToSet = 8000000;
    spi.frequency( frequencyToSet );
    wait(0.1); 
}

void XRange::IoIrqInit( DioIrqHandler *irqHandlers )
{
    dio0.mode(PullDown);
    dio1.mode(PullDown);   
    dio2.mode(PullDown);
    dio3.mode(PullDown); 
    dio4.mode(PullDown); 
    dio0.rise( this, static_cast< TriggerXRange > ( irqHandlers[0] ) );
    dio1.rise( this, static_cast< TriggerXRange > ( irqHandlers[1] ) );
    dio2.rise( this, static_cast< TriggerXRange > ( irqHandlers[2] ) );
    dio3.rise( this, static_cast< TriggerXRange > ( irqHandlers[3] ) );
    dio4.rise( this, static_cast< TriggerXRange > ( irqHandlers[4] ) );
}

void XRange::IoDeInit( void )
{
    //nothing
}

uint8_t XRange::GetPaSelect( uint32_t channel )
{
   return RF_PACONFIG_PASELECT_PABOOST;
}

void XRange::SetAntSwLowPower( bool status )
{
    if( isRadioActive != status )
    {
        isRadioActive = status;
    
        if( status == false )
        {
            AntSwInit( );
        }
        else
        {
            AntSwDeInit( );
        }
    }
}

void XRange::AntSwInit( void )
{
    antSwitch = 0;
}

void XRange::AntSwDeInit( void )
{
    antSwitch = 0;
}

void XRange::SetAntSw( uint8_t rxTx )
{
    if( this->rxTx == rxTx )
    {
        //no need to go further
        return;
    }

    this->rxTx = rxTx;

    if( rxTx != 0 )
    {
        antSwitch = 1;
    }
    else
    {
        antSwitch = 0;
    }
}

bool XRange::CheckRfFrequency( uint32_t frequency )
{
    //TODO: Implement check, currently all frequencies are supported
    return true;
}


void XRange::Reset( void )
{
    reset.output();
    reset = 1;
    wait_ms( 1 );
    reset.input();
    wait_ms( 6 );
}
    
void XRange::Write( uint8_t addr, uint8_t data )
{
    Write( addr, &data, 1 );
}

uint8_t XRange::Read( uint8_t addr )
{
    uint8_t data;
    Read( addr, &data, 1 );
    return data;
}

void XRange::Write( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    nss = 0;
    spi.write( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        spi.write( buffer[i] );
    }
    nss = 1;
}

void XRange::Read( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    nss = 0;
    spi.write( addr & 0x7F );
    for( i = 0; i < size; i++ )
    {
        buffer[i] = spi.write( 0 );
    }
    nss = 1;
}

void XRange::WriteFifo( uint8_t *buffer, uint8_t size )
{
    Write( 0, buffer, size );
}

void XRange::ReadFifo( uint8_t *buffer, uint8_t size )
{
    Read( 0, buffer, size );
}
