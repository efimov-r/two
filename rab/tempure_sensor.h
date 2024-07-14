#ifndef TEMPURE_SENSOR_H_

#define TEMPURE_SENSOR_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h> // Used for memcpy.

typedef struct
{
    unsigned char id[8];    //!< The 64 bit identifier.
} OWI_device;


#define SEARCH_SUCCESSFUL     0x00
#define SEARCH_CRC_ERROR      0x01
#define SEARCH_ERROR          0xff
#define AT_FIRST              0xff

#define __save_interrupt() SREG
#define __restore_interrupt(var) SREG = (var)
#define __disable_interrupt() cli()
#define __enable_interrupt() sei()
#define __delay_cycles(var) _delay_us((unsigned int)(var)/(F_CPU/1000000))


#define OWI_PULL_BUS_LOW(bitMask) \
            OWI_DDR |= bitMask; \
            OWI_PORT &= ~bitMask;
#ifdef OWI_USE_INTERNAL_PULLUP
#define OWI_RELEASE_BUS(bitMask) \
            OWI_DDR &= ~bitMask; \
            OWI_PORT |= bitMask; 
#else
#define OWI_RELEASE_BUS(bitMask) \
            OWI_DDR &= ~bitMask; \
            OWI_PORT &= ~bitMask;
#endif

/****************************************************************************
 ROM commands
****************************************************************************/
#define     OWI_ROM_READ    0x33    //!< READ ROM command code.
#define     OWI_ROM_SKIP    0xcc    //!< SKIP ROM command code.
#define     OWI_ROM_MATCH   0x55    //!< MATCH ROM command code.
#define     OWI_ROM_SEARCH  0xf0    //!< SEARCH ROM command code.


/****************************************************************************
 Return codes
****************************************************************************/
#define     OWI_ROM_SEARCH_FINISHED     0x00    //!< Search finished return code.
#define     OWI_ROM_SEARCH_FAILED       0xff    //!< Search failed return code.


/****************************************************************************
 UART patterns
****************************************************************************/
#define     OWI_UART_WRITE1     0xff    //!< UART Write 1 bit pattern.
#define     OWI_UART_WRITE0     0x00    //!< UART Write 0 bit pattern.
#define     OWI_UART_READ_BIT   0xff    //!< UART Read bit pattern.
#define     OWI_UART_RESET      0xf0    //!< UART Reset bit pattern.

#define OWI_UART_STATCTRL_REG_A     UCSRA
#define OWI_UART_STATCTRL_REG_B     UCSRB
#define OWI_UART_STATCTRL_REG_C     UCSRC
#define OWI_UART_DATA_REGISTER      UDR
#define OWI_UART_BAUD_RATE_REG_L    UBRRL
#define OWI_UART_BAUD_RATE_REG_H    UBRRH

#define OWI_U2X                     U2X
#define OWI_RXEN                    RXEN
#define OWI_TXEN                    TXEN
#define OWI_RXCIE                   RXCIE
#define OWI_UCSZ1                   UCSZ1
#define OWI_UCSZ0                   UCSZ0
#define OWI_UDRIE                   UDRIE
#define OWI_FE                      FE
#define OWI_URSEL                   URSEL
#define OWI_RXC                     RXC

#define OWI_UART_RXC_VECT           USART_RXC_vect
#define OWI_UART_UDRE_VECT          USART_UDRE_vect

#define     OWI_SOFTWARE_DRIVER 
#define     CPU_FREQUENCY   16.000

#define     OWI_PORT        PORTC   //!< 1-Wire PORT Data register.
#define     OWI_PIN         PINC    //!< 1-Wire Input pin register.
#define     OWI_DDR         DDRC    //!< 1-Wire Data direction register.

#define     OWI_UART_2X         1
#define     OWI_UBRR_115200     8
#define     OWI_UBRR_9600       103

#define     OWI_PIN_0       0x01
#define     OWI_PIN_1       0x02
#define     OWI_PIN_2       0x04
#define     OWI_PIN_3       0x08
#define     OWI_PIN_4       0x10
#define     OWI_PIN_5       0x20
#define     OWI_PIN_6       0x40
#define     OWI_PIN_7       0x80
#define     OWI_DELAY_OFFSET_CYCLES    13   //!< Timing delay when pulling bus low and releasing bus.

// Bit timing delays in clock cycles (= us*clock freq in MHz).
#define     OWI_DELAY_A_STD_MODE    ((6   * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_B_STD_MODE    ((64  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_C_STD_MODE    ((60  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_D_STD_MODE    ((10  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_E_STD_MODE    ((9   * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_F_STD_MODE    ((55  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
//#define     OWI_DELAY_G_STD_MODE  ((0   * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_H_STD_MODE    ((480 * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_I_STD_MODE    ((70  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_J_STD_MODE    ((410 * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)


void OWI_WriteBit1(unsigned char pins);
void OWI_WriteBit0(unsigned char pins);

unsigned char OWI_ReadBit(unsigned char pins);
unsigned char OWI_DetectPresence(unsigned char pins);



void OWI_SendByte(unsigned char data, unsigned char pin);
unsigned char OWI_ReceiveByte(unsigned char pin);
void OWI_SkipRom(unsigned char pin);
void OWI_Init(unsigned char pins);


#endif TEMPURE_SENSOR_H_
