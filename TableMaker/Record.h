#ifndef RECORD_H
#define RECORD_H

#include <string>

#define String std::string

class Record {
	private:
		String wineName;
		int	wineLocation;
		int buttonID;
		int labelID;

	public:
		Record(String inputWineName);
		~Record();

		void updateLocation(int inputWineLocation);
		void updateRecordData(String inputWineName, int inputWineLocation);
		void updateRecordObjectIDs(int inputButtonID, int inputLabelID);
		//	May be unneeded. Names are provided and verified.
		//	void updateWineName(int wineLocation);
		
		String getWineName();
		int	getWineLocation();
		String getWineLocationAsString();
		int getButtonID();
		int	getLabelID();
};
		
#endif
