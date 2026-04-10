# ATmega32 Embedded Systems Project

An embedded C project for the **ATmega32** demonstrating three core peripherals:

- **Button-controlled LEDs** with software debouncing
- **ADC temperature sensing** (LM35) displayed on a 16x2 HD44780 LCD
- **Timer1 CTC interrupt** for non-blocking LED blinking and periodic LCD refresh

---

## Features

| Feature | Detail |
|---|---|
| MCU | ATmega32 @ 8 MHz |
| LED control | 3 user LEDs toggled by push-buttons (debounced) |
| Blink LED | 1 LED toggled every 500 ms via Timer1 ISR |
| Temperature | LM35 on ADC0, refreshed every 1 s, shown to 1 decimal place |
| Display | 16x2 LCD in 4-bit mode |

---

## Hardware Connections

### LEDs — PORTB

| Pin | Function |
|-----|----------|
| PB0 | LED0 (button-controlled) |
| PB1 | LED1 (button-controlled) |
| PB2 | LED2 (button-controlled) |
| PB3 | BLINK\_LED (timer, 1 Hz) |

Connect each LED in series with a 220 Ω resistor to GND.

### Buttons — PORTD (active-low, internal pull-ups)

| Pin | Function |
|-----|----------|
| PD2 | BTN0 — toggles LED0 |
| PD3 | BTN1 — toggles LED1 |
| PD4 | BTN2 — toggles LED2 |

Connect each button between the pin and GND. No external resistors needed.

### LCD (HD44780, 4-bit mode) — PORTC

| LCD Pin | MCU Pin | Note |
|---------|---------|------|
| RS | PC0 | |
| RW | GND | Write-only |
| EN | PC1 | |
| D4 | PC4 | |
| D5 | PC5 | |
| D6 | PC6 | |
| D7 | PC7 | |
| V0 | 10 kΩ pot to GND | Contrast |

### Temperature Sensor (LM35) — PORTA

| LM35 Pin | Connection |
|----------|------------|
| +VS | +5 V |
| Vout | PA0 (ADC0) |
| GND | GND |

---

## LCD Display Layout

```
Row 0:  Temp: 24.3°C
Row 1:  L0:0 L1:1 L2:0
```

---

## Project Structure

```
GIU-2601/
├── src/
│   ├── main.c        # Application entry point and main loop
│   ├── led.h/.c      # LED GPIO driver
│   ├── button.h/.c   # Button driver with 20 ms debounce
│   ├── adc.h/.c      # ADC driver + LM35 temperature conversion
│   ├── timer.h/.c    # Timer1 CTC ISR (500 ms / 1 s ticks)
│   └── lcd.h/.c      # HD44780 LCD driver (4-bit mode)
├── Makefile
└── README.md
```

---

## Building

### Prerequisites

**Ubuntu / Debian / WSL:**
```bash
sudo apt install gcc-avr binutils-avr avr-libc avrdude
```

**macOS:**
```bash
brew tap osx-cross/avr && brew install avr-gcc avrdude
```

### Compile

```bash
make
```

### Flash to MCU (USBasp)

```bash
make flash
```

Override programmer/port as needed:
```bash
make flash PROGRAMMER=avrispmkII PORT=/dev/ttyUSB0
```

### Memory usage

```bash
make size
```

### Clean

```bash
make clean
```

---

## How It Works

### Button-controlled LEDs

`Button_WasPressed()` applies a 20 ms debounce and waits for button release before returning, giving one clean event per press. The LED state is toggled and the LCD status line is updated immediately.

### Timer1 Interrupt

Timer1 runs in CTC mode with OCR1A = 3905 and prescaler = 1024:

```
f = 8 000 000 / (1024 × 3906) ≈ 2 Hz → 500 ms period
```

The ISR sets `g_blink_tick` every 500 ms and `g_temp_tick` every 1000 ms. The main loop reads and clears these flags — no LCD or ADC calls inside the ISR.

### ADC Temperature Reading

The LM35 outputs 10 mV/°C. With AVcc = 5 V and a 10-bit ADC:

```
Temp (°C × 10) = ADC_value × 500 / 1024
```

Example: ADC = 486 → 486 × 500 / 1024 = 237 → **23.7 °C**

---

## Fuse Bits (8 MHz external crystal)

```bash
avrdude -c usbasp -p atmega32 -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m
```

> **Warning:** Wrong fuse settings can brick the MCU. Verify with an AVR fuse calculator first.

---

## License

MIT — free to use for educational purposes.
