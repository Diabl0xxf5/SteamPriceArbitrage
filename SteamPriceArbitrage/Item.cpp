#include "Item.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include "log_duration.h"

std::wstring Item::toCSVLine() {
	std::wstringstream ss;
	ss << market_hash_name << ";" << lowest_price << ";" << median_price << ";" << market_price;
	return ss.str();
}

void SerializeHead() {

	LOG_DURATION("HEAD SERIALIZE");

	static const std::wstring headCSV = L"market_hash_name;lowest_price;median_price;market_price";

	std::wofstream out;
	out.open("data.csv", std::ios_base::out | std::ios_base::trunc);

	if (out.is_open()) {
		out << headCSV << "\n";
	}

	out.close();

}

void SerializeItem(Item& item) {

	LOG_DURATION("ITEM SERIALIZE");

	std::wofstream out;
	out.open("data.csv", std::ios_base::app);

	if (out.is_open()) {
		out << item.toCSVLine() << "\n";
	}

	out.close();

}