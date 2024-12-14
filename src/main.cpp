#include <Arduino.h>
#include <Wire.h>
#include "./tic-tac-toe/Player.cpp"
#include "./tic-tac-toe/Board.cpp"
#include "./tic-tac-toe/Robot.cpp"
#include "./keyboard.cpp"
#include "./settings.cpp"
#include "./display.cpp"

ISettingsManager* settings_manager = new SettingsManager();
IKeyInput* key_input = new KeyPad();
IBoard* board = new Board();
GameJudge* judge = new GameJudge(board);
OLED display = OLED(board);
BoardPrinter printer = BoardPrinter(board);

void setup() {
	Serial.begin(115200);
	Serial.println("Start");
	display.showWelcome();
}

byte state = 1;

void updateDifficulty() {
	byte difficulty = settings_manager->getDifficulty();
	display.showDifficulty(difficulty);
}

void updateTurn() {
	bool turn = settings_manager->getTurn();
	display.showTurn(turn);
}

void updateUseBot() {
	bool use_bot = settings_manager->getUseBot();
	display.showIfUseBot(use_bot);
}

Coordinate get_coordinate_by_key(char key) {
	switch (key) {
		case '1':
			return Coordinate(0, 0);
		case '2':
			return Coordinate(0, 1);
		case '3':
			return Coordinate(0, 2);
		case '4':
			return Coordinate(1, 0);
		case '5':
			return Coordinate(1, 1);
		case '6':
			return Coordinate(1, 2);
		case '7':
			return Coordinate(2, 0);
		case '8':
			return Coordinate(2, 1);
		case '9':
			return Coordinate(2, 2);
		default:
			return Coordinate(-1, -1);
	}
}

Player* get_player() {
	bool turn = settings_manager->getTurn();
	if (turn) {
		return new Player(PlayerType::O);
	}
	return new Player(PlayerType::X);
}

Player* get_oppenent(Player player) {
	if (player.getType() == PlayerType::O) {
		return new Player(PlayerType::X);
	}
	return new Player(PlayerType::O);
}

char selected_key;
Coordinate selected;
Player* human;
bool use_bot = settings_manager->getUseBot();
Robot* robot;

void initialize() {
	board->reset();
	human = get_player();
	use_bot = settings_manager->getUseBot();
	if (use_bot) {
		byte difficulty = settings_manager->getDifficulty();
		Player* oppenent = get_oppenent(*human);
		robot = new Robot(*oppenent, board, difficulty);
	}
}

void loop() {
	char key = key_input->get_key();
	if (key != '\0') {
		Serial.println("Pressed: " + key);
	}

	Serial.println("State: " + String(state));
	switch (state) {
		// Welcome
		case 1: {
			display.showWelcome();
			if (key == '1') {
				initialize();
				if (use_bot) {
					if (human->getType() == PlayerType::O) {
						state = 20;
						break;
					}
					state = 23;
					break;
				}
				state = 30;
			}
			if (key == '2') {
				state = 10;
			}
			break;
		}

		case 20: {
			display.showBoard();

			if (key == '\0') {
				break;
			}

			selected_key = key;
			selected = get_coordinate_by_key(selected_key);
			if (selected.x == -1 || selected.y == -1) {
				break;
			}

			if (board->get_cell(selected).isFilled()) {
				break;
			}

			state = 21;

			break;
		}
		
		case 21: {
			board->draw(selected, *human);
			display.showBoard();
			delay(100);

			if (key == selected_key) {
				state = 22;
				break;
			}

			board->erase(selected);
			display.showBoard();
			delay(100);

			if (key != '\0') {
				state = 20;
			}

			break;
		}
		
		case 22: {
			GameResult result = judge->judge();

			if (result.status == GameStatus::WIN) {
				display.showWinner(human->getType());
				delay(1000);
				state = 1;
				break;
			}

			if (result.status == GameStatus::DRAW) {
				display.showDraw();
				delay(1000);
				state = 1;
				break;
			}

			state = 23;
			break;
		}
			
		case 23: {
			robot->play();
			display.showBoard();
			state = 24;
			break;
		}

		case 24: {
			GameResult result = judge->judge();

			if (result.status == GameStatus::WIN) {
				delay(500);
				display.showWinner(get_oppenent(human->getType())->getType());
				delay(1000);
				state = 1;
				break;
			}

			if (result.status == GameStatus::DRAW) {
				delay(500);
				display.showDraw();
				delay(1000);
				state = 1;
				break;
			}

			state = 20;
			break;
		}

		// Human vs Human
		case 30: {
			display.showBoard();

			if (key == '\0') {
				break;
			}

			selected_key = key;
			selected = get_coordinate_by_key(selected_key);
			if (selected.x == -1 || selected.y == -1) {
				break;
			}

			if (board->get_cell(selected).isFilled()) {
				break;
			}

			state = 31;

			break;
		}

		case 31: {
			board->draw(selected, *human);
			display.showBoard();
			delay(100);

			if (key == selected_key) {
				state = 32;
				break;
			}

			board->erase(selected);
			display.showBoard();
			delay(100);

			if (key != '\0') {
				state = 30;
			}

			break;
		}

		case 32: {
			GameResult result = judge->judge();

			if (result.status == GameStatus::WIN) {
				delay(500);
				display.showWinner(human->getType());
				delay(1000);
				state = 1;
				break;
			}

			if (result.status == GameStatus::DRAW) {
				delay(500);
				display.showDraw();
				delay(1000);
				state = 1;
				break;
			}

			state = 33;
			break;
		}

		case 33: {
			human = get_oppenent(*human);
			state = 30;
			break;
		}

		// Settings
		case 10: {
			display.showSettings();
			if (key == '1') {
				state = 11;
				break;
			}
			if (key == '2') {
				state = 12;
				break;
			}
			if (key == '3') {
				state = 13;
				break;
			}
			if (key == '9') {
				state = 1;
				break;
			}
			break;
		}

		// Difficulty
		case 11: {
			if (key == '1') {
				settings_manager->setDifficulty(1);
			}
			if (key == '2') {
				settings_manager->setDifficulty(2);
			}
			if (key == '3') {
				settings_manager->setDifficulty(3);
			}
			if (key == '4') {
				settings_manager->setDifficulty(4);
			}

			updateDifficulty();

			if (key == '9') {
				state = 10;
			}
			break;
		}

		// Turn
		case 12: {
			if (key == '1') {
				settings_manager->setTurn(true);
			}
			if (key == '2') {
				settings_manager->setTurn(false);
			}

			updateTurn();

			if (key == '9') {
				state = 10;
			}
			break;
		}

		// Use Bot
		case 13: {
			if (key == '1') {
				settings_manager->useBot(true);
			}
			if (key == '2') {
				settings_manager->useBot(false);
			}
			updateUseBot();

			if (key == '9') {
				state = 10;
			}
			break;
		}

		default:
			display.showError();
			break;
	}
}