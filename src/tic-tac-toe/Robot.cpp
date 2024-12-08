#include "./tic-tac-toe/Board.cpp"

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

class Robot {
private:
	Player robot_player;

	BestMoveResult get_best_next_move(Board board, Player current_player, int alpha, int beta) {
		int minimum_value = 1e9;
		int maximum_value = -1e9;
		Coordinate best_coordinate;

		GameResult game_result = board.check_winner();
		if (game_result.isDraw) {
			return 0;
		}
		if (game_result.isWin) {
			if (game_result.winner.getType() == robot_player.getType()) {
				return 1;
			}
			return -1;
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Coordinate coordinate = Coordinate(i, j);
				if (board.get_cell(coordinate).isFilled()) {
					continue;
				}
				board.draw(coordinate, current_player);
				current_player.next();

				BestMoveResult next_move = get_best_next_move(board, current_player, alpha, beta);

				board.erase(coordinate);
				current_player.next();

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

	bool is_board_empty(Board board) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (board.get_cell(Coordinate(i, j)).isFilled()) {
					return false;
				}
			}
		}
		return true;
	} 

public:
	Robot(Player player) : robot_player(player) {};
	Coordinate get_next_move(Board board) {
		if (is_board_empty(board)) {
			return Coordinate(1, 1);
		}
		return get_best_next_move(board, robot_player, -1e9, 1e9).coordinate;
	}
};