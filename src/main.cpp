#include <Arduino.h>
#include "./lcd.cpp"

void setup() {
	LCD_Controller lcd(0x20);
	lcd.print(LCD_Block(0, 0, 'H'));
}

void loop() {

}