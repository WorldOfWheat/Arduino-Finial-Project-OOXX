#ifndef GAME_JUDGE_H
#define GAME_JUDGE_H
#include <Arduino.h>
#include "./Board.cpp"
#include "./Player.cpp"

enum class GameStatus {
	PLAYING = 0,
	WIN = 1,
	DRAW = 2
};

class GameResult {
public:
	GameStatus status;
	Player winner = Player(PlayerType::O);
	GameResult(bool isDraw) {
		if (isDraw) {
			this->status = GameStatus::DRAW;
		} else {
			this->status = GameStatus::PLAYING;
		}
	}
	GameResult(Player winner) {
		this->status = GameStatus::WIN;
		this->winner = winner;
	}
};

class IGameJudge {
public:
	virtual GameResult judge() = 0;
};

class GameJudge : public IGameJudge {
private:
	IBoard* board;

	bool is_row_filled(byte row) {
		for (int i = 0; i < board->get_col_size(); i++) {
			Cell cell = board->get_cell(Coordinate(row, i));
			if (!cell.isFilled()) {
				return false;
			}
		}
		return true;
	}

	bool is_column_filled(byte column) {
		for (int i = 0; i < board->get_row_size(); i++) {
			Cell cell = board->get_cell(Coordinate(i, column));
			if (!cell.isFilled()) {
				return false;
			}
		}
		return true;
	}

	Cell get_row_winner(byte row) {
		if (!is_row_filled(row)) {
			return Cell();
		}
		for (int i = 1; i < board->get_row_size(); i++) {
			Cell cell = board->get_cell(Coordinate(row, i));
			Cell cell2 = board->get_cell(Coordinate(row, i - 1));
			if (cell != cell2) {
				return Cell();
			}
		}
		return board->get_cell(Coordinate(row, 0));
	}

	Cell get_column_winner(byte column) {
		if (!is_column_filled(column)) {
			return Cell();
		}
		for (int i = 0; i < board->get_col_size() - 1; i++) {
			Cell cell = board->get_cell(Coordinate(i, column));
			Cell cell2 = board->get_cell(Coordinate(i + 1, column));
			if (cell != cell2) {
				return Cell();
			}
		}
		return board->get_cell(Coordinate(0, column));
	}

	bool is_slash_filled() {
		for (int i = 0; i < board->get_row_size(); i++) {
			Cell cell = board->get_cell(Coordinate(i, i));
			if (!cell.isFilled()) {
				return false;
			}
		}
		return true;
	}

	Cell get_slash_winner() {
		if (!is_slash_filled()) {
			return Cell();
		}
		for (int i = 0; i < board->get_row_size() - 1; i++) {
			Cell cell = board->get_cell(Coordinate(i, i));
			Cell cell2 = board->get_cell(Coordinate(i + 1, i + 1));
			if (cell != cell2) {
				return Cell();
			}
		}
		return board->get_cell(Coordinate(0, 0));
	}

	bool is_backslash_filled() {
		for (int i = 0; i < board->get_row_size(); i++) {
			Cell cell = board->get_cell(Coordinate(i, 2 - i));
			if (!cell.isFilled()) {
				return false;
			}
		}
		return true;
	}

	Cell get_backslash_winner() {
		if (!is_backslash_filled()) {
			return Cell();
		}
		for (int i = 0; i < board->get_row_size(); i++) {
			Cell cell = board->get_cell(Coordinate(i, board->get_col_size() - 1 - i));
			Cell cell2 = board->get_cell(Coordinate(i + 1, board->get_col_size() - 2 - i));
			if (cell != cell2) {
				return Cell();
			}
		}
		return board->get_cell(Coordinate(0, board->get_col_size() - 1));
	}

public:
	GameJudge(IBoard* board) {
		this->board = board;
	}

	GameResult judge() {
		for (int i = 0; i < board->get_row_size(); i++) {
			Cell row_winner = get_row_winner(i);
			Cell column_winner = get_column_winner(i);
			if (row_winner.isFilled()) {
				return GameResult(row_winner.getDrawer());
			}
			if (column_winner.isFilled()) {
				return GameResult(column_winner.getDrawer());
			}
		}
		Cell slash_winner = get_slash_winner();
		Cell backslash_winner = get_backslash_winner();
		if (slash_winner.isFilled()) {
			return GameResult(slash_winner.getDrawer());
		}
		if (backslash_winner.isFilled()) {
			return GameResult(backslash_winner.getDrawer());
		}
		if (board->is_board_full()) {
			return GameResult(true);
		}
		return GameResult(false);
	}
};

#endif