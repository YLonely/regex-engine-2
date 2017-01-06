#pragma once
#include <stdexcept>
#include <string>

namespace regex_engine2_exception
{

std::wstring s2ws(const std::string &s);

/************************************************************************/
/*                  Not a good exception implementation                 */
/************************************************************************/

class engine_exception : public std::exception
{
public:
	virtual void info() const = 0;

protected:
	std::wstring detial;
};

class regex_logic_error : public engine_exception
{
public:
	regex_logic_error() = delete;
	explicit regex_logic_error(const std::wstring &detial) : detial(detial) {}
	virtual void info() const = 0;

protected:
	std::wstring detial;
};

class syntax_error final : public regex_logic_error
{
public:
	syntax_error() = delete;
	explicit syntax_error(const std::wstring &detial, const std::wstring &regex, std::wstring::size_type pos) : regex_logic_error(detial), regex(regex), pos(pos)
	{
		this->detial = std::move(detial);
	}
	void info() const override;

private:
	std::wstring regex;
	std::wstring::size_type pos;
};

class arguement_error final : public regex_logic_error
{
public:
	arguement_error() = delete;
	explicit arguement_error(const std::wstring &detial, const std::wstring &regex, std::wstring::size_type pos) : regex_logic_error(detial), regex(regex), pos(pos)
	{
		this->detial = std::move(detial);
	}
	void info() const override;

private:
	std::wstring regex;
	std::wstring::size_type pos;
};


class regex_runtime_error :public engine_exception
{
public:
	regex_runtime_error() = delete;
	explicit regex_runtime_error(const std::wstring &detial):detial(detial){}
	void info() const override;
private:
	std::wstring detial;
};
}