#include "Utils.h"

using namespace std;

namespace OracleFormsUtils {

	string getPathFromBuilderWindowTitle(string _title) {
		return _title.substr(23);
	}

	bool isOracleFormsWindow(string _title) {
		return _title.find("Oracle Forms Builder") != string::npos && _title.find(".fmb") != string::npos;
	}
}