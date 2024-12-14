#ifndef OLED_H
#define OLED_H

#include "./tic-tac-toe/Board.cpp"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class IDisplay {
public:
	virtual void showPage(byte page) = 0;
};

class OLED {
private:
	Adafruit_SSD1306* display;
	byte width = 128;
	byte height = 64;

	IBoard* board;

	void printRow(byte row) {
		for (byte i = 0; i < 3; i++) {
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
		for (byte i = 0; i < 3; i++) {
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