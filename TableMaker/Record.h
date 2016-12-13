#ifndef RECORD_H
#define RECORD_H

#include <string>

#define cString std::string

class Record {
	private:
		cString wineName;
		int	wineLocation;
		int buttonID;
		int labelID;
		char state;

	public:
		Record();
		Record(cString inputWineName, char inputState);

		void updateLocation(int inputWineLocation);
		void updateState(char defState);
		void updateButtonID(int inputButtonID);
		void updateLabelID(int inputLabelID);

		//	May be unneeded. Names are provided and verified.
		//	void updateWineName(int wineLocation);

		cString getWineName();
		int	getWineLocation();
		cString getWineLocationAscString();
		int getButtonID();
		int	getLabelID();
		char getState();
};

#endif
