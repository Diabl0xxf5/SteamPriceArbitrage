#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

struct csv_line {


	std::wstring c_classid;
	std::wstring c_instanceid;
	std::wstring c_price;
	std::wstring c_offers;
	std::wstring c_popularity;
	std::wstring c_rarity;
	std::wstring c_quality;
	std::wstring c_heroid;
	std::wstring c_slot;
	std::wstring c_stickers;
	std::wstring c_market_name;
	std::wstring c_market_name_en;
	std::wstring c_market_hash_name;
	std::wstring c_name_color;
	std::wstring c_price_updated;
	std::wstring c_pop;
	std::wstring c_base_id;

	csv_line(std::wstring line) {

		std::wstringstream stream(line); // Преобразование строки в поток

		getline(stream, this->c_classid, delimiter);
		getline(stream, this->c_instanceid, delimiter);
		getline(stream, this->c_price, delimiter);
		getline(stream, this->c_offers, delimiter);
		getline(stream, this->c_popularity, delimiter);
		getline(stream, this->c_rarity, delimiter);
		getline(stream, this->c_quality, delimiter);
		getline(stream, this->c_heroid, delimiter);
		getline(stream, this->c_slot, delimiter);
		getline(stream, this->c_stickers, delimiter);
		getline(stream, this->c_market_name, delimiter);
		getline(stream, this->c_market_name_en, delimiter);
		getline(stream, this->c_market_hash_name, delimiter);
		getline(stream, this->c_name_color, delimiter);
		getline(stream, this->c_price_updated, delimiter);
		getline(stream, this->c_pop, delimiter);
		getline(stream, this->c_base_id, delimiter);

	}

	const wchar_t delimiter = ';';
	
};

std::vector<csv_line> parseCSV(std::string filename) {

	std::ifstream work_file(filename);

	std::vector<csv_line> data;
	data.reserve(100'000);

	if (work_file.is_open()) {
		std::wifstream f(filename);
		std::wstringstream ss;
		ss << f.rdbuf();

		std::wstring line;
		getline(ss, line);

		while (getline(ss, line)) {
			data.push_back(line);
		}

	}

	work_file.close();

	return data;

}