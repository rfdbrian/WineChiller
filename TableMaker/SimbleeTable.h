#ifndef SIMBLEE_TABLE
#define SIMBLEE_TABLE



#include <Arduino.h>
#include <SimbleeForMobile.h>
#include <stdexcept>
#include <vector>
#include <map>
#include "Record.h"
#include <cassert>

#define vector std::vector
#define PERM_X SimbleeForMobile.screenWidth
#define PERM_Y SimbleeForMobile.screenHeight

class SimbleeTable {
	private:
		vector<Record> rows;
		std::map<int,int> button_ids;
		std::map<int,int> label_ids;
		int slider_id;
		
		void draw_line(int startX, int startY, char dir, int len);
		void draw_slider(int startX, int startY, int width, int minS, int maxS);
		void add_button(int startX, int startY, int width, const char *title);
		void add_label(int startX, int startY, const char *text);
		void shift_list(int value);
		void update_rows_to_screen(char desiredState);
        void clear_table();

	public:
		SimbleeTable();		
		SimbleeTable(uint16_t startHeight, const char* titleName);

		void draw_table(uint16_t startHeight, const char* screenTitle);
		void update_table(char desiredState); // More info found in definition

		Record* get_record_by_loc(int crcBase36);
		Record* get_record_by_button_id(uint8_t& inputID, char currScreen);
		void add_record(cString wineName);
		void del_record(cString wineName);

		vector<Record> get_rows(char desiredState);
		bool find_button_id(uint8_t buttonID);
		bool find_label_id(uint8_t buttonID);

        void resetRowsTo(char desiredState);

		//void update_button_ids_vector(std::map<int,int> newButtonVector);
		//void update_label_ids_vector(std::map<int,int> newLabelVector);

//		void content_double_thinK(int shiftVal);

		//std::map<int,int> get_button_ids_vector();
		//std::map<int,int> get_label_ids_vector();

		
		//void clear_object_ids();

		//void hide_object(int objectID);
		//void show_object(int objectID);
};


#endif 
