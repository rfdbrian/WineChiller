#include "SimbleeTable.h"

SimbleeTable::SimbleeTable() :
    rows(NULL), button_ids(NULL), label_ids(NULL), slider_id(0) {
        draw_table(0);
        update_table();
};

SimbleeTable::SimbleeTable(uint16_t startHeight) :
	rows(NULL), button_ids(NULL), label_ids(NULL), slider_id(0) {
		draw_table(startHeight);
		update_table();
};
				
Record SimbleeTable::get_record_by_loc(int crcBase36) {
	Record* ptrToSelRecord = NULL;
	for (vector<Record>::iterator it  = rows.begin() ; it != rows.end(); ++it) {
		if (it->getWineLocation() == crcBase36) {
            // &(*..) Dereferences iterator to reference directly the object.
			ptrToSelRecord = &(*it);
			break;
		}
	}
	return *ptrToSelRecord;
}
			
void SimbleeTable::add_record(Record inputRecord) {
	rows.push_back(inputRecord);
}

void SimbleeTable::del_record(int recordVectorIndex) {
	rows.erase(rows.begin() + recordVectorIndex, rows.end() + recordVectorIndex + 1);
}

int SimbleeTable::get_total_records() {
	return  rows.size();
}

bool SimbleeTable::find_button_id(uint8_t searchValue) {
    for (vector<int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
        if (*it == searchValue) {
            return true;
        }
    }
	return false;
}

void SimbleeTable::update_button_ids_vector(vector<int>* newButtonVector) {
	button_ids = *newButtonVector;
}

bool SimbleeTable::find_label_id(uint8_t searchValue) {
    for (vector<int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
        if (*it == searchValue) {
            return true;
        }
    }
    return false;
} 

void SimbleeTable::update_label_ids_vector(vector<int>* newLabelVector) {
	label_ids = *newLabelVector;
}

void SimbleeTable::clear_table() {
	rows.clear();
	
	for (vector<int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
		SimbleeForMobile.updateText(*it, "");
	}
	
	for (vector<int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
		SimbleeForMobile.updateText(*it, "");
	}
}

void SimbleeTable::hide_object(int objectID) {
	SimbleeForMobile.setVisible(objectID, false);
}

void SimbleeTable::show_object(int objectID) {
	SimbleeForMobile.setVisible(objectID, true);
}

/** 
 * A table is a collection of records. Columns added by need. 
 */ 
void SimbleeTable::draw_table(uint16_t startHeight) {
	//	Table Name 

	// if ((SimbleeForMobile.screenHeight - (inputRows * CELL_HEIGHT) < 0) || 
	// 		(SimbleeForMobile.screenWidth - (inputCols * CELL_ROW) < 0))		 {
	// 	throw std::out_of_range("Number of rows exceed screenHeight");
	// }
	
	uint16_t PERM_X = SimbleeForMobile.screenWidth;
	uint16_t PERM_Y = SimbleeForMobile.screenHeight;

    SimbleeForMobile.drawText(PERM_X / 2 - 100, 50, "STORED WINES", BLACK, 30);
    draw_slider(5, startHeight, PERM_X - 10, 0, 10);

	for (int border = startHeight + 50; border <= PERM_Y; border += 47) {
		draw_line(5, border, 'r', PERM_X - 10);
        if (border < PERM_Y) {
            add_button(5, border + 5, PERM_X - 10, "asdfasdf"); 
            int label_width = PERM_X - 50;
            add_label(label_width, border + 14, "TEST");
        }
	}
}

void    SimbleeTable::update_table() {
	for (vector<Record>::iterator it = rows.begin(); it != rows.end(); ++it) {
        const char* tempCharArray = String(it->getWineLocation(), 36).c_str();
        
		SimbleeForMobile.updateText(it->getButtonID(), it->getWineName().c_str());
		SimbleeForMobile.updateText(it->getLabelID(), tempCharArray);
	}
}		
	
/**
 * Draws a line given a coordinate pair (x, y). Direction is char representing
 * 	direction within the set ['d', 'r']. Length being line length.
 */
void	SimbleeTable::draw_line(int startX, int startY, char dir, int len) {
		switch(dir) {
			case 'd':
				SimbleeForMobile.drawRect(startX, startY, 1, len, BLACK);
				break;
			case 'r':
				SimbleeForMobile.drawRect(startX, startY, len, 1, BLACK);
				break;
	}
}

void	SimbleeTable::add_button(int startX, int startY, int width, const char *title) {
	button_ids.push_back(SimbleeForMobile.drawButton(startX, startY, width, title, YELLOW, TEXT_TYPE)); 
}

void	SimbleeTable::draw_slider(int startX, int startY, int width, int minS, int maxS) {
	SimbleeForMobile.drawSlider(startX, startY, width, minS, maxS);
}

void	SimbleeTable::add_label(int startX, int startY, const char *text) {
	label_ids.push_back(SimbleeForMobile.drawText(startX, startY, text));
}

	
