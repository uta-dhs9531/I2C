//I2C Expander LAB 2
//Darshil Shah

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -


// Hardware configuration:
//   SDL on PB3
//   SCLK on PB2
//   INT connected to PE1




#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "uart0.h"
#include "gpio.h"
#include "clock.h"
#include "nvic.h"
#include "wait.h"
#include "i2c0.h"

#define IO_EXPANDER_GREEN_LED_MASK   (0x01)
#define IO_EXPANDER_RED_LED_MASK     (0x02)
#define IO_EXPANDER_PUSH_BUTTON_MASK (0x04)
#define ADDRESSI2C                   (0x20)

#define OPCODER  0x47
#define OPCODEW  0x46
#define IODIR    0x00
#define IPOL     0x01
#define GPINTEN  0x02
#define DEFVAL   0x03
#define INTCON   0x04
#define IOCON    0x05
#define GPPU     0x06
#define INTF     0x07
#define INTCAP   0x08
#define GPIO     0x09
#define OLAT     0x0A
#define DEFAULT  0x00

// PortB masks
#define SDA_MASK 8
#define SCL_MASK 4

// Pins
#define I2C0SCL PORTB,2
#define I2C0SDA PORTB,3

#define GPIOE_VECTOR_INTERRUPT 20

uint8_t INTCAPregValue;




void GPIOE_isr()
{
    writeI2c0Register(ADDRESSI2C, GPIO, IO_EXPANDER_GREEN_LED_MASK);
    waitMicrosecond(1000000);
    writeI2c0Register(ADDRESSI2C, GPIO, IO_EXPANDER_RED_LED_MASK);
    clearPinInterrupt(PORTE, 3);
    INTCAPregValue= readI2c0Register(ADDRESSI2C, INTCAP);

}

/**
 * main.c
 */

int main(void)
{
    initSystemClockTo40Mhz();
    initI2c0();

    // Enable clocks port E1 as an Input and enable Interrupt on Tiva

    enablePort(PORTE);
    _delay_cycles(3);
    selectPinDigitalInput(PORTE, 1);
    enablePinPullup(PORTE, 1);

    enableNvicInterrupt(GPIOE_VECTOR_INTERRUPT);
    disablePinInterrupt(PORTE, 1);
    selectPinInterruptLowLevel(PORTE,1);
    enablePinInterrupt(PORTE, 1);
    clearPinInterrupt(PORTE, 1);

    //Setting I2C Expander config
    writeI2c0Register(ADDRESSI2C, GPINTEN, DEFAULT);
    writeI2c0Register(ADDRESSI2C, IODIR, IO_EXPANDER_PUSH_BUTTON_MASK);
    writeI2c0Register(ADDRESSI2C, DEFVAL, IO_EXPANDER_PUSH_BUTTON_MASK);
    writeI2c0Register(ADDRESSI2C, INTCON, IO_EXPANDER_PUSH_BUTTON_MASK);
    writeI2c0Register(ADDRESSI2C, GPINTEN, IO_EXPANDER_PUSH_BUTTON_MASK);
    writeI2c0Register(ADDRESSI2C, GPIO, IO_EXPANDER_RED_LED_MASK);


    while(true);

}
