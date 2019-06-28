#pragma once
#include <stdexcept>
#include <string>

namespace lw_regex {
std::wstring s2ws(const std::string& s);

/************************************************************************/
/*                  Not a good exception implementation                 */
/************************************************************************/

class engine_exception : public std::exception {
   public:
    virtual std::wstring info() const = 0;

   protected:
    std::wstring detail;
};

class regex_logic_error : public engine_exception {
   public:
    regex_logic_error() = delete;
    explicit regex_logic_error(const std::wstring& detail) : detail(detail) {}
    virtual std::wstring info() const = 0;

   protected:
    std::wstring detail;
};

class syntax_error final : public regex_logic_error {
   public:
    syntax_error() = delete;
    explicit syntax_error(const std::wstring& detail,
                          const std::wstring& regex,
                          std::wstring::size_type pos)
        : regex_logic_error(detail), regex(regex), pos(pos) {
        this->detail = std::move(detail);
    }
    std::wstring info() const override;

   private:
    std::wstring regex;
    std::wstring::size_type pos;
};

class arguement_error final : public regex_logic_error {
   public:
    arguement_error() = delete;
    explicit arguement_error(const std::wstring& detail,
                             const std::wstring& regex,
                             std::wstring::size_type pos)
        : regex_logic_error(detail), regex(regex), pos(pos) {
        this->detail = std::move(detail);
    }
    std::wstring info() const override;

   private:
    std::wstring regex;
    std::wstring::size_type pos;
};

class regex_runtime_error : public engine_exception {
   public:
    regex_runtime_error() = delete;
    explicit regex_runtime_error(const std::wstring& detail) : detail(detail) {}
    std::wstring info() const override;

   private:
    std::wstring detail;
};

}  // namespace lw_regex