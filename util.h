#ifndef _util_
#define _util_

#include <algorithm> // remove

namespace util {
	std::string removeSpaces(std::string s) {
		s.erase(remove(s.begin(), s.end(), ' '),s.end());
		return s;
	}
};

#endif
