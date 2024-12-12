#ifndef STRATEGY
#define STRATEGY
#include <Arduino.h>
#include "./Player.cpp"
#include "./Coordinate.cpp"
#include "./Board.cpp"
#include "./GameJudge.cpp"
#endif

class Strategy {
public:
	virtual Coordinate get_best_move() = 0;
};

class BestMoveResult {
public:
	Coordinate coordinate = Coordinate(-1, -1);
	int alpha;
	int beta;
	int value;
	BestMoveResult() {}
	BestMoveResult(int value) {
		this->value = value;
	}
	BestMoveResult(Coordinate coord, int alpha, int beta, int value) : coordinate(coord), alpha(alpha), beta(beta), value(value) {}
};

class MinimaxStrategy : public Strategy {
private:
	IBoard* board;
	GameJudge game_judge;
	Player robot_player;

	Player get_opponent(Player current_player) {
		return current_player.getType() == PlayerType::O ? Player(PlayerType::X) : Player(PlayerType::O);
	}

	BestMoveResult minimax(Player current_player, int alpha, int beta) {
		int minimum_value = 1e9;
		int maximum_value = -1e9;
		Coordinate best_coordinate;

		GameResult game_result = game_judge.judge();
		if (game_result.status == GameStatus::DRAW) {
			return BestMoveResult(0);
		}
		if (game_result.status == GameStatus::WIN) {
			if (game_result.winner.getType() == robot_player.getType()) {
				return BestMoveResult(1);
			}
			return BestMoveResult(-1);
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Coordinate coordinate = Coordinate(i, j);
				if ((*board).get_cell(coordinate).isFilled()) {
					continue;
				}

				(*board).draw(coordinate, current_player);
				Player opponent = get_opponent(current_player);

				BestMoveResult next_move = minimax(opponent, alpha, beta);

				(*board).erase(coordinate);

				if (alpha > beta) {
					break;
				}

				if (current_player.getType() == robot_player.getType()) {
					if (next_move.value > maximum_value) {
						best_coordinate = Coordinate(i, j);
						maximum_value = next_move.value;
						alpha = maximum_value;
					}
				}
				else {
					if (next_move.value < minimum_value) {
						best_coordinate = Coordinate(i, j);
						minimum_value = next_move.value;
						beta = minimum_value;
					}
				}
			}
		}

		return BestMoveResult(
			best_coordinate,
			alpha,
			beta,
			current_player.getType() == robot_player.getType() ? maximum_value : minimum_value
		);
	}
public:
	MinimaxStrategy(Player robot_player, IBoard* board) : 
		robot_player(robot_player), board(board), game_judge(GameJudge(board))
	{}

	Coordinate get_best_move() {
		BestMoveResult best_move = minimax(this->robot_player, -1e9, 1e9);
		return best_move.coordinate;
	}
};

class RandomStrategy : public Strategy {
private:
	Player robot_player;
	IBoard* board;
public:
	RandomStrategy(Player robot_player, IBoard* board) : 
		robot_player(robot_player), board(board)
		{ }
	Coordinate get_best_move() {
		Coordinate coordinate;
		do {
			coordinate = Coordinate(random(3), random(3));
		} while ((*board).get_cell(coordinate).isFilled());
		return coordinate;
	}
};