#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

template <typename It>
std::string PrintRangeToString(It range_begin, It range_end) {
	std::ostringstream out;
	for (auto it = range_begin; it != range_end; ++it) {
		out << *it;
	}
	return out.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s) {
	os << "{";
	bool is_first = true;
	for (const auto& elem : s) {
		if (is_first) {
			os << ", ";
			is_first = false;
		}
		os << elem;
	}
	return os << "}";
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
	os << "{";
	bool first = true;
	for (const auto& kv : m) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << kv.first << ": " << kv.second;
	}
	return os << "}";
}