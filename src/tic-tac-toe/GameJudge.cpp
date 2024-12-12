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
		Cell cell = board->get_cell(Coordinate(row, 0));
		Cell cell2 = board->get_cell(Coordinate(row, 1));
		Cell cell3 = board->get_cell(Coordinate(row, 2));
		return cell.isFilled() && cell2.isFilled() && cell3.isFilled();
	}

	bool is_column_filled(byte column) {
		Cell cell = board->get_cell(Coordinate(0, column));
		Cell cell2 = board->get_cell(Coordinate(1, column));
		Cell cell3 = board->get_cell(Coordinate(2, column));
		return cell.isFilled() && cell2.isFilled() && cell3.isFilled();
	}

	Cell get_row_winner(byte row) {
		Cell cell = board->get_cell(Coordinate(row, 0));
		Cell cell2 = board->get_cell(Coordinate(row, 1));
		Cell cell3 = board->get_cell(Coordinate(row, 2));
		if (
			is_row_filled(row) &&
			cell == cell2 && 
			cell2 == cell3
		) {
			return cell;
		}
		return Cell();
	}

	Cell get_column_winner(byte column) {
		Cell cell = board->get_cell(Coordinate(0, column));
		Cell cell2 = board->get_cell(Coordinate(1, column));
		Cell cell3 = board->get_cell(Coordinate(2, column));
		if (
			is_column_filled(column) &&
			cell == cell2 && 
			cell2 == cell3
		) {
			return cell;
		}
		return Cell();
	}

	bool is_slash_filled() {
		Cell cell = board->get_cell(Coordinate(0, 0));
		Cell cell2 = board->get_cell(Coordinate(1, 1));
		Cell cell3 = board->get_cell(Coordinate(2, 2));
		return cell.isFilled() && cell2.isFilled() && cell3.isFilled();
	}

	Cell get_slash_winner() {
		Cell cell = board->get_cell(Coordinate(0, 0));
		Cell cell2 = board->get_cell(Coordinate(1, 1));
		Cell cell3 = board->get_cell(Coordinate(2, 2));
		if (
			is_slash_filled() &&
			cell == cell2 && 
			cell2 == cell3
		) {
			return cell;
		}
		return Cell();
	}

	bool is_backslash_filled() {
		Cell cell = board->get_cell(Coordinate(0, 2));
		Cell cell2 = board->get_cell(Coordinate(1, 1));
		Cell cell3 = board->get_cell(Coordinate(2, 0));
		return cell.isFilled() && cell2.isFilled() && cell3.isFilled();
	}

	Cell get_backslash_winner() {
		Cell cell = board->get_cell(Coordinate(0, 2));
		Cell cell2 = board->get_cell(Coordinate(1, 1));
		Cell cell3 = board->get_cell(Coordinate(2, 0));
		if (
			is_backslash_filled() &&
			cell == cell2 && 
			cell2 == cell3
		) {
			return cell;
		}
		return Cell();
	}

public:
	GameJudge(IBoard* board) {
		this->board = board;
	}

	GameResult judge() {
		for (int i = 0; i < 3; i++) {
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