#pragma once

#include <vector>
#include <string>
#include <iostream>

class Verify {
public:
    Verify() noexcept = default;
    explicit Verify(std::string s) noexcept {
        m_errors.emplace_back(s);
    }

    friend Verify operator&&(Verify const& lhs, Verify const& rhs) noexcept {
        Verify out;
        for (auto const& s : lhs.m_errors) {
            out.m_errors.emplace_back(s);
        }
        for (auto const& s : rhs.m_errors) {
            out.m_errors.emplace_back(s);
        }
        return out;
    }

    friend Verify operator&&(Verify const& lhs, bool rhs) noexcept {
        Verify out;
        for (auto const& s : lhs.m_errors) {
            out.m_errors.emplace_back(s);
        }
        if (!rhs) {
            out.m_errors.emplace_back("unknown");
        }
        return out;
    }

    friend Verify operator&&(bool lhs, Verify const& rhs) noexcept {
        Verify out;
        for (auto const& s : rhs.m_errors) {
            out.m_errors.emplace_back(s);
        }
        if (!lhs) {
            out.m_errors.emplace_back("unknown");
        }
        return out;
    }

    Verify& description(std::string const& d) noexcept {
        for (auto& s : m_errors) {
            s += "(" + d + ")";
        }
        return *this;
    }

    explicit(false) operator bool() const noexcept {
        return m_errors.empty();
    }

    friend std::ostream& operator<<(std::ostream& os, Verify const& v) noexcept {
        if (v) {
            os << "No error";
        } else {
            os << v.m_errors.size() << " errors: \n";
            for (auto const& s : v.m_errors) {
                os << s << "\n";
            }
        }
        return os << std::endl;
    }
private:
    std::vector<std::string> m_errors;
};

#define VERIFY(x) [&](){if (!(x)) {return Verify(#x);} else {return Verify();}}()