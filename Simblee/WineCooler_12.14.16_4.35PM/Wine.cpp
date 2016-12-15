#include "Wine.h"

Wine::Wine() : name(""), vineyard(""), year(""), wineType(""), price(""), index(-1), rating(-1) {};

Wine::Wine(char*  input_name, char* input_vineyard, char*  input_year, char*  input_wineType, char*  input_price, int input_index, int8_t input_rating) :
name(input_name), vineyard(input_vineyard), year(input_year), wineType(input_wineType), price(input_price), index(input_index), rating(input_rating) {};

void Wine::setIndex(int assignedSlotIndex) {
    index = assignedSlotIndex;
}

void Wine::setRating(int assignedRating) {
  rating = assignedRating;
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

int8_t Wine::getRating() {
    return rating;
}

