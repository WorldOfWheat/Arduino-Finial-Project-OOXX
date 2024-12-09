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

enum class GameStatus {
	PLAYING,
	WIN,
	DRAW
};

class GameResult {
public:
	GameStatus status;
	byte winner;
	GameResult(bool isWin, byte winner = 0) {
		if (isWin) {
			status = GameStatus::WIN;
			this->winner = winner;
		} else {
			status = GameStatus::DRAW;
		}
	}
};

class IBoard {
public:
	virtual void draw(Coordinate coordinate, Player currentPlayer) = 0;
	virtual void erase(Coordinate coordinate) = 0;
	virtual bool is_board_full() = 0;
	virtual Cell get_cell(Coordinate coordinate) = 0;
};

class Board : public IBoard {
private:
	Cell cells[3][3];

public:
	Board() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cells[i][j] = Cell();
			}
		}
	}

	void draw(Coordinate coordinate, Player currentPlayer) {
		int x = coordinate.x;
		int y = coordinate.y;
		cells[x][y].setType((char) currentPlayer.getType());
	}

	void erase(Coordinate coordinate) {
		int x = coordinate.x;
		int y = coordinate.y;
		cells[x][y].setType(0);
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

	Cell get_cell(Coordinate coordinate) {
		return cells[coordinate.x][coordinate.y];
	}
};

class BoardPrinter {
private:
	Board board;
	void print_split() {
		Serial.println("-+-+-");
	}

	void print_row(int row) {
		for (int i = 0; i < 3; i++) {
			if (board.get_cell(Coordinate(row, i)).getType() == 0) {
				Serial.print(" ");
			} else {
				Serial.print((char) board.get_cell(Coordinate(row, i)).getType());
			}
			if (i < 2) {
				Serial.print("|");
			}
		}
		Serial.println();
	}

public:
	void print(Board board) {
		print_split();
		for (int i = 0; i < 3; i++) {
			print_row(i);
			print_split();
		}

	}
};

class GameJudge {
private:
	IBoard* board;

	GameResult check_row_winner(byte row) {
		Cell cell = (*board).get_cell(Coordinate(row, 0));
		Cell cell2 = (*board).get_cell(Coordinate(row, 1));
		Cell cell3 = (*board).get_cell(Coordinate(row, 2));
		if (
			cell.getType() != 0 &&
			cell.getType() == cell2.getType() && 
			cell2.getType() == cell3.getType()
		) {
			return GameResult(true, cell.getType());
		}
		return GameResult(false);
	}

	GameResult check_column_winner(byte column) {
		Cell cell = (*board).get_cell(Coordinate(0, column));
		Cell cell2 = (*board).get_cell(Coordinate(1, column));
		Cell cell3 = (*board).get_cell(Coordinate(2, column));
		if (
			cell.getType() != 0 &&
			cell.getType() == cell2.getType() && 
			cell2.getType() == cell3.getType()
		) {
			return GameResult(true, cell.getType());
		}
	}

	GameResult check_slash_winner() {
		Cell cell = (*board).get_cell(Coordinate(0, 0));
		Cell cell2 = (*board).get_cell(Coordinate(1, 1));
		Cell cell3 = (*board).get_cell(Coordinate(2, 2));
		if (
			cell.getType() != 0 &&
			cell.getType() == cell2.getType() && 
			cell2.getType() == cell3.getType()
		) {
			return GameResult(true, cell.getType());
		}
	}

	GameResult check_backslash_winner() {
		Cell cell = (*board).get_cell(Coordinate(0, 2));
		Cell cell2 = (*board).get_cell(Coordinate(1, 1));
		Cell cell3 = (*board).get_cell(Coordinate(2, 0));
		if (
			cell.getType() != 0 &&
			cell.getType() == cell2.getType() && 
			cell2.getType() == cell3.getType()
		) {
			return GameResult(true, cell.getType());
		}
	}

public:
	GameResult judge(Board* board) {
		for (int i = 0; i < 3; i++) {
			GameResult row_winner = check_row_winner(i);
			GameResult column_winner = check_column_winner(i);
			if (row_winner.status == GameStatus::WIN) {
				return row_winner;
			}
			if (column_winner.status == GameStatus::WIN) {
				return column_winner;
			}
		}
		GameResult slash_winner = check_slash_winner();
		GameResult backslash_winner = check_backslash_winner();
		if (slash_winner.status == GameStatus::WIN) {
			return slash_winner;
		}
		if (backslash_winner.status == GameStatus::WIN) {
			return backslash_winner;
		}

	}
};