#pragma once

#include "Item.h"
#include <map>
#include <string>
#include <vector>
#include "MarketCSVParser.h"
#include "log_duration.h"
#include "my_algorithms.h"
#include <deque>
#include <mutex>

class ItemsStore {

public:

	ItemsStore() = delete;

	explicit ItemsStore(std::string filename) {
		
		std::vector<csv_line> data_csv;

		{
			LOG_DURATION("CSV PARSE");

			data_csv = parseCSV("D:\\SteamPriceArbitrage\\items_730_1681315188.csv");
			std::cerr << "DATA SIZE: " << data_csv.size() << std::endl;
		}

		{
			LOG_DURATION("MAP GENERATE");

			const std::map<std::wstring, std::wstring> char_replace{ {L" ", L"%20"}, {L"(", L"%28"}, {L")", L"%29"}, {L"|",L"%7C"}, {L"\"", L""}, {L"'", L"%27"} };
			for (auto item : data_csv) {
				std::wstring market_hash_name = item.c_market_hash_name;
				double market_price = stod(item.c_price) / 100;

				for (auto[k, v] : char_replace) {
					replace_all(market_hash_name, k, v);
				}

				if (auto it = Items.find(market_hash_name); it != Items.end()) {
					if (market_price < it->second.market_price) {
						Items[market_hash_name].market_price = market_price;
					}
				}
				else {
					Items[market_hash_name] = { market_hash_name, 0, 0, market_price };
				}
			}

			std::cerr << "MAP SIZE: " << Items.size() << std::endl;
		}

		UpdateQueue();
		SerializeHead();
	}

	std::wstring GetNextToUpdate() {
		
		m_deque.lock();

		std::wstring name = L"";
		
		if (!deque_update.empty()) {
			name = deque_update.front();
			deque_update.pop_front();
		}

		m_deque.unlock();

		return name;
	}

	void UpdateItem(std::wstring name, double lowest_price, double median_price) {
		
		m_item.lock();

		Items[name].lowest_price = lowest_price;
		Items[name].median_price = lowest_price;
		SerializeItem(Items[name]);

		m_item.unlock();

	}

	

private:
	std::map<std::wstring, Item> Items;
	std::deque<std::wstring> deque_update;
	std::mutex m_deque;
	std::mutex m_item;

	void UpdateQueue() {

		deque_update.clear();

		for (auto[name, item] : Items) {
			deque_update.push_back(name);
		}

	}

};