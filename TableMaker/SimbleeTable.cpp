#include "SimbleeTable.h"

SimbleeTable::SimbleeTable() :
  rows(NULL), button_ids(), label_ids(), slider_id(0) {
};

SimbleeTable::SimbleeTable(uint16_t startHeight, const char* titleName) :
  rows(NULL), button_ids(), label_ids(), slider_id(0) {
  draw_table(startHeight, titleName);
  update_table();
};

Record* SimbleeTable::get_record_by_loc(int crcBase36) {
  Record* ptrToSelRecord = NULL;
  for (vector<Record>::iterator it  = rows.begin() ; it != rows.end(); ++it) {
    if (it->getWineLocation() == crcBase36) {
      // &(*..) Dereferences iterator to reference directly the object.
      ptrToSelRecord = &(*it);
      break;
    }
  }
  return ptrToSelRecord;
}

Record* SimbleeTable::get_record_by_button_id(uint8_t inputObjectID) {
  Record* ptrToSelRecord = NULL;
  Serial.println(inputObjectID);
  for (vector<Record>::iterator it  = rows.begin() ; it != rows.end(); ++it) {
    if (it->getButtonID() == inputObjectID) {
      // &(*..) Dereferences iterator to reference directly the object.
      ptrToSelRecord = &(*it);
        Serial.println(it->getWineName().c_str());
      break;
    }
  }
  if (ptrToSelRecord == NULL) {
    return 0;
  }
  return ptrToSelRecord;
}

void SimbleeTable::add_record(Record inputRecord) {
  Record newRecord = inputRecord;
  for (std::map<int, int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
    if (it->second == -1) {
      newRecord.updateRecordButtonID(it->first);
      button_ids[it->first] = newRecord.getWineLocation();
      break;
    }
  }

  for (std::map<int, int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
    if (it->second == -1) {
      newRecord.updateRecordLabelID(it->first);
      label_ids[it->first] = newRecord.getWineLocation();
      break;
    }
  }
  rows.push_back(newRecord);

}

void SimbleeTable::del_record(Record *inputRecord) {

  for (vector<Record>::iterator it = rows.begin(); it != rows.end(); ++it) {
    if (it->getWineName().compare(inputRecord->getWineName()) == 0) {
      rows.erase(it);
      Serial.println("Deleted");
      break;
    }
  Serial.println(it->getWineName().c_str());
  }
}

int SimbleeTable::get_total_records() {
  return  rows.size();
}

bool SimbleeTable::find_button_id(uint8_t searchValue) {
  std::map<int, int>::iterator it;
  it = button_ids.find(searchValue);

  if (it != button_ids.end()) {
    return true;
  }
  return false;
}

std::map<int,int> SimbleeTable::get_button_ids_vector() {
    return button_ids;
}


std::map<int,int> SimbleeTable::get_label_ids_vector() {
    return label_ids;
}

vector<Record> SimbleeTable::get_rows() {
    return rows;
}

void SimbleeTable::update_rows(vector<Record> inputVector) {
    clear_table();    
    rows = inputVector;
    clear_object_ids();
    for (vector<Record>::iterator newRecord = rows.begin(); newRecord != rows.end(); ++newRecord) {
        for (std::map<int, int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
              Serial.println(it->first);
            if (it->second == -1) {
              newRecord->updateRecordButtonID(it->first);
              button_ids[it->first] = newRecord->getWineLocation();
              break;
              
            }
            Serial.println(newRecord->getWineName().c_str());
        }
        
        for (std::map<int, int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
            if (it->second == -1) {
              newRecord->updateRecordLabelID(it->first);
              label_ids[it->first] = newRecord->getWineLocation();
              break;
            }
        }
    }
  update_table();
}


void SimbleeTable::update_button_ids_vector(std::map<int,int> newButtonVector) {
  button_ids = newButtonVector;
}

bool SimbleeTable::find_label_id(uint8_t searchValue) {
  std::map<int, int>::iterator it;
  it = label_ids.find(searchValue);

  if (it != label_ids.end()) {
    return true;
  }
  return false;
}

void SimbleeTable::update_label_ids_vector(std::map<int, int> newLabelVector) {
  label_ids = newLabelVector;
}

void SimbleeTable::clear_object_ids() {
  for (std::map<int, int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
//    SimbleeForMobile.updateText(it->first, "");
    button_ids[it->first] = -1;
  }

  for (std::map<int, int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
//    SimbleeForMobile.updateText(it->first, "");
        label_ids[it->first] = -1;
  }
}

void SimbleeTable::clear_table() {
  rows.clear();

  for (std::map<int, int>::iterator it = button_ids.begin(); it != button_ids.end(); ++it) {
    SimbleeForMobile.updateText(it->first, "");
    it->second = -1;
  }

  for (std::map<int, int>::iterator it = label_ids.begin(); it != label_ids.end(); ++it) {
    SimbleeForMobile.updateText(it->first, "");
    it->second = -1;
  }
}

void SimbleeTable::hide_object(int objectID) {
  SimbleeForMobile.setVisible(objectID, false);
}

void SimbleeTable::show_object(int objectID) {
  SimbleeForMobile.setVisible(objectID, true);
}

/**
   A table is a collection of records. Columns added by need.
*/
void SimbleeTable::draw_table(uint16_t startHeight, const char* screenTitle) {
  //	Table Name

  // if ((SimbleeForMobile.screenHeight - (inputRows * CELL_HEIGHT) < 0) ||
  // 		(SimbleeForMobile.screenWidth - (inputCols * CELL_ROW) < 0))		 {
  // 	throw std::out_of_range("Number of rows exceed screenHeight");
  // }

  uint16_t PERM_X = SimbleeForMobile.screenWidth;
  uint16_t PERM_Y = SimbleeForMobile.screenHeight;

  SimbleeForMobile.drawText(PERM_X / 2 - 100, 50, screenTitle, BLACK, 30);
  draw_slider(5, startHeight, PERM_X - 10, 0, 10);

  for (int border = startHeight + 50; border <= PERM_Y; border += 47) {
    draw_line(5, border, 'r', PERM_X - 10);
    if (border < PERM_Y) {
      add_button(5, border + 5, PERM_X - 10, " ");
      int label_width = PERM_X - 50;
      add_label(label_width, border + 14, " ");
    }
  }
}

String base36ToStr(uint16_t value) {
    String testValue = String(value, 36);
    String result = "";
    for (int i = 0; i < testValue.length(); i++) {
        
        
    }
  
  return testValue;
}


void	SimbleeTable::update_table() {
  for (vector<Record>::iterator it = rows.begin(); it != rows.end(); ++it) {
    String tempCharArray = String(it->getWineLocation(), 36);

    if (it->getWineLocation() == '0') {
        tempCharArray = "";
    }

    SimbleeForMobile.updateText(it->getButtonID(), it->getWineName().c_str());
    Serial.print(it->getWineName().c_str());
    Serial.print(" ---- ");
    Serial.println((*it).getButtonID());
    SimbleeForMobile.updateText(it->getLabelID(), tempCharArray.c_str());
    Serial.print(tempCharArray.c_str());
    Serial.print(" ---- ");
    Serial.println((*it).getLabelID());
    Serial.println(it->getWineLocation());

    Serial.println("\n");
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
  uint8_t deviceID = SimbleeForMobile.drawButton(startX, startY, width, title, YELLOW, TEXT_TYPE);
  SimbleeForMobile.setEvents(deviceID, EVENT_RELEASE);
  button_ids[deviceID] = -1;
}

void	SimbleeTable::draw_slider(int startX, int startY, int width, int minS, int maxS) {
  SimbleeForMobile.drawSlider(startX, startY, width, minS, maxS);
}

void	SimbleeTable::add_label(int startX, int startY, const char *text) {
  uint8_t deviceID = SimbleeForMobile.drawText(startX, startY, text);
  label_ids[deviceID] = -1;
}


