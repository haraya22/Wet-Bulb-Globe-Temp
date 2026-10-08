# Navy Temperature Flags Reader (WBGT System)

An embedded safety monitoring system built on the **CY8CKIT-059 PSoC 5LP** development kit that calculates Wet Bulb Globe Temperature (WBGT) conditions and visually indicates physical activity risk levels based on official U.S. Navy flag standards.

---

## Overview

The Wet-Bulb Globe Temperature (WBGT) is a measure of heat stress in direct sunlight, taking into account temperature, humidity, wind speed, solar angle, and cloud cover. This project uses onboard sensors paired with a PSoC 5LP microcontroller to evaluate real-time environmental conditions and drive LED flag indicators to safeguard military personnel and athletes during physical training.

---

## Flag & LED Logic

| Flag Color | LED Output | Temperature Range (°F) | Risk Level & Navy Guidelines |
| :--- | :--- | :--- | :--- |
| **White** | All LEDs OFF | $< 80.0^\circ\text{F}$ | **Low Risk:** Minimal restrictions; encourage hydration. |
| **Green** | Green LED ON | $80.0 - 84.9^\circ\text{F}$ | **Moderate Risk:** Exercise caution and supervision for heavy exercise or unacclimated personnel. |
| **Yellow** | Green + Red LED ON + FLASHING | $85.0 - 87.9^\circ\text{F}$ | **High Risk:** Curtail strenuous exercise and outdoor activities for new or unacclimated personnel. |
| **Red** | Red LED ON | $88.0 - 89.9^\circ\text{F}$ | **Very High Risk:** Suspend strenuous exercise for personnel with $<12$ weeks of hot-weather training. |
| **Black** | All LEDs ON + FLASHING| $\ge 90.0^\circ\text{F}$ | **Extreme Risk:** All non-essential physical training and strenuous outdoor activities halted. |

---

## Hardware & Software Requirements

### Hardware
* **Microcontroller:** Cypress/Infineon CY8CKIT-050 PSoC 5LP Development Kit
* **Sensors / Input:** Potentiometer (used to emulate temperature / WBGT analog voltage levels)
* **Indicators:** Red LED, Green LED, LED3, and LED4
* **Breadboard & Jumpers**

### Software & IDE
* **PSoC Creator** (v4.4 or similar)
* **Language:** C / PSoC Schematic Creator

---

## Hardware Setup & Pin Mapping

| Peripheral / Component | PSoC 5LP Pin | Description |
| :--- | :--- | :--- |
| Green LED | P3[7] | Output for Green Flag signal |
| Red LED | P12[4] | Output for Red Flag signal |
| LED3 | P6[2] | Output for on board LED for Black Case (ALL LEDS) |
| LED4 | P6[3] | Output for on board LED for Black Case (ALL LEDS) |
| LCD Display | P2[6:0] | Prompts user to start + Shows temperature reading + Flag color section |
| Temp Sensor Data | P6[5] | ADC or Digital input pin for temperature data |

---
