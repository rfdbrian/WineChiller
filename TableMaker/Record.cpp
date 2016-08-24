#include "Record.h"


Record::Record() : 
    wineName(""), wineLocation(0), buttonID(-1), labelID(-1), empty(true) { }
    
Record::Record(String inputWineName) : 
	wineName(inputWineName), wineLocation(0), buttonID(-1), labelID(-1), empty(false) { }

Record::~Record() { } 

/*
 *	Function: Wine location is a 36-base number, describing
 *		the cell in which the object exists.
 */
void Record::updateLocation(int inputWineLocation) {
	wineLocation = inputWineLocation; 
    empty = false;
}

void Record::updateRecordData(String inputWineName, int inputWineLocation) {
	wineName = inputWineName;
	wineLocation = inputWineLocation;
}

void Record::updateRecordLabelID(int inputLabelID) {
	labelID = inputLabelID;
}

void Record::updateRecordButtonID(int inputButtonID) {
    buttonID = inputButtonID;
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

bool Record::isEmpty() {
    return empty;
}

