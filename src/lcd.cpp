#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LCD_Block {
public:
	byte x, y;
	char character;
	LCD_Block(byte x, byte y, char character) : x(x), y(y), character(character) {}
};

class LCD_Controller {
private:
	LiquidCrystal_I2C* lcd;
public:
	LCD_Controller(byte address, byte cols = 16, byte rows = 2) {
		lcd = new LiquidCrystal_I2C(address, 16, 2);
		lcd->init();
		lcd->backlight();
	}

	void print(LCD_Block block) {
		lcd->setCursor(block.x, block.y);
		lcd->print(block.character);
	}
};