#include "StarSystemSim/utilities/error.h"

#include <iostream>
#include <cstdarg>

namespace utils {

	void fatalError(const char* format, ...) {
		char message[256];

		va_list ap;
		va_start(ap, format);
		vsprintf_s(message, sizeof(message), format, ap);
		va_end(ap);

		std::cerr << "\033[1;31m" << "SSS::FatalError: " << message << "\033[0m\n";

		exit(1);
	}

	void printError(const char* format, ...) {
		char message[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(message, sizeof(message), format, ap);
		va_end(ap);

		std::cerr << "\033[0;33m" << "SSS::Error: " << message << "\033[0m\n";
	}

	void print(const char* format, ...) {
		char message[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(message, sizeof(message), format, ap);
		va_end(ap);

		std::cerr << "\033[0;32m" << "SSS::Info: " << message << "\033[0m\n";
	}

}