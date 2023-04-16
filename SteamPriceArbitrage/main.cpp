#include <iostream>
#include "HTTPClient.hpp"
#include "JSONParser.hpp"
#include "WinHTTPClient.hpp"
#include "log_duration.h"
#include <map>
#include <algorithm>
#include "ProxyList.hpp"
#include "ItemsStore.hpp"
#include <thread>
#include <chrono>


int main()
{

	
	ProxyList proxy_list;

	{
		LOG_DURATION("PROXYLIST FILL");

		proxy_list.AddProxyHTTP("G:\\SteamPriceArbitrage\\proxylist_http.txt");
		proxy_list.AddProxyListSOCK4("G:\\SteamPriceArbitrage\\proxylist_sock4.txt");
		proxy_list.AddProxyListSOCK5("G:\\SteamPriceArbitrage\\proxylist_sock5.txt");
	}

	ItemsStore itemStore("G:\\SteamPriceArbitrage\\items_730_1681315188.csv");

	{
		LOG_DURATION("STEAM PRICE FILL");
		
		WinHTTPClient client(L"steamcommunity.com", L"/market/priceoverview/");
		JSONParser parser;
		const std::wstring staticRequest = L"&currency=5&appid=730&format=json";

		auto WriteItem = [&](std::wstring& name, double lowest_price, double median_price, bool& good_proxy) {
			
			using namespace std::chrono_literals;
			
			itemStore.UpdateItem(name, lowest_price, median_price);
			name = itemStore.GetNextToUpdate();
			good_proxy = true;
			std::this_thread::sleep_for(3000ms);

		};

		auto Updater = [&]() {
			
			bool good_proxy = false;
			std::wstring market_hash_name = itemStore.GetNextToUpdate();
			std::string proxyAdress;
		
			while (!market_hash_name.empty()) {

				if (!good_proxy) {
					proxyAdress = proxy_list.GetNext();
				}

				std::wstring wbodyRequest = L"?market_hash_name=";
				wbodyRequest.append(market_hash_name);
				wbodyRequest.append(staticRequest);
				std::wstring wproxyAdress(proxyAdress.begin(), proxyAdress.end());

				int cur_try = 0;
				while (cur_try++ < 3) {

					good_proxy = false;

					auto[json, success] = client.getResponse(wbodyRequest, wproxyAdress);

					if (!success) {
						//std::cerr << "GET RESPONSE ERROR" << json << std::endl;
						continue;
					}

					if (json.empty()) {
						//std::cerr << "EMPTY JSON. REQUEST: " << bodyRequest << std::endl;
						continue;
					}
					else if (json == ")L") {
						//std::cerr << "NO DATA. REQUEST: " << bodyRequest << std::endl;
						WriteItem(market_hash_name, -1, -1, good_proxy);
						break;
					}

					auto[r_success, success_parse] = parser.tryGetFieldFromJSON<bool>(json, "success", false);
					if (!success_parse) {
						//std::cerr << "SUCCESS PARSE ERROR. json: " << json << std::endl;
						//std::cerr << "REQUEST: " << bodyRequest << std::endl;
						continue;
					}
					else if (!r_success) {
						//std::cerr << market_hash_name.c_str() << std::endl;
						std::cerr << "GET SUCCESS EQUELS FALSE. json: " << json << "\n";
						continue;
					}

					double lowest_price = -1;
					auto[tmp_lowest_price, lprice_parse] = parser.tryGetFieldFromJSON<std::string>(json, "lowest_price");
					if (lprice_parse) {
						lowest_price = stod(tmp_lowest_price);
					}

					double median_price = -1;
					auto[tmp_medium_price, mprice_parse] = parser.tryGetFieldFromJSON<std::string>(json, "median_price");
					if (mprice_parse) {
						median_price = stod(tmp_medium_price);
					}

					WriteItem(market_hash_name, lowest_price, median_price, good_proxy);
					break;
				}
			}		
		};
		
		std::vector<std::thread> threads;

		int num_thread = 20;

		for (int i = 0; i < num_thread; ++i) {
			std::thread t(Updater);
			threads.push_back(move(t));
		}

		for (int i = 0; i < num_thread; ++i) {
			threads[i].join();
		}

	}

	return 0;
}
