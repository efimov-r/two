#include "tempure_sensor.h"

void OWI_SendByte(unsigned char data, unsigned char pin)
{
    unsigned char temp;
    unsigned char i;
    
    for (i = 0; i < 8; i++)
    {
        temp = data & 0x01;
        if (temp)
        {
            OWI_WriteBit1(pin);
        }
        else
        {
            OWI_WriteBit0(pin);
        }
        data >>= 1;
    }
}

unsigned char OWI_ReadBit(unsigned char pins)
{
    unsigned char intState;
    unsigned char bitsRead;
    
    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();
    
    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_A_STD_MODE);
    
    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_E_STD_MODE);
    
    // Sample bus and delay.
    bitsRead = OWI_PIN & pins;
    __delay_cycles(OWI_DELAY_F_STD_MODE);
    
    // Restore interrupts.
    __restore_interrupt(intState);
    
    return bitsRead;
}

unsigned char OWI_ReceiveByte(unsigned char pin)
{
    unsigned char data;
    unsigned char i;

    data = 0x00;
    
    for (i = 0; i < 8; i++)
    {
        data >>= 1;
        if (OWI_ReadBit(pin))
        {
            data |= 0x80;
        }
    }
    return data;
}


void OWI_SkipRom(unsigned char pin)
{
    OWI_SendByte(OWI_ROM_SKIP, pin);
}

void OWI_Init(unsigned char pins)
{
    OWI_RELEASE_BUS(pins);
    // The first rising edge can be interpreted by a slave as the end of a
    // Reset pulse. Delay for the required reset recovery time (H) to be 
    // sure that the real reset is interpreted correctly.
    __delay_cycles(OWI_DELAY_H_STD_MODE);
}


void OWI_WriteBit1(unsigned char pins)
{
    unsigned char intState;
    
    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();
    
    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_A_STD_MODE);
    
    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_B_STD_MODE);
    
    // Restore interrupts.
    __restore_interrupt(intState);
}


void OWI_WriteBit0(unsigned char pins)
{
    unsigned char intState;
    
    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();
    
    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_C_STD_MODE);
    
    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_D_STD_MODE);
    
    // Restore interrupts.
    __restore_interrupt(intState);
}

unsigned char OWI_DetectPresence(unsigned char pins)
{
    unsigned char intState;
    unsigned char presenceDetected;
    
    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();
    
    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_H_STD_MODE);
    
    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_I_STD_MODE);
    
    // Sample bus to detect presence signal and delay.
    presenceDetected = ((~OWI_PIN) & pins);
    __delay_cycles(OWI_DELAY_J_STD_MODE);
    
    // Restore interrupts.
    __restore_interrupt(intState);
    
    return presenceDetected;
}
