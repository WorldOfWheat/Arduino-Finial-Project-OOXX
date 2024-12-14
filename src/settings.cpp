#include <Arduino.h>
#include <EEPROM.h>

class ISettingsManager {
public:
	virtual void setDifficulty(byte difficulty) = 0;
	virtual byte getDifficulty() = 0;
	virtual void setTurn(bool turn) = 0;
	virtual bool getTurn() = 0;
	virtual void useBot(bool use_bot) = 0;
	virtual bool getUseBot() = 0;
};

class SettingsManager : public ISettingsManager {
public:
	SettingsManager() {
		EEPROM.begin(512);
	}

	void setDifficulty(byte difficulty) {
		EEPROM.write(0, difficulty);
		EEPROM.commit();
	}

	byte getDifficulty() {
		return EEPROM.read(0);
	}

	void setTurn(bool turn) {
		EEPROM.write(1, turn);
		EEPROM.commit();
	}

	bool getTurn() {
		return EEPROM.read(1);
	}

	void useBot(bool use_bot) {
		EEPROM.write(2, use_bot);
		EEPROM.commit();
	}

	bool getUseBot() {
		return EEPROM.read(2);
	}
};