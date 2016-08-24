#ifndef SIMBLEE_TABLE
#define SIMBLEE_TABLE

#include <Arduino.h>
#include <SimbleeForMobile.h>
#include <stdexcept>
#include <vector>
#include <map>
#include "Record.h"

#define vector std::vector

class SimbleeTable {
	private:
		vector<Record> rows;
		std::map<int,int> button_ids;
        std::map<int,int> label_ids;
		int slider_id;
		void draw_line(int startX, int startY, char dir, int len);

		void add_button(int startX, int startY, int width, const char *title);
		void draw_slider(int startX, int startY, int width, int minS, int maxS);
		void add_label(int startX, int startY, const char *text);
		void shift_list(int value);

	public:
		SimbleeTable();		

        void draw_table(uint16_t startHeight, const char* screenTitle);
        void update_table();
		SimbleeTable(uint16_t startHeight, const char* titleName);

		Record* get_record_by_loc(int crcBase36);
		Record* get_record_by_button_id(uint8_t inputObjectID);
		void add_record(Record inputRecord);
		void del_record(Record* inputRecord);
		int get_total_records();
        
        vector<Record> get_rows();
        void update_rows(vector<Record> inputVector);

		bool find_button_id(uint8_t buttonID);
		void update_button_ids_vector(std::map<int,int> newButtonVector);

		bool find_label_id(uint8_t buttonID);
		void update_label_ids_vector(std::map<int,int> newLabelVector);

        std::map<int,int> get_button_ids_vector();
        std::map<int,int> get_label_ids_vector();
		
		void clear_table();
        void clear_object_ids();
		void hide_object(int objectID);
		void show_object(int objectID);
};


#endif 
