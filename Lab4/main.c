#include <stdint.h>

/*******************************************************************************
 * Port F AHB address begins at 0x4005D
 * All offsets from this address correspond to specific register.
 ******************************************************************************/

// 0x3FC = 0b0011.1111.1100
// The offset is a mask indicating what bits to read/write for the data register
// 3FC gives a mask for all the bits. Note that the mask is offset by 2.
// Bits [9:2] are used for the mask.
#define GPIO_PORTF_DATA_R       (*((volatile uint32_t*)0x4005D3FC))

// Sets the pin direction as either input or output
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t*)0x4005D400))

// Alternate function select register
#define GPIO_PORTF_AFSEL_R      (*((volatile uint32_t*)0x4005D420))

// Enable/disable weak pullup register
#define GPIO_PORTF_GPIOPUR_R    (*((volatile uint32_t*)0x4005D510))

// Enable/disable pin as digital input/output
#define GPIO_PORTF_GPIODEN_R    (*((volatile uint32_t*)0x4005D51C))

// Enable/disable write access to the GPIOCR register
#define GPIO_PORTF_GPIOLOCK_R   (*((volatile uint32_t*)0x4005D520))

// The commit register for GPIOAFSEL, GPIOPDR, and GPIODEN registers
// Used here for PF0 which is connected to SW2.
#define GPIO_PORTF_GPIOCR_R     (*((volatile uint32_t*)0x4005D524))

// Controls isloation circuits to the analog side of a undified I/O pad
#define GPIO_PORTF_GPIOAMSEL_R  (*((volatile uint32_t*)0x4005D528))

// Used with the GPIOAFSEL register; selects the peripheral signal for a GPIO
// pin when using the alternate function mode
#define GPIO_PORTF_GPIOPCTRL_R  (*((volatile uint32_t*)0x4005D52C))

// Controls the clock gating logic in normal Run mode.
#define SYSCTRL_RCGC2_R         (*((volatile uint32_t*)0x400FE108))

// Controls whether to use the legacy APB bus or AHB bus
#define SYSCTRL_GPIOHBCRL_R     (*((volatile uint32_t*)0x400FE06C))

// Indicates whether the GPIO modules are ready to be accessed after a change in
// status
#define PR_GPIO_R               (*((volatile uint32_t*)0x400FE000))

void portF_init(void);

int main(void)
{
    uint32_t input_sw1;
    uint32_t input_sw2;
    uint32_t output_led;
    portF_init();

    while(1) {
        input_sw1 = GPIO_PORTF_DATA_R & 0x10; // Switch 1 is on pin 4
        input_sw2 = GPIO_PORTF_DATA_R & 0x01; // Switch 2 is on pin 0
        output_led = 0x00000000; // LED colors: red=pin1, blue=pin2, green=pin3

        /*
        * LED = off if no switch pressed
        * LED = blue if both switches pressed
        * LED = red if SW1 pressed
        * LED = green if SW2 pressed
        *
        * Switches use pullup resistors. If a switch is pressed, the pin will go
        * low
        */
        if (0x00 == input_sw1 && 0x00 == input_sw2) {
            output_led = 0x04; // Enable blue
        } else if (0x00 == input_sw1) {
            output_led = 0x02; // Enable red
        } else if (0x00 == input_sw2) {
            output_led = 0x08; // Enable green
        }

        GPIO_PORTF_DATA_R = output_led;
    }

	return 0;
}

void portF_init(void) {
    // Bit 5 controls GPIOF; Setting GPIOF allows port F to receive a clock
    // and functions.
    SYSCTRL_RCGC2_R |= 0x00000020;

    // Setting the RCGC2 clock requires waiting 3 cycles. Use the PRGPIO
    // register to determine when the port is ready.
    while (0x00 == PR_GPIO_R) {}

    // Enable the AHB bus instead of the legacy APB bus
    SYSCTRL_GPIOHBCRL_R = 0x00000020;

    /*
     * Unlock the commit register and enable PF0 to be used as a GPIO pin for
     * SW2. PF0 is a protected register defaulted for use with JTAG.
     */
    GPIO_PORTF_GPIOLOCK_R = 0x4C4F434B;
    GPIO_PORTF_GPIOCR_R = 0x00000001;

    // Disable analog functionality (note that port F appears to have no analog
    // functionality.
    GPIO_PORTF_GPIOAMSEL_R = 0x00000000;

    // Enable the pins for digital input/output
    GPIO_PORTF_GPIODEN_R = 0x0000001F;

    // Set the pins to function as GPIO
    GPIO_PORTF_AFSEL_R = 0x00000000;

    // Do not set an alternate peripheral function
    GPIO_PORTF_GPIOPCTRL_R = 0x00000000;

    // PF0/PF4 inputs; PF1/PF2/PF3 outputs: 0b___0.1110
    GPIO_PORTF_DIR_R = 0x0000000E;

    // Enable pullup resistors on PF0 and PF4 (input pins)
    GPIO_PORTF_GPIOPUR_R = 0x00000011;
}

