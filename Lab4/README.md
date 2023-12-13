# Lab4

## Switches
GPIO Port F APB begins at 0x4002.5000
GPIO Port F AHB begins at 0x4005.C000

Register 2: GPIO Direction (GPIODIR)
    - offset 0x400
    - page 660
    - 0 = input
    - 1 = output

Register 10: GPIO Alternate Function Select (GPIOAFSEL)
    - offset 0x420
    - page 668
    - The mode select register. If a bit is clear, the pin is used as a GPIO and  
      is controller by the GPIO registers. Setting a bit in this register
      configures the corresponding GPIO line to be controlled by an associated
      peripheral. The **GPIO Port Control (GPIOCTL) register** is used to select
      one of the possible function. Page 1344 details which functions are muxed
      on each GPIO pin.

Register 15: GPIO Pull-Up Select (GPIOPUR)
    - offset 0x510
    - page 674
    - The pull-up control register. When a bit is set, a  weak pull-up resistor
       on the corresponding GPIO signal is enabled. Setting GPIOPUR
       automatically clears the corresponding bit in the GPIOPDR register.

Register 18: GPIO Digital Enable (GPIODEN)
    - offset 0x51C
    - page 679
    - The **GPIODEN** register is the digital enable register. To use the pin
      as a digital input or output, the corresponding GPIODEN bit must be set.

Register 19: GPIO Lock (GPIOLOCK)
    - offset 0x520
    - page 681
    - The **GPIOLOCK** register enabales write access to the **GPIOCR**
      register. Writing 0x4C4F.434B unlocks the GPIOCR register. Writing any
      other value re-enables the locked state. Reading the register returns the
      lock status.

Register 20: GPIO Commit (GPIOCR)
    - offset 0x524
    - page 682
    - The **GPIOCR** register is the commit register. The value of the GPIOCR
      register determines which bits of the GPIOAFSEL, GPIOPDR, and GPIODEN
      registers are comminted when a wite to these registers is performed. If
      a bit in the GPIOCR register is cleared, the data being written to the
      corresponding bit in the GPIOAFSEL, GPIOUR, GPIODR, or GPIODEN registers
      is commited to the register and reflects the new value. The contents of
      GPIOCR register can only be modified if the status in the GPIOLOCK
      register is unlocked. Writes to the GPIOCR register are ignored if the
      status in the GPIOLOCK register is locked.

Register 21: GPIO Analog Mode Select (GPIOAMSEL)
    - offset 0x528
    - page 684
    - The **GPIOAMSEL** register controls isloation circuits to the analog side
      of a undified I/O pad. Because the GPIOs may be driven by a 5V source and 
      affect operation, analog circuitry requieres isolation from the pins when
      they are not used in their analog function. This register is only valid
      for ports and pins that can be used as ADC AINx inputs. If any pin is to
      be used as an ADC input, the appropriate bit in the GPIOASMSEL must be set
      to disable the analog isolation circuit.

Register 22: GPIO Port Control (GPIOCTL)
    - offset 0x52C
    - page 685
    The **GPIOPCTRL** register is used in conjuction with the GPIOAFSEL register
    and selects the specific peripheral signal for each GPIO pin when useing the
    alternate function mode. The GPIOCTL register selects one out of a set of
    peripheral functions for each GPIO, providing addition flexibility in signal
    definiton.

Register 136: Run Mode Clock Gating Control Register 2 (RCGC2)
    - offset 0x108
    - page 462
    - This register controls the clock gating logic in normal Run mode. Each bit
      controls a clock enable for a given interface, function, or module. If
      set, the module receives a clock and functions. If the module is unlocked
      (not set), reads or writes to the module generate a bus fault. It is the
      responsibility of software to enable the ports necessary for the
      application. **RCGC2** is the clock configuration register for running
      operations, **SCGC2** for sleeo operation, and **DCGC2** for deep-sleep
      operation.
    - NOTE: System Control Register descriptions begin on page 235

Register 9: GPIO HIGH-Performance Bus Control (GPIOHBCRL)
    - offset 0x06C
    - page 256
    - Controls which internal bus is used to access each GPIO port.

NOTE
page 338 for enabling gpio before use
