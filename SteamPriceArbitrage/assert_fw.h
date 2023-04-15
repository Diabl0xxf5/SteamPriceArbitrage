#pragma once

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>

#include "output_functions.h"

class TestRunner {
public:

	template <class TestFunc>
	void RunTest(TestFunc func, const std::string& test_name);

	~TestRunner();

private:
	int fail_count = 0;
};

template <class TestFunc>
void TestRunner::RunTest(TestFunc func, const std::string& test_name) {
	try {
		func();
		std::cerr << test_name << " OK" << std::endl;
	}
	catch (std::exception& e) {
		++fail_count;
		std::cerr << test_name << " fail: " << e.what() << std::endl;
	}
	catch (...) {
		++fail_count;
		std::cerr << "Unknown exception caught" << std::endl;
	}
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)

void Assert(bool value, const std::string& expr_str, const std::string& file,
	const std::string& func, long line, const std::string& hint);

#define ASSERT(expr) Assert((expr), #expr, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_HINT(expr, hint) Assert((expr),  #expr, __FILE__, __FUNCTION__, __LINE__, (hint))


template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file,
	const std::string& func, long line, const std::string& hint) {

	using namespace std;

	if (t != u) {
		cerr << boolalpha;
		cerr << file << "("s << line << "): "s << func << ": "s;
		cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
		cerr << t << " != "s << u << "."s;
		if (!hint.empty()) {
			cerr << " Hint: "s << hint;
		}
		cerr << endl;
		abort();
	}
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define CATCHER(some_throw) \
try { some_throw; ASSERT_HINT(false, #some_throw);} \
catch (const invalid_argument& ex) { /*OK*/ } \
catch (const out_of_range& ex) { /*OK*/ }