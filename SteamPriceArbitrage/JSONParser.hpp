#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <string>

#include "assert_fw.h"

class JSONParser {

public:

	JSONParser() = default;

	template<class Type>
	Type getFieldFromJSON(std::string& json, std::string field) {
		
		std::stringstream jsonEncoded(json);
		boost::property_tree::ptree root;
		boost::property_tree::read_json(jsonEncoded, root);

		ASSERT(!root.empty());

		return root.get<Type>(field);

	}

	template<class Type>
	std::pair<Type, bool> tryGetFieldFromJSON(std::string& json, std::string field, Type default_value = {}) noexcept {

		Type result = default_value;
		bool success = false;

		try {
			std::stringstream jsonEncoded(json);
			boost::property_tree::ptree root;
			boost::property_tree::read_json(jsonEncoded, root);
			result = root.get<Type>(field);
			success = true;
		}
		catch (...) {

		}

		return { result, success };

	}

};