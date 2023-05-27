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



/*

https://steamcommunity.com/market/search/render/?appid=730&norender=1&sort_column=name&sort_dir=desc&search_descriptions=0&count=100&start=20500

success : true
start : 20400
pagesize : 100
total_count : 20538

results: [

***
{
name : "AK-47 | Пустынный повстанец (Поношенное)"
hash_name : "AK-47 | Wasteland Rebel (Well-Worn)"
sell_listings : 21
sell_price : 3714
sell_price_text : "$37.14"
app_icon : "https://cdn.cloudflare.steamstatic.com/steamcommunity/public/images/apps/730/69f7ebe2735c366c65c0b33dae00e12dc40edbe4.jpg"
app_name : "Counter-Strike: Global Offensive"
asset_description
sale_price_text : "$35.53"
}

***

]

*/