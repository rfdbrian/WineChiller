#ifndef WINE_H
#define WINE_H

#include <string>

class Wine {
private:
    char*  name;
    char* vineyard;
    char*  year;
    char*  wineType;
    char*  price;
    //index = -3 = consumed
    //-2 = gifted
    //-1 = in possesion
    //0 to 24 = in chiller
    int index;
    int8_t image;
    
public:
    Wine();
    Wine( char*  input_name, char* input_vineyard, char*  input_wineYear, char*  input_wineType, char*  input_price, int input_index, int8_t input_image);
    
    void setIndex(int assignedSlotIndex);
    
    char*  getName();
    char* getVineyard();
    char*  getYear();
    char*  getwineType();
    char*  getPrice();
    int getIndex();
    int8_t getImage();
};

#endif
