#include "assert_fw.h"

TestRunner::~TestRunner() {
	if (fail_count > 0) {
		std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
		exit(1);
	}
}

void Assert(bool value, const std::string& expr_str, const std::string& file,
	const std::string& func, long line, const std::string& hint) {

	using namespace std;

	if (!value) {
		cerr << std::boolalpha;
		cerr << file << "(" << line << "): " << func << ": ";
		cerr << "ASSERT(" << expr_str << ") failed.";
		if (!hint.empty()) {
			cerr << " Hint: " << hint;
		}
		cerr << endl;
		abort();
	}
}