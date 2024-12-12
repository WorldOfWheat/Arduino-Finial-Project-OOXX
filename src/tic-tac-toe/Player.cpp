#ifndef PLAYER_H
#define PLAYER_H
#include "./Coordinate.cpp"

enum class PlayerType {
	O = 'O',
	X = 'X'
};

class Player {
private:
	PlayerType type;
public:
	Player(PlayerType type) : 
		type(type)
		{ };
	
	bool operator == (const Player& compare_player) {
		return type == compare_player.type;
	}

	PlayerType getType() {
		Serial.println("getType");
		Serial.println((char) type);
		return type;
	}
};

#endif
