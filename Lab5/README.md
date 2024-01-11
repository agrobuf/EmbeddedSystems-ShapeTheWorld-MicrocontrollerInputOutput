# Lab 5

## Goal
Write a program which takes input from a user's keyboard and calculates the area
of a "room" given a length and width. The length and width are to be within the
range of 3 to 20, inclusive. Input from the keyboard is gathered via UART.

## Notes
The UART signals are alternate functions for some GPIO signals and default to be
GPIO signals at reset, except for U0Rx and U0Tx, which default to UART. The
`AFSEL` bit in the **GPIO Alternate Function Select (GPIOAFSEL)** register
should be set to choose the UART function. The number in paraenthese is the
encoding that must be programmed into the PMCn fild in the **GPIO Port Control
(GPIOCTL)** register.

### U0Rx: UART module 0 receive
- PA0 (pin 17)


U1Tx: UART module 0 transmit
- PA1 (pin 18)
