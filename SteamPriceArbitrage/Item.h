#pragma once

#include <string>


struct Item {

	std::wstring market_hash_name;
	double lowest_price;
	double median_price;
	double market_price;

	std::wstring toCSVLine();
};

void SerializeHead();

void SerializeItem(Item& item);