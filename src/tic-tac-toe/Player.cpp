#ifndef PLAYER_H
#define PLAYER_H
	#include <Arduino.h>
	#include "./Coordinate.cpp"
#endif

enum class PlayerType {
	O = 'O',
	X = 'X'
};

class Player {
private:
	PlayerType type;
	IBoard* board;
public:
	Player(PlayerType type, IBoard* board) : type(type), board(board) {
	};

	PlayerType getType() {
		return type;
	}

	void play(Coordinate coordinate) {
		board->draw(coordinate, *this);
	}
};