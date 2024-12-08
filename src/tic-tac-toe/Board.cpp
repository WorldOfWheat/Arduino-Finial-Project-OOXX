#include <Arduino.h>
#include "./tic-tac-toe/Player.cpp"
#include "./tic-tac-toe/Coordinate.cpp"

class Cell {
private:
	// O = 1
	// X = 2
	// Empty = 0
	byte type;
public:
	Cell(byte type = 0) : type(type) {};
	byte getType() {
		return type;
	}
	void setType(byte type) {
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

class Board {
private:
	Cell cells[3][3];

	GameResult check_row_winner(byte row) {
		if (
			cells[row][0].getType() != 0 &&
			cells[row][0].getType() == cells[row][1].getType() && 
			cells[row][1].getType() == cells[row][2].getType()
		) {
			return GameResult(true, cells[row][0].getType());
		}
		return GameResult(false);
	}

	GameResult check_column_winner(byte column) {
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
		Serial.println("-+-+-");
	}

	void print_row(int row) {
		for (int i = 0; i < 3; i++) {
			if (cells[row][i].getType() == 0) {
				Serial.print(" ");
			} else {
				Serial.print((char) cells[row][i].getType());
			}
			if (i < 2) {
				Serial.print("|");
			}
		}
		Serial.println();
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
		cells[x][y].setType(currentPlayer.getType());
	}

	void erase(Coordinate coordinate) {
		int x = coordinate.x;
		int y = coordinate.y;
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