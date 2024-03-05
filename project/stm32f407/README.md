### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. HTU31D

#### 3.1 Command Instruction

1. Show htu31d chip and driver information.

   ```shell
   htu31d (-i | --information)
   ```

2. Show htu31d help.

   ```shell
   htu31d (-h | --help)
   ```

3. Show htu31d pin connections of the current board.

   ```shell
   htu31d (-p | --port)
   ```

4. Run htu31d register test.

   ```shell
   htu31d (-t reg | --test=reg) [--addr=<0 | 1>]
   ```

5. Run htu31d read test, num is test times.

   ```shell
   htu31d (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
   ```

6. Run htu31d read function, num is read times.

   ```shell
   htu31d (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
htu31d -i

htu31d: chip is TE HTU31D.
htu31d: manufacturer is TE.
htu31d: interface is IIC.
htu31d: driver version is 1.0.
htu31d: min supply voltage is 3.0V.
htu31d: max supply voltage is 5.5V.
htu31d: max current is 0.41mA.
htu31d: max temperature is 125.0C.
htu31d: min temperature is -40.0C.
```

```shell
htu31d -p

htu31d: SCL connected to GPIOB PIN8.
htu31d: SDA connected to GPIOB PIN9.
```

```shell
htu31d -t reg --addr=0

htu31d: chip is TE HTU31D.
htu31d: manufacturer is TE.
htu31d: interface is IIC.
htu31d: driver version is 1.0.
htu31d: min supply voltage is 3.0V.
htu31d: max supply voltage is 5.5V.
htu31d: max current is 0.41mA.
htu31d: max temperature is 125.0C.
htu31d: min temperature is -40.0C.
htu31d: start register test.
htu31d: htu31d_set_addr_pin/htu31d_get_addr_pin test.
htu31d: set addr pin low.
htu31d: check addr pin ok.
htu31d: set addr pin high.
htu31d: check addr pin ok.
htu31d: htu31d_set_humidity_osr/htu31d_get_humidity_osr test.
htu31d: set osr very high.
htu31d: check humidity osr ok.
htu31d: set osr high.
htu31d: check humidity osr ok.
htu31d: set osr middle.
htu31d: check humidity osr ok.
htu31d: set osr low.
htu31d: check humidity osr ok.
htu31d: htu31d_set_temperature_osr/htu31d_get_temperature_osr test.
htu31d: set osr very high.
htu31d: check temperature osr ok.
htu31d: set osr high.
htu31d: check temperature osr ok.
htu31d: set osr middle.
htu31d: check temperature osr ok.
htu31d: set osr low.
htu31d: check temperature osr ok.
htu31d: htu31d_set_heater_on test.
htu31d: heater on.
htu31d: heater on ok.
htu31d: htu31d_set_heater_off test.
htu31d: heater off.
htu31d: heater off ok.
htu31d: htu31d_get_serial_number test.
htu31d: serial number is 0x06 0x48 0x7E.
htu31d: htu31d_get_diagnostic test.
htu31d: diagnostic is 0x00.
htu31d: htu31d_soft_reset test.
htu31d: finish register test.
```

```shell
htu31d -t read --addr=0 --times=3

htu31d: chip is TE HTU31D.
htu31d: manufacturer is TE.
htu31d: interface is IIC.
htu31d: driver version is 1.0.
htu31d: min supply voltage is 3.0V.
htu31d: max supply voltage is 5.5V.
htu31d: max current is 0.41mA.
htu31d: max temperature is 125.0C.
htu31d: min temperature is -40.0C.
htu31d: start read test.
htu31d: set humidity osr very high.
htu31d: set temperature osr very high.
htu31d: temperature is 27.10C.
htu31d: humidity is 27.32%.
htu31d: temperature is 27.12C.
htu31d: humidity is 26.64%.
htu31d: temperature is 27.11C.
htu31d: humidity is 27.68%.
htu31d: set humidity osr low.
htu31d: set temperature osr low.
htu31d: humidity is 27.06%.
htu31d: humidity is 26.83%.
htu31d: humidity is 26.47%.
htu31d: finish read test.
```

```shell
htu31d -e read --addr=0 --times=3

htu31d: 1/3.
htu31d: temperature is 27.03C.
htu31d: humidity is 27.12%.
htu31d: 2/3.
htu31d: temperature is 27.05C.
htu31d: humidity is 27.15%.
htu31d: 3/3.
htu31d: temperature is 27.05C.
htu31d: humidity is 26.78%.
```

```shell
htu31d -h

Usage:
  htu31d (-i | --information)
  htu31d (-h | --help)
  htu31d (-p | --port)
  htu31d (-t reg | --test=reg) [--addr=<0 | 1>]
  htu31d (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
  htu31d (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]

Options:
  --addr=<0 | 1>                 Set the chip address.([default: 0]).
  -e <read>, --example=<read>    Run the driver example.
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>
                                 Run the driver test.
      --times=<num>              Set the running times.([default: 3])
```

