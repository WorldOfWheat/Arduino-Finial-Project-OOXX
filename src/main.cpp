#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Player {
private:
	char type;
public:
	Player(char type) {
		if (type != 'O' && type != 'X') {
			throw "Invalid type";
		}
		this->type = type;
	};
	char getType() {
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

class Coordinate {
public:
	int x;
	int y;
	Coordinate(int x = -1, int y = -1): x(x), y(y) {}
};

class Cell {
private:
	// O = 1
	// X = 2
	// Empty = 0
	char type;
public:
	Cell(char type = 0) : type(type) {};
	char getType() {
		return type;
	}
	void setType(char type) {
		if (type != 'O' && type != 'X' && type != 0) {
			throw "Invalid type";
		}
		this->type = type;
	}
	bool isFilled() {
		return type != 0;
	}
};

class GameResult {
public:
	GameResult() : isWin(false), isDraw(false), winner('O') {};
	GameResult(bool isDraw) : isWin(false), isDraw(isDraw), winner('O') {};
	GameResult(bool isWin, Player winner) : isWin(isWin), winner(winner) {};
	bool isWin;
	bool isDraw;
	Player winner;
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

class Board {
private:
	Cell cells[3][3];
	Player current_player = 'O';

	GameResult check_row_winner(char row) {
		if (
			cells[row][0].getType() != 0 &&
			cells[row][0].getType() == cells[row][1].getType() && 
			cells[row][1].getType() == cells[row][2].getType()
		) {
			return GameResult(true, cells[row][0].getType());
		}
		return GameResult(false);
	}

	GameResult check_column_winner(char column) {
		if (
			cells[0][column].getType() != 0 &&
			cells[0][column].getType() == cells[1][column].getType() && 
			cells[1][column].getType() == cells[2][column].getType()
		) {
			return GameResult(true, cells[0][column].getType());
		}
		return GameResult(false);
	}

	GameResult check_slash_winner() {
		if (
			cells[0][0].getType() != 0 &&
			cells[0][0].getType() == cells[1][1].getType() && 
			cells[1][1].getType() == cells[2][2].getType()
		) {
			return GameResult(true, cells[0][0].getType());
		}
		return GameResult(false);
	}

	GameResult check_backslash_winner() {
		if (
			cells[1][1].getType() != 0 &&
			cells[0][2].getType() == cells[1][1].getType() && 
			cells[1][1].getType() == cells[2][0].getType()
		) {
			return GameResult(true, cells[0][2].getType());
		}
		return GameResult(false);
	}

	void print_split() {
		std::cout << "-+-+-" << std::endl;
	}

	void print_row(int row) {
		for (int i = 0; i < 3; i++) {
			if (cells[row][i].getType() == 0) {
				std::cout << " ";
			} else {
				std::cout << cells[row][i].getType();
			}
			if (i < 2) {
				std::cout << "|";
			}
		}
		std::cout << std::endl;
	}

	bool is_board_full() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (cells[i][j].getType() == 0) {
					return false;
				}
			}
		}
		return true;
	}

public:
	Board() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cells[i][j] = Cell();
			}
		}
	}

	GameResult check_winner() {
		for (int i = 0; i < 3; i++) {
			GameResult row_winner = check_row_winner(i);
			GameResult column_winner = check_column_winner(i);
			if (row_winner.isWin) {
				return row_winner;
			}
			if (column_winner.isWin) {
				return column_winner;
			}
		}
		GameResult slash_winner = check_slash_winner();
		GameResult backslash_winner = check_backslash_winner();
		if (slash_winner.isWin) {
			return slash_winner;
		}
		if (backslash_winner.isWin) {
			return backslash_winner;
		}

		if (is_board_full()) {
			return GameResult(true);
		}

		return 0;
	}

	void draw(Coordinate coordinate, Player currentPlayer) {
		int x = coordinate.x;
		int y = coordinate.y;
		if (x < 0 || x > 2 || y < 0 || y > 2) {
			throw "Invalid position";
		}
		if (cells[x][y].getType() != 0) {
			throw "Cell is already filled";
		}
		cells[x][y].setType(currentPlayer.getType());
	}

	void erase(Coordinate coordinate) {
		int x = coordinate.x;
		int y = coordinate.y;
		if (x < 0 || x > 2 || y < 0 || y > 2) {
			throw "Invalid position";
		}
		cells[x][y].setType(0);
	}

	void print() {
		print_split();
		for (int i = 0; i < 3; i++) {
			print_row(i);
			print_split();
		}
	}

	Cell get_cell(Coordinate coordinate) {
		return cells[coordinate.x][coordinate.y];
	}
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


int main() {
	Board board;

	Player player1 = Player('O');
	Player player2 = Player('X');

	Robot robot = Robot(player1);
	Robot robot2 = Robot(player2);

	board.draw(robot.get_next_move(board), player1);
	board.draw(robot2.get_next_move(board), player2);
	board.draw(robot.get_next_move(board), player1);
	board.draw(robot2.get_next_move(board), player2);
	board.draw(robot.get_next_move(board), player1);
	board.draw(robot2.get_next_move(board), player2);
	board.draw(robot.get_next_move(board), player1);
	board.draw(robot2.get_next_move(board), player2);
	board.print();

	return 0;
}
