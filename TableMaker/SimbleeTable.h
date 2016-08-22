#ifndef SIMBLEE_TABLE
#define SIMBLEE_TABLE

#include <SimbleeForMobile.h>
#include <stdexcept>
#include <vector>
#include "Record.h"

#ifndef CELL_HEIGHT
#define CELL_HEIGHT 100
#endif

#ifndef CELL_WIDTH
#define CELL_WIDTH 100
#endif

#ifndef SPACING_OFFSET
#define SPACING_OFFSET 52
#endif

class SimbleeTable {
	private:
		vector<Record> rows;
		vector<int> button_ids;
		vector<int> label_ids;

		void draw_table();
		void update_table();
		void draw_line(int startX, int startY, char direction, int length);

		//	ToDo --- Define function
		void add_buttons();
		void draw_slider();
		void add_labels();
		void shift_list(int value);

	public:
		SimbleeTable();

		Record get_record();
		void add_record(Record inputRecord);
		void del_record(int recordVectorIndex);
		int get_total_records();

		vector<int> get_button_ids_vector();
		void update_button_ids_vector(vector<int>* newButtonVector);
		
		vector<int> get_label_ids_vector();
		void update_label_ids_vector(vector<int>* newLabelVector);

		void clear_table();
		void hide_object(int objectID);
		void show_object(int objectID);
}
		

#endif 
