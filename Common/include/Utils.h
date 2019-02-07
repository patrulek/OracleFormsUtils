#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace OracleFormsUtils {

	// _title: "Oracle Forms Builder - <path>"
	std::string getPathFromBuilderWindowTitle(std::string _title);

	bool isOracleFormsWindow(std::string _title);
}

#endif // UTILS_H