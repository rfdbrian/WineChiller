#include "Wine.h"

Wine::Wine() : name(""), vineyard(""), year(""), wineType(""), price(""), index(-1), image() {};

Wine::Wine(char*  input_name, char* input_vineyard, char*  input_year, char*  input_wineType, char*  input_price, int input_index, int8_t input_image) :
name(input_name), vineyard(input_vineyard), year(input_year), wineType(input_wineType), price(input_price), index(input_index), image(input_image) {};

void Wine::setIndex(int assignedSlotIndex) {
    index = assignedSlotIndex;
}

// Getters for various properties for wine.
//  These properties should never change after initialization.
char*  Wine::getName() {
    return name;
}

char* Wine::getVineyard() {
    return vineyard;
}

char*  Wine::getYear() {
    return year;
}

char*  Wine::getwineType() {
    return wineType;
}

char*  Wine::getPrice() {
    return price;
}

int Wine::getIndex() {
    return index;
}

int8_t Wine::getImage() {
    return image;
}

