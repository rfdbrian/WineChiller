#ifndef WINE_H
#define WINE_H

#include <string>

class Wine {
	private:
		std::string name;
    std::string vineyard;
    int year;
		std::string country;
		std::string price;
		int index;

	public:
    Wine();
    Wine(std::string input_name, std::string input_vineyard, int input_wineYear, std::string input_country, std::string input_price, int input_index);

		void setIndex(int assignedSlotIndex);

    std::string getName();
    std::string getVineyard();
    int getYear();
    std::string getCountry();
    std::string getPrice();
    int getIndex();
};

#endif
