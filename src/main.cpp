#include <Arduino.h>
#include <Wire.h>
#include "./tic-tac-toe/Player.cpp"
#include "./tic-tac-toe/Board.cpp"
#include "./tic-tac-toe/Robot.cpp"
#include "./keyboard.cpp"
#include "./display.cpp"

IKeyInput* key_input = new KeyPad();

void setup() {
	Serial.begin(115200);
	Serial.println("Start");

	IDisplay* display = new OLED();
	display->showWelcome();

	// IBoard* board = new Board();

	// Player player1 = Player(PlayerType::O);
	// Player player2 = Player(PlayerType::X);

	// Robot robot = Robot(player2, board, 4);
	// BoardPrinter* board_printer = new BoardPrinter(board);
	// board->draw(Coordinate(0, 0), player1);
	// robot.play();
	// board->draw(Coordinate(0, 1), player1);
	// robot.play();
	// board->draw(Coordinate(1, 0), player1);
	// robot.play();
	// board_printer->print();
}

byte state = 1;



void loop() {
	switch (state) {
		case 1:
			break;
		
		default:
			state = 1;
			break;
	}
	char key = key_input->get_key();
	if (key != '\0') {
		Serial.println(key);
	}
}