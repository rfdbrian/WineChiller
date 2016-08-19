#include "SimbleeTable.h"

SimbleeTable::SimbleeTable(int inputRows = 2, int inputCols = 1) :
	rows(inputRows), columns(inputCols), tableCell(NULL), cellID(NULL) {
		drawTable();
		drawTableTextContainers();
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


	
				
	
