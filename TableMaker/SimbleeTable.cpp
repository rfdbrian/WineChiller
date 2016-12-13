#include "SimbleeTable.h"

SimbleeTable::SimbleeTable() :
	rows(NULL), button_ids(), label_ids(), slider_id(-1) {
	};

SimbleeTable::SimbleeTable(uint16_t startHeight, const char* titleName) :
	rows(NULL), button_ids(), label_ids(), slider_id(-1) {
		draw_table(startHeight, titleName);
	};

Record* SimbleeTable::get_record_by_loc(int crcBase36) {
	Record* ptrToSelRecord = NULL;
	for (vector<Record>::iterator it  = rows.begin() ; it != rows.end(); ++it) {
		if (it->getWineLocation() == crcBase36) {
			// &(*..) Dereferences iterator to reference directly the object.
			ptrToSelRecord = &(*it);
#ifdef DEBUG
			Serial.println("RECORD FOUND BY LOC");
#endif
			break;
		}
	}

	if (ptrToSelRecord == NULL) {
		return 0;
	}
	return ptrToSelRecord;
}

Record* SimbleeTable::get_record_by_button_id(uint8_t& inputID, char desiredChar) {
	Record* ptrToSelRecord = NULL;
	for (vector<Record>::iterator it  = rows.begin() ; it != rows.end(); ++it) {
        if (it->getState() != desiredChar) {
            continue;
        }
		if (it->getButtonID() == inputID) {
			// &(*..) Dereferences iterator to reference directly the object.
			ptrToSelRecord = &(*it);
#ifdef DEBUG
			Serial.println("RECORD FOUND BY BUTTON ID");
#endif            
			break;
		}
	}
	if (ptrToSelRecord == NULL) {
		return 0;
	}
	return ptrToSelRecord;
}

void SimbleeTable::add_record(cString wineName) {
	Record newRecord = Record(wineName, 'l');
	rows.push_back(newRecord);
}

void SimbleeTable::del_record(cString wineName) {
	int checkValue = rows.size();
	for (vector<Record>::iterator it = rows.begin(); it != rows.end(); ++it) {
		if (it->getWineName().compare(wineName) == 0) {
			rows.erase(it);
#ifdef DEBUG         
			Serial.println("Deleted");
#endif          
			break;
		}
	}
	assert(checkValue == checkValue - 1);
}

bool SimbleeTable::find_button_id(uint8_t searchValue) {
	std::map<int, int>::iterator it;
	it = button_ids.find(searchValue);

	if (it != button_ids.end()) {
		return true;
	}
	return false;
}

bool SimbleeTable::find_label_id(uint8_t searchValue) {
	std::map<int, int>::iterator it;
	it = label_ids.find(searchValue);

	if (it != label_ids.end()) {
		return true;
	}
	return false;
}

void SimbleeTable::resetRowsTo(char desiredState) {
    vector<Record>::iterator recordIter = rows.begin();
    for (recordIter; recordIter != rows.end(); ++recordIter) {
        recordIter->updateState(desiredState);
        recordIter->updateLocation(-1);
    }
}


/**
	A table is a collection of records. Columns added by need.
	*/
void SimbleeTable::draw_table(uint16_t startHeight, const char* screenTitle) {
	//	Table Name
//    clear_table();
	// if ((SimbleeForMobile.screenHeight - (inputRows * CELL_HEIGHT) < 0) ||
	// 		(SimbleeForMobile.screenWidth - (inputCols * CELL_ROW) < 0))		 {
	// 	throw std::out_of_range("Number of rows exceed screenHeight");
	// }

	
	SimbleeForMobile.drawText(PERM_X / 2 - 100, 50, screenTitle, BLACK, 30);
//	draw_slider(5, startHeight, PERM_X - 10, 0, 10);

	for (int border = startHeight + 50; border <= PERM_Y; border += 47) {
		draw_line(5, border, 'r', PERM_X - 10);
		if (border < PERM_Y) {
			add_button(5, border + 5, PERM_X - 10, " ");
			int label_width = PERM_X - 50;
			add_label(label_width, border + 14, " ");
		}
	}
}

/**
 * There exists two states which are recognized.
 *  'l' -> Limbo
 *  's' -> Stored
 */
void	SimbleeTable::update_table(char desiredChar) {
    update_rows_to_screen(desiredChar);
	for (vector<Record>::iterator it = rows.begin(); it != rows.end(); ++it) {
		if (it->getState() != desiredChar) {
			continue;
		}
		String tempCharArray = String(it->getWineLocation(), 36);
		String wineName = String(it->getWineName().c_str());

		if (it->getWineLocation() == -1) {
			tempCharArray = "";
		}

		SimbleeForMobile.updateText(it->getButtonID(), wineName.c_str());
#ifdef DEBUG       
		Serial.print("WineName: ");
		Serial.print(wineName.c_str());
		Serial.print(" ---- ButtonID: ");
		Serial.println(it->getButtonID());
#endif      
		SimbleeForMobile.updateText(it->getLabelID(), tempCharArray.c_str());
#ifdef DEBUG       
		Serial.print("Label ID: ");
		Serial.print(tempCharArray.c_str());
		Serial.print(" ---- Label Value: ");
		Serial.println(it->getLabelID());

		Serial.println("\n");
#endif       
	}
}

void    SimbleeTable::clear_table() {
    for (std::map<int,int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
        SimbleeForMobile.updateText(it->first, "");
    }
    for (std::map<int,int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
        SimbleeForMobile.updateText(it->first, "");
    }
}

/**
	Draws a line given a coordinate pair (x, y). Direction is char representing
	direction within the set ['d', 'r']. Length being line length.
	*/
void	SimbleeTable::draw_line(int startX, int startY, char dir, int len) {
	switch (dir) {
		case 'd':
			SimbleeForMobile.drawRect(startX, startY, 1, len, BLACK);
			break;
		case 'r':
			SimbleeForMobile.drawRect(startX, startY, len, 1, BLACK);
			break;
	}
}

void	SimbleeTable::add_button(int startX, int startY, int width, const char *title) {
    color_t fuschia = rgb(255, 0, 128);
	uint8_t deviceID = SimbleeForMobile.drawButton(startX, startY, width, title, fuschia, TEXT_TYPE);
	SimbleeForMobile.setEvents(deviceID, EVENT_RELEASE);
	button_ids[deviceID] = -1;
}

void	SimbleeTable::draw_slider(int startX, int startY, int width, int minS, int maxS) {
	slider_id = SimbleeForMobile.drawSlider(startX, startY, width, minS, maxS);
}

void	SimbleeTable::add_label(int startX, int startY, const char *text) {
    color_t fuschia = rgb(255, 0, 128);
	uint8_t deviceID = SimbleeForMobile.drawText(startX, startY, text, fuschia);
	label_ids[deviceID] = -1;
}

void SimbleeTable::update_rows_to_screen(char desiredState) {
	std::map<int,int>::iterator buttonIDIter = button_ids.begin();
	std::map<int,int>::iterator labelIDIter = label_ids.begin();

	for (vector<Record>::iterator newRecord = rows.begin(); newRecord != rows.end(); ++newRecord) {
		if (newRecord->getState() != desiredState) {
			continue;
		} 
		if ((buttonIDIter == button_ids.end()) || (labelIDIter == label_ids.end())) {
			break;
		}

		//	Deal with buttons
		newRecord->updateButtonID(buttonIDIter->first);
		buttonIDIter->second = newRecord->getWineLocation();
		++buttonIDIter;

		//	Deal with labels
		newRecord->updateLabelID(labelIDIter->first);
		labelIDIter->second = newRecord->getWineLocation();
		++labelIDIter;

#ifdef DEBUG
        Serial.print("This is where this should be updated...\t");
        Serial.println(newRecord->getButtonID());
#endif
	}
}
