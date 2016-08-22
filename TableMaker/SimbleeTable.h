#ifndef SIMBLEE_TABLE
#define SIMBLEE_TABLE

#include <SimbleeForMobile.h>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "Record.h"

#define vector std::vector

class SimbleeTable {
	private:
		vector<Record> rows;
		vector<int> button_ids;
		vector<int> label_ids;
		int slider_id;

		void draw_table(uint16_t startHeight);
		void update_table();
		void draw_line(int startX, int startY, char dir, int len);

		void add_button(int startX, int startY, int width, const char *title);
		void draw_slider(int startX, int startY, int width, int minS, int maxS);
		void add_label(int startX, int startY, const char *text);
		void shift_list(int value);

	public:
        SimbleeTable();		
        SimbleeTable(uint16_t startHeight);

		Record get_record_by_loc(int crcBase36);
		void add_record(Record inputRecord);
		void del_record(int recordVectorIndex);
		int get_total_records();

		bool find_button_id(uint8_t buttonID);
		void update_button_ids_vector(vector<int>* newButtonVector);
		
		bool find_label_id(uint8_t buttonID);
		void update_label_ids_vector(vector<int>* newLabelVector);

		void clear_table();
		void hide_object(int objectID);
		void show_object(int objectID);
};
		

#endif 
