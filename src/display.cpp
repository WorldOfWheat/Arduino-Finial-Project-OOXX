#ifndef DISPLAY_H
#define DISPLAY_H

#include "./tic-tac-toe/Board.cpp"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class IDisplay {
public:
	virtual void showWelcome() = 0;
	virtual void showBoard() = 0;
	virtual void showGameOver() = 0;
	virtual void showSettings() = 0;
	virtual void showDifficulty(byte difficulty) = 0;
	virtual void showTurn(bool turn) = 0;
	virtual void showIfUseBot(bool use) = 0;
	virtual void showDraw() = 0;
	virtual void showWinner(PlayerType type) = 0;
	virtual void showError() = 0;
};

class SerialDisplay : public IDisplay {
private:
	IBoard* board;

	void printRow(byte row) {
		for (byte i = 0; i < board->get_col_size(); i++) {
			Cell cell = board->get_cell(Coordinate(row, i));
			if (cell.isFilled()) {
				Serial.print((char) board->get_cell(Coordinate(row, i)).getDrawer().getType());
			}
			else {
				Serial.print(" ");
			}
			if (i < 2) {
				Serial.print("|");
			}
		}
		Serial.println("");
	}

	void printBoardSpliter() {
		Serial.println("-+-+-");
	}

	void printBoard() {
		for (byte i = 0; i < board->get_row_size(); i++) {
			printRow(i);
			if (i < 2) {
				printBoardSpliter();
			}
		}
	}

	void printDifficultyString(byte difficulty) {
		switch (difficulty) {
			case 1:
				Serial.print("1: Easy");
				break;
			case 2:
				Serial.print("2: Medium");
				break;
			case 3:
				Serial.print("3: Hard");
				break;
			case 4:
				Serial.print("4: Impossible");
				break;
		}
	}

public:
	SerialDisplay(IBoard* board) {
		this->board = board;
	}

	void showWelcome() {
		Serial.println("Welcome");
		Serial.println("");
		Serial.println("Press 1 to start");
	}

	void showBoard() {
		printBoard();
	}

	void showGameOver() {
		Serial.println("Game Over");
	}

	void showSettings() {
		Serial.println("Settings");
		Serial.println("1: Difficulty");
		Serial.println("2: Turn");
		Serial.println("3: Use Bot");
		Serial.println("");
		Serial.println("9: Back");
	}

	void showDifficulty(byte difficulty) {
		Serial.println("Difficulty");
		// add star to the selected difficulty
		for (byte i = 1; i <= 4; i++) {
			if (i == difficulty) {
				Serial.print("*");
			}
			printDifficultyString(i);
			Serial.println("");
		}
		Serial.println("");
		Serial.println("9: Back");
	}

	void showTurn(bool turn) {
		Serial.println("Turn");
		// add star to the selected turn
		if (turn) {
			Serial.print("*");
		}
		Serial.println("1: O");
		if (!turn) {
			Serial.print("*");
		}
		Serial.println("2: X");
		Serial.println("");
		Serial.println("9: Back");
	}

	void showIfUseBot(bool use) {
		Serial.println("Use Bot");
		if (use) {
			Serial.print("*");
		}
		Serial.println("1: Yes");
		if (!use) {
			Serial.print("*");
		}
		Serial.println("2: No");
		Serial.println("");
		Serial.println("9: Back");
	}

	void showDraw() {
		Serial.println("Draw");
	}

	void showWinner(PlayerType type) {
		Serial.print((char) type);
		Serial.println(" Wins");
	}

	void showError() {
		Serial.println("Error");
		Serial.println("Please click reset button");
	}
};

class OLED : public IDisplay {
private:
	Adafruit_SSD1306* display;
	byte width = 128;
	byte height = 64;

	IBoard* board;

	void printRow(byte row) {
		for (byte i = 0; i < board->get_col_size(); i++) {
			Cell cell = board->get_cell(Coordinate(row, i));
			if (cell.isFilled()) {
				display->print((char) board->get_cell(Coordinate(row, i)).getDrawer().getType());
			}
			else {
				display->print(" ");
			}
			if (i < 2) {
				display->print("|");
			}
		}
		display->println("");
	}

	void printBoardSpliter() {
		display->println("-+-+-");
	}

	void printBoard() {
		for (byte i = 0; i < board->get_row_size(); i++) {
			printRow(i);
			if (i < 2) {
				printBoardSpliter();
			}
		}
	}

	void printDifficultyString(byte difficulty) {
		switch (difficulty) {
			case 1:
				display->print("1: Easy");
				break;
			case 2:
				display->print("2: Medium");
				break;
			case 3:
				display->print("3: Hard");
				break;
			case 4:
				display->print("4: Impossible");
				break;
		}
	}

public:
	OLED(IBoard* board) {
		this->board = board;
		display = new Adafruit_SSD1306(128, 64, &Wire, -1);
		display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
		display->setTextColor(WHITE);
		display->setTextSize(1);
	}

	void showWelcome() {
		display->clearDisplay();
		display->setCursor(0, height / 2 - 10);
		display->println("Welcome");
		display->println("");
		display->println("Press 1 to start");
		display->display();
	}

	void showBoard() {
		display->clearDisplay();
		display->setCursor(0, 20);
		printBoard();
		display->setCursor(width / 2, 20);
		display->display();
	}

	void showGameOver() {
		display->setCursor(0, 0);
		display->println("Game Over");
		display->display();
	}

	void showSettings() {
		display->clearDisplay();
		display->setCursor(0, 0);
		display->println("Settings");
		display->println("1: Difficulty");
		display->println("2: Turn");
		display->println("3: Use Bot");
		display->println("");
		display->println("9: Back");
		display->display();
	}

	void showDifficulty(byte difficulty) {
		display->clearDisplay();
		display->setCursor(0, 0);
		display->println("Difficulty");
		// add star to the selected difficulty
		for (byte i = 1; i <= 4; i++) {
			if (i == difficulty) {
				display->print("*");
			}
			printDifficultyString(i);
			display->println("");
		}
		display->println("");
		display->println("9: Back");
		display->display();
	}

	void showTurn(bool turn) {
		display->clearDisplay();
		display->setCursor(0, 0);
		display->println("Turn");
		// add star to the selected turn
		if (turn) {
			display->print("*");
		}
		display->println("1: O");
		if (!turn) {
			display->print("*");
		}
		display->println("2: X");
		display->println("");
		display->println("9: Back");
		display->display();
	}

	void showIfUseBot(bool use) {
		display->clearDisplay();
		display->setCursor(0, 0);
		display->println("Use Bot");
		if (use) {
			display->print("*");
		}
		display->println("1: Yes");
		if (!use) {
			display->print("*");
		}
		display->println("2: No");
		display->println("");
		display->println("9: Back");
		display->display();
	}

	void showDraw() {
		display->clearDisplay();
		display->setCursor(0, 0);
		display->setTextSize(2);
		display->println("Draw");
		display->display();
		display->setTextSize(1);
	}

	void showWinner(PlayerType type) {
		display->clearDisplay();
		display->setCursor(0, 0);
		display->setTextSize(2);
		display->print((char) type);
		display->println(" Wins");
		display->display();
		display->setTextSize(1);
	}

	void showError() {
		display->clearDisplay();
		display->setCursor(0, 0);
		display->println("Error");
		display->println("Please click reset button");
		display->display();
	}
};

#endif