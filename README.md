# UXN on STM32 ARM

## Getting Started with Development

I used [this guide](https://rhye.org/post/stm32-with-opencm3-0-compiling-and-uploading/) to get started.

### Building

    $ make

### Programming

    $ make flash

### Debugging

Connect to target via OpenOCD

    $ openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

Open another terminal window and start a GDB session. This series of commands loads the target symbols, connects to the
running OpenOCD session, and resets remote execution halting.

    $ arm-none-eabi-gdb build/blinky.elf
    > target remote :3333
    > monitor reset halt

You are now ready to debug using [GDB commands](https://users.ece.utexas.edu/~adnan/gdb-refcard.pdf)

## Useful Docs

Some important data sheets are included in the repo, but here is some further reading...

[libopenCM3 examples for
F4-Disco](https://github.com/libopencm3/libopencm3-examples/tree/master/examples/stm32/f4/stm32f4-discovery)

[Example Makefile for libopenCM3](https://github.com/libopencm3/libopencm3-examples/blob/master/examples/rules.mk)

[RM0090 STM32 Ref Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
