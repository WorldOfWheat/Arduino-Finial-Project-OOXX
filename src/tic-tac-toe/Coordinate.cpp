#include <Arduino.h>

class Coordinate {
public:
	byte x;
	byte y;
	Coordinate(byte x = -1, byte y = -1): x(x), y(y) {}
};