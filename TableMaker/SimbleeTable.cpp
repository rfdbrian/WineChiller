#include "SimbleeTable.h"

SimbleeTable::SimbleeTable() :
	rows(NULL), button_ids(NULL), label_ids(NULL) {
		drawTable();
};

/** 
 * A table is a collection of records. Columns added by need. 
 */ 
void SimbleeTable::drawTable() {
	//	Table Name 
	SimbleeForMobile.drawRect(100, 40, "STORED WINES", BLACK, 40);

	if ((SimbleeForMobile.screenHeight - (inputRows * CELL_HEIGHT) < 0) || 
			(SimbleeForMobile.screenWidth - (inputCols * CELL_ROW) < 0))		 {
		throw std::out_of_range("Number of rows exceed screenHeight");
	}
	//	Utilize for-loop to generate X number of cells line by line,
	//		given the following condition: 
	//			SimbleeForMobile.screenHeight - (cell-y-pos + cell-height) > 0
	//		Note: For this library, we'll assume a resolution of 480x360
	
	//	Line weigth of table outline is specified by min(height, width) of a given
	//		drawn rectangle. 
	
	//	Drawing column lines.
	int cellPositionOffset = 0;
	int widthOfTable = 1;
	for (int colRunner = 0; colRunner <= columns; colRunner++) {
		cellPositionOffset = colRunner * CELL_WIDTH;
		SimbleeForMobile.drawRect(1 + cellPositionOffset, 100, 1, CELL_HEIGHT, BLACK); 
		widthOfTable += 1 + cellPositionOffset;
	}

	//	Drawing row lines.
	cellPositionOffset = 0;
	for (int rowRunner = 0; rowRunner <= rows; rowRunner++) { 
		cellPositionOffset = rowRunner * CELL_HEIGHT;
		SimbleeForMobile.drawRect(1, 100 + cellPositionOffset, widthOfTable, 1, BLACK);
	}
}

void SimbleeTable::drawTableTextContainers() {
	//	Simblee has a "Borderless" button called TEXT_TYPE button. 
	//		A label is similar to the previous in most respects, except default
	//		text centering.  
	
	int rowCellPositionOffset = 0;
	int colCellPositionOffset = 0;
	//	Drawing row lines.
	for (int rowRunner = 0; rowRunner < rows; rowRunner++) {
		for (int colRunner = 0; colRunner < columns; colRunner++) {
			rowCellPositionOffset = rowRunner * CELL_HEIGHT;
			colCellPositionOffset = colRunner * CELL_WIDTH;
			SimbleeForMobile.drawButton(1 + rowCellPositionOffset, 100 + colCellPositionOffset, CELL_WIDTH, BLACK, TEXT_TYPE);
		}
	}
}	
				
Record SimbleeTable::get_record_by_loc(int crcBase36) {
	Record* ptrToSelRecord = NULL;
	for (vector<Record>::iterator it  = rows.begin() ; it != rows.end(); ++it) {
		if (*it->getWineLocation() == crcBase36) {
			ptrToSelRecord = *it;
			break;
		}
	}
	return &ptrToSelRecord;
}
			
void add_record(Record inputRecord) {
	rows.push_back(inputRecord);
}

void del_record(int recordVectorIndex) {
	rows.erase(recordVectorIndex);
}

void get_total_records() {
	return	rows.size();
}

vector<int> get_button_ids_vector() {
	return button_ids;
}

void update_button_ids_vector(vector<int>* newButtonVector);
	button_ids = &newButtonVector;
}

void get_label_ids_vector() {
	return label_ids;
} 

void update_label_ids_vector(vector<int>* newLabelVector) {
	label_ids = &newLabelVector;
}

void clear_table() {
	rows.clear();
	
	for (vector<int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
		SimbleeForMobile.updateValue(*it->getButtonID(), "");
	}
	
	for (vector<int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
		SimbleeForMobile.updateValue(*it->getLabelID(), "");
	}
}

void hide_object(int objectID) {
	SimbleeForMobile.setVisible(objectID, false);
}

void show_object(int objectID) {
	SimbleeForMobile.setVisible(objectID, true);
}

 
	
	
