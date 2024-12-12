#include <Arduino.h>	
#include <Wire.h>
#include <unity.h>

void setup() {
	Serial.begin(115200);
	UNITY_BEGIN();
	RUN_TEST(test);
	UNITY_END();
}

void loop() {
}