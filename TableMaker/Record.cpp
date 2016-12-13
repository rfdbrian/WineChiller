#include "Record.h"

Record::Record() : 
	wineName(""), wineLocation(-1), buttonID(-1), labelID(-1), state(-1) { };

Record::Record(cString inputWineName, char inputState) : 
	wineName(inputWineName), wineLocation(-1), buttonID(-1), labelID(-1), state(inputState) { };

/*
 *	Function: Wine location is a 36-base number, describing
 *		the cell in which the object exists.
 */
void Record::updateLocation(int inputWineLocation) {
	wineLocation = inputWineLocation; 
}

void Record::updateLabelID(int inputLabelID) {
	labelID = inputLabelID;
}

void Record::updateButtonID(int inputButtonID) {
	buttonID = inputButtonID;
}

void Record::updateState(char defState) {
	state = defState;
}

cString Record::getWineName() {
	return wineName;
}

int Record::getWineLocation() {
	return wineLocation;
}

cString Record::getWineLocationAscString() {
	cString result = cString(wineLocation, 36);	
	return result;
}

int Record::getButtonID() {
	return buttonID;
}

int Record::getLabelID() {
	return labelID;
}	

char Record::getState() {
	return state;
}

