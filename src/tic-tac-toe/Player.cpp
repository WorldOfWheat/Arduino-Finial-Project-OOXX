#include <Arduino.h>

class Player {
private:
	byte type;
public:
	Player(byte type) {
		this->type = type;
	};
	byte getType() {
		return type;
	}
	void next() {
		if (type == 'O') {
			type = 'X';
		} else {
			type = 'O';
		}
	}
};