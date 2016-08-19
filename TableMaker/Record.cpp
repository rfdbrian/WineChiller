#include "Record.h"

Record::Record(String inputWineName) : 
	wineName(inputWineName) { }

Record::~Record() { } 

/*
 *	Function: Wine location is a 36-base number, describing
 *		the cell in which the object exists.
 */
void Record::updateLocation(int inputWineLocation) {
	wineLocation = inputWineLocation; 
}

void Record::updateRecordData(String inputWineName, int inputWineLocation) {
	wineName = inputWineName;
	wineLocation = inputWineLocation;
}

void Record::updateRecordObjectIDs(int inputButtonID, int inputLabelID) {
	buttonID = inputButtonID;
	labelID = inputLabelID;
}

String Record::getWineName() {
	return wineName;
}

int Record::getWineLocation() {
	return wineLocation;
}

String Record::getWineLocationAsString() {
	String result = String(wineLocation, 36);	
	return result;
}

int Record::getButtonID() {
	return buttonID;
}

int Record::getLabelID() {
	return labelID;
}	
