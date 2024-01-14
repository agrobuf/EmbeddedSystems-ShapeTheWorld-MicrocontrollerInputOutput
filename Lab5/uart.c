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
 *
 *
 * The first thing to consider when programming the UART is the baud-rate
 * divisor (BRD), becuase the UARTIBRD and UARTFBRD registers must be written
 * before the UARTLCRH register. Use the *Baud-Rate Generation* equation of page
 * 893 to calculate the BRD. Use the equation on page 912 to calculate the value
 * for the UARTFBRD register. With these two values, the UART configuration is
 * written to the module in the following order:
 * 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register
 *    The following sequence is recommended for making changes to the
 *    UARTCTL register
 *          a. Disable the UART
 *          b. Wait for the end of transmission or reception of the current
 *             character.
 *          c. Flush the transmit FIFO by clearing bit 4 (FEN) in the line
 *             control register (UARTLCRH).
 *          d. Reprogram the control register
 *          e. Enable the UART.
 * 2. Write the integer portion of the BRD to the UARTBRD register
 * 3. Write the fractional portion of the BRD to the UARTFBRD register
 * 4. Write the desired serial parameters to the UARTLCRH register
 * 5. Configure the UART clock source by writing to the UARTCC register
 * 6. Optionally, configure the uDMA channel to enable the DMA in the UARTDMACTL
 *    register (page 583)
 * 7. Enable the UART by setting the UARTEN bit in the UARTCTL register
 *
 * Baud-Rate Generation
 * BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
 * UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
 *
 * System Clock
 * The PLL (Phase-Lock-Loop) will be driven by the MOSC (Main Oscillator). On  
 * Tive Launchpad, the MOSC is connected to an external 16 MHz crystal.
 ******************************************************************************/

// NOTE: Come back to page 219 when finished setting the system clock. There is 
// an important note about checking the EEPROM Done Status before setting the
// system clock

// TODO: RCC register
// Run-Mode Clock Configuration
// Pages: 252
// Configure the system clock and oscillators
#define SYS_CTRL_RCC_R (*((volatile uint32_t*) 0x400FE060)


// TODO: RCC2 register





// Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control
// Pages: 342
// Provides software the capability to enable and disable the UART modules in
// Run mode.
#define SYS_CTRL_RCGCUART_R (*((volatile uint32_t*) 0x400FE618))

// TODO: need to wait after setting clock

// General-Purpose Input/Output Run Mode Clock Gating Control
// Pages: 338
// Provides software the capability to enable and disable GPIO modules in Run
// mode.
#define SYS_CTRL_RCGCGPIO_R (*((volatile uint32_t*) 0x400FE608))

// TODO need to wait after setting clock

// GPIO Alternate Function Select
// Pages: 668, 1344
// The mode control select register. 
#define GPIO_PORTA_AFSEL_R (*((volatile uint32_t*) 0x40058420))

// GPIO 2-mA Drive Select
// Offset: 0x500
// This is the 2-mA drive control register.
#define GPIO_GPIODR2R_R (*((volatile uint32_t*) 0x40058500))

// GPIO Port Control
// Offset 0x52C
// Used in conjunction with the GPIOAFSEL register and selects the specific
// peripheral signal for each GPIO pin when using the alternate function mode.
#define GPIO_PORTA_GPIOCTL_R (*((volatile uint32_t*) 0x4005852C))

// UART Control
// Pages: 915
// The UART control register.
#define UARTCTL0_R (*((volatile uint32_t*) 0x4000C030))

// UART Integer Baud-Rate Divisor
// Pages: 911, 893
// The integer part of the baud-rate divior value.
#define UARTIBRD0_R (*((volatile uint32_t*) 0x4000C024))

// UART Fractional Baud-Rate Divisor
// Pages: 912, 893
// The fractional part of the baud-rate divisor value
#define UARTFBRD0_R (*((volatile uint32_t*) 0x4000C028))

// UART Line Control
// Pages: 913, 893
// Serial parameters such as data length, parity and stop bit selection are 
// implemented in this register.
#define UARTLCRH0_R (*((volatile uint32_t*) 0x4000C02C))

// GPIO Digital Enable
// Pages: 679
// Offset: 0x51C
// Enable a pin to act as a digital input or output
// #define GPIO_PORTA_GPIODEN_R (*((volatile uint32_t*) 0x4005851C))


// UART0 base: 0x4000.C000
// The data register.
// 11 OE: Overrun Error
// 10 BE: Break Error
//  9 PE: Parity Error
//  8 FE: Frame Error
// [7:0] DATA: Data transmitted or received
// Page 903
// #define UART_DR

void init_uart(void) {
    // Enable UART module 0 Run Mode Clock Gating Control
    SYS_CTRL_RCGCUART_R = 0x01;

    // Enable clock for Port A
    SYS_CTRL_RCGCGPIO_R = 0x01;
    
    // Set PA0 and PA1 to use alternate function
    GPIO_PORTA_AFSEL_R |= 0x03;

    // Set PA0 and PA1 to use alternate function UART
    GPIO_PORTA_GPIOCTL_R = 0x03;

    // Set PA0 and PA1 to have a 2-mA drive
    GPIO_GPIODR2R_R = 0x03;

    // Disable UART0 before making any changes
    UARTCTL0_R |= 0x01;

    // Set UART0 integer-part baud rate value
    // BRDI = integer(UARTSysClk / (ClkDiv * Baud Rate))
    // UARTSysClk = System Clock = 
    // ClkDiv = 16 because HSE bit is UARTCTL is clear
    // Baud Rate = 115,200
    UARTIBRD0_R = NULL;

    // Set UART0 fractional-part baud rate value
    UARTFBRD0_R = NULL;

    // Set UART0 Stick Parity Select, Word Length, Enable FIFO,
    // Two Stop Bits Select, Even Parity Select, Parity Enable, Send Break,
    // TODO: Update comment to describe what is being enabled/disabled/selected
    // once you know what the hell is going on and what any of this means.
    UARTLCRH0_R = NULL;

    // Set PA0 and PA1 to be digitally enabled
    // GPIO_PORTA_GPIODEN_R = 0x03;
}
 
