#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class IDisplay {
public:
	virtual void showWelcome() = 0;
	virtual void showGameOver() = 0;
	virtual void showGameAndScore() = 0;
};

class OLED : public IDisplay {
private:
	Adafruit_SSD1306* display;
	byte width = 128;
	byte height = 64;

public:
	OLED() {
		display = new Adafruit_SSD1306(128, 64, &Wire, -1);
		display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
		display->clearDisplay();
		display->setTextSize(1);
		display->setTextColor(WHITE);
		display->setCursor(0, 0);
	}

	void showWelcome() {
		display->clearDisplay();
		display->setTextSize(1);
		display->setCursor(0, height / 2 - 10);
		display->println("Welcome");
		display->setCursor(0, height / 2 + 10);
		display->println("Press 1 to start");
		display->display();
	}

	void showGameOver() {
		display->clearDisplay();
		display->setTextSize(2);
		display->setCursor(0, 0);
		display->println("Game Over");
		display->display();
	}

	void showGameAndScore() {
		display->clearDisplay();
		display->setTextSize(1);
		display->setCursor(0, 0);
		display->println("Game");
		display->setCursor(0, 10);
		display->println("Score");
		display->display();
	}

};

#endif