#ifndef TIC_TAC_TOE_ROBOT
#define TIC_TAC_TOE_ROBOT
#include <Arduino.h>
#include "./tic-tac-toe/GameJudge.cpp"
#include "./tic-tac-toe/Board.cpp"
#include "./tic-tac-toe/Player.cpp"
#include "./tic-tac-toe/Coordinate.cpp"
#include "./tic-tac-toe/Strategy.cpp"
#endif

class Robot {
private:
	Player robot_player;
	IBoard* board;
	byte difficulty;

	bool decide_strategy() {
		byte random_number = random(1, 4);
		return  difficulty >= random_number;
	}

public:
	Robot(Player robot_player, IBoard* board, byte difficulty) : 
		robot_player(robot_player), 
		board(board), 
		difficulty(difficulty)
		{ }
	void play() {
		Coordinate coordinate;
		if (decide_strategy()) {
			Strategy* strategy = new MinimaxStrategy(robot_player, board);
			coordinate = strategy->get_best_move();
		}
		else {
			Strategy* strategy = new RandomStrategy(robot_player, board);
			coordinate = strategy->get_best_move();
		}
		(*board).draw(coordinate, robot_player);
	}
};