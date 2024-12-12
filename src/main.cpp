#include <Arduino.h>
#include <Wire.h>
#include "./tic-tac-toe/Player.cpp"
#include "./tic-tac-toe/Board.cpp"
#include "./tic-tac-toe/Robot.cpp"

void setup() {
	Serial.begin(115200);
	Serial.println("Start");
	Player player1 = Player(PlayerType::O);
	Player player2 = Player(PlayerType::X);

	Serial.println((char) player1.getType());
	IBoard* board = new Board();
	BoardPrinter* board_printer = new BoardPrinter(board);
	board->draw(Coordinate(0, 0), player1);
	board_printer->print();
}

void loop() {
}