#include <stdint.h>

/*******************************************************************************
 * PA0 and PA1 are UART receive and transmit pins connected to the in-cirucit
 * debugger.
 *
 * Port A AHB base address: 0x4005.8000
 * System Control base address: 0x400F.E000
 *
 * To enable and initialize the UART, the following steps are necessary
 * (page 899)
 * 1. Enable UART module using the RCGCUART register (page 342)
 * 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register
 *    (page 1344)
 * 3. Set the GPIO AFSEL bits for the appropriate pins (page 1337)
 * 4. Configure the GPIO current level and/or slew rate as specified for the
 *    mode selected (pages 670 and 678)
 * 5. Configure the PMCn fields in the GPIOCTL register to assign the UART
 *    signals to the appropriate pin (pages 685 and 1344)
 ******************************************************************************/

// Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control
// Pages: 342
// Offset: 0x618
// Provides software the capability to enable and disable the UART modules in
// Run mode.
#define SYS_CTRL_RCGCUART_R (*((volatile uint32_t*) 0x400FE618))

// General-Purpose Input/Output Run Mode Clock Gating Control
// Pages: 338
// Offset: 0x608
// Provides software the capability to enable and disable GPIO modules in Run
// mode.
#define SYS_CTRL_RCGCGPIO_R (*((volatile uint32_t*) 0x400FE608))

// GPIO Alternate Function Select
// Pages: 668, 1344
// Offset 0x420
// The mode control select register. 
#define GPIO_PORTA_AFSEL_R (*((volatile uint32_t*) 0x40058000))

// GPIO Port Control
// Pages: 685, 1344
// Offset 0x52C
// Used in conjunction with the GPIOAFSEL register and selects the specific
// peripheral signal for each GPIO pin when using the alternate function mode.
#define GPIO_PORTA_GPIOCTL_R (*((volatile uint32_t*) 0x4005852C))

// GPIO Digital Enable
// Pages: 679
// Offset: 0x51C
// Enable a pin to act as a digital input or output
#define GPIO_PORTA_GPIODEN_R (*((volatile uint32_t*) 0x4005851C))


// UART0 base: 0x4000.C000
// The data register.
// 11 OE: Overrun Error
// 10 BE: Break Error
//  9 PE: Parity Error
//  8 FE: Frame Error
// [7:0] DATA: Data transmitted or received
// Page 903
#define UART_DR

// #define GPIO_PORTA_DATA_R
// #define GPIO_PORTA_DIR_R
// #define GPIO_PORTA_GPIOPUR_R
// #define GPIO_PORTA_GPIODEN_R
// #define GPIO_PORTA_GPIOLOCK_R
// #define GPIO_PORTA_GPIOCTRL_R
// #define GPIO_PORTA_RCGC2_R
// #define GPIO_PORTA_GPIOHBCRL_R
// #define PR_GPIO_R

void init_uart(void) {
    // Enable UART module 0 Run Mode Clock Gating Control
    SYS_CTRL_RCGCUART_R = 0x01;

    // Enable Input/Output for Port A
    SYS_CTRL_RCGCGPIO_R = 0x01;
    
    // Set PA0 and PA1 to use alternate function
    GPIO_PORTA_AFSEL_R |= 0x03;

    // Set PA0 and PA1 to use alternate function UART
    GPIO_PORTA_GPIOCTL_R = 0x03;

    // Set PA0 and PA1 to be digitally enabled
    GPIO_PORTA_GPIODEN_R = 0x03;
}
 
