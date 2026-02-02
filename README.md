<b> File details </b>
1. RFID-LED-Vibration-two-outputs.ino is the code file for having 1 output of LED and 2nd output of LED+vibration motor.
2. RFID-Vibration-only-two-input.ino is the code file for having 1 output of single vibration and 2nd output of vibration on loop. <b> (FINAL)</b>

<b> RFID-Triggered Vibration Prototype (Arduino) </b>

This project is a small Arduino prototype that explores tangible interaction using RFID and haptic feedback.
An enclosed box responds differently to two RFID objects by triggering distinct vibration patterns.
The interaction relies entirely on non-visual feedback, using vibration as the primary output.

Input -> Output

RFID keyfob -> single gentle vibration

RFID white card -> repeated vibration on loop

no RFID detected -> no vibration




<b> Hardware used: </b>
- Arduino Uno
- MFRC522 RFID reader
- RFID keyfob
- RFID wite card
- Vibration module
- jumper wires
- Enclosure

<b> Wiring summary: </b>

RFID:

- SDA to D10
- SCK to D13
- MOSI to D11
- MISO to D12
- RST to D9
- 3.3V to 3.3V
- GND to GND

Vibration motor:

- Controlled via digital pin D7
- Common ground shared with Arduino


<b> How to Run </b>

1. Instll the Arduino IDE
2. Install MFRC522 library
3. Connect the RFID UID values in the sketch to match your tags
4. Upload the code to the Arduino Uno
5. Power the circuit and test with each RFID object.
