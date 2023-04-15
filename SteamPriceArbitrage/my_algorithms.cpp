#include "my_algorithms.h"

std::size_t replace_all(std::wstring& inout, std::wstring_view what, std::wstring_view with) {
	std::size_t count{};
	for (std::string::size_type pos{};
		inout.npos != (pos = inout.find(what.data(), pos, what.length()));
		pos += with.length(), ++count)
		inout.replace(pos, what.length(), with.data(), with.length());
	return count;
}