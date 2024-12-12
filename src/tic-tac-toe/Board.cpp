#ifndef BOARD_H
#define BOARD_H
#include <Arduino.h>
#include "./tic-tac-toe/Player.cpp"
#include "./tic-tac-toe/Coordinate.cpp"

class ICell {
public:
	virtual Player getDrawer() = 0;
	virtual void setDrawer(Player drawer) = 0;
	virtual bool isFilled() = 0;
};

class Cell : public ICell {
private:
	Player* drawer;
public:
	Cell() {
		drawer = nullptr;
	}

	Cell(Player* drawer) {
		this->drawer = drawer;
	}

	Player getDrawer() {
		return *drawer;
	}

	void setDrawer(Player drawer) {
		this->drawer = &drawer;
	}

	bool isFilled() {
		return drawer != nullptr;
	}

	bool operator == (const Cell& compare_cell) {
		return drawer == compare_cell.drawer;
	}
};

class IBoard {
public:
	virtual void draw(Coordinate coordinate, Player currentPlayer) = 0;
	virtual void erase(Coordinate coordinate) = 0;
	virtual bool is_board_full() = 0;
	virtual bool is_board_empty() = 0;
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
		cells[x][y].setDrawer(currentPlayer);	
	}

	void erase(Coordinate coordinate) {
		int x = coordinate.x;
		int y = coordinate.y;
		cells[x][y] = Cell();
	}

	bool is_board_full() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (!cells[i][j].isFilled()) {
					return false;
				}
			}
		}
		return true;
	}

	bool is_board_empty() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (cells[i][j].isFilled()) {
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
	IBoard* board;
	void print_split() {
		Serial.println("-+-+-");
	}

	void print_row(int row) {
		for (int i = 0; i < 3; i++) {
			Cell cell = (*board).get_cell(Coordinate(row, i));
			if (cell.isFilled()) {
				Serial.print((char) cell.getDrawer().getType());
			}
			else {
				Serial.print("#");
			}
			if (i < 2) {
				Serial.print("|");
			}
		}
		Serial.println();
	}

public:
	BoardPrinter(IBoard* board) {
		this->board = board;
	}
	void print() {
		print_split();
		for (int i = 0; i < 3; i++) {
			print_row(i);
			print_split();
		}
	}
};

#endif