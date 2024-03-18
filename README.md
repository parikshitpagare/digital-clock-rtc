
![Banner](https://github.com/parikshitpagare/digital-clock-rtc/assets/80714882/52aac55b-3805-4f54-a024-a568591d1c7f)

<p align="center">
   <img src="https://img.shields.io/badge/F401-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white" >
   <img src="https://img.shields.io/badge/-CORTEX M4-0091BD?style=for-the-badge&logo=Arm&logoColor=white">
   <img src="https://img.shields.io/badge/LICENSE-MIT-green?style=for-the-badge" >
</p>

<p align="center">
A digital clock and calendar with alarm functionality developed on STM32F401 (ARM Cortex-M4) microcontroller using DS1307 RTC for time keeping. It includes a LCD based user interface to set time, date and alarm using interrupt based multiple push buttons.
</p>

<br>

# Features

- 4 Push buttons to configure and control the system.
- Updated clock and calendar using DS1307 RTC and timer interrupt.
- User configurable clock modes to switch between 12h and 24h with instant time conversion.
- Clock, Calendar and Alarm setting in both clock modes.
- Alarm indicator using Led and Buzzer.
- Automated as well as button based manual control to stop alarm.
- LCD back light control using button.

# Hardware

## Requirements

- STM32 F401 Nucleo Microcontroller
- DS1307 RTC Module
- Character LCD 16x2
- Potentiometer 10K
- Push Buttons
- Led
- Buzzer

## Schematic

<p align="center">
	<img src="Docs/schematic_digital_clock_rtc.png" width="80%" height="80%">
</p>
