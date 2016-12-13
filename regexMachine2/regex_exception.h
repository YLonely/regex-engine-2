#pragma once
#include <stdexcept>
#include <string>

namespace regex_engine2_exception {

class arguement_error final :public std::logic_error {
public:
	arguement_error() = delete;
	explicit arguement_error(const std::string &detial, const std::wstring &regex, std::wstring::size_type pos) :
		logic_error(detial), regex(regex), pos(pos) {
		this->detial = std::move(s2ws(detial));
	}
	void info() const;
private:
	std::wstring detial;
	std::wstring regex;
	std::wstring::size_type pos;
	std::wstring s2ws(const std::string& s);
};
}