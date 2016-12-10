#include "Wine.h"

Wine::Wine() : name(""), vineyard(""), year(-1), country(""), price(""), index(-1) {};

Wine::Wine(std::string input_name, std::string input_vineyard, int input_year, std::string input_country, std::string input_price, int input_index) : 
  name(input_name), vineyard(input_vineyard), year(input_year), country(input_country), price(input_price), index(input_index) {};

void Wine::setIndex(int assignedSlotIndex) {
  index = assignedSlotIndex;
}

// Getters for various properties for wine.
//  These properties should never change after initialization.
std::string Wine::getName() {
  return name;
}

std::string Wine::getVineyard() {
  return vineyard;
}

int Wine::getYear() {
  return year;
}

std::string Wine::getCountry() {
  return country;
}

std::string Wine::getPrice() {
  return price;
}

int Wine::getIndex() {
  return index;
} 


