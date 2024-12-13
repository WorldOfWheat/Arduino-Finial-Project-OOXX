#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <Arduino.h>
#include <Wire.h>

class IKeyInput {
public:
	virtual char get_key() = 0;
};

class Keyboard : public IKeyInput {
public:
	Keyboard() {
		Serial.begin(115200);
	}

	char get_key() {
		char key = ' ';
		if (Serial.available() > 0) {
			key = Serial.read();
		}
		return key;
	}
};

class KeyPad : public IKeyInput {
private:
	const byte row_pins[3] = {D0, D3, D4};
	const byte col_pins[3] = {D5, D6, D7};

	const char keys[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};

	char prev_key = '\0';

	char scan_key() {
		char key = '\0';
		for (int i = 0; i < 3; i++) {
			digitalWrite(row_pins[i], LOW);
			for (int j = 0; j < 3; j++) {
				if (digitalRead(col_pins[j]) == LOW) {
					key = keys[i][j];
				}
			}
			digitalWrite(row_pins[i], HIGH);
			delayMicroseconds(100);
		}
		return key;
	}

public:
	KeyPad() {
		for (int i = 0; i < 3; i++) {
			pinMode(row_pins[i], OUTPUT);
			digitalWrite(row_pins[i], HIGH);
			pinMode(col_pins[i], INPUT_PULLUP);
		}
	}

	char get_key() {
		char key = scan_key();
		if (key != '\0' && key != prev_key) {
			prev_key = key;
			return key;
		}
		prev_key = key;
		return '\0';
	}
};

#endif