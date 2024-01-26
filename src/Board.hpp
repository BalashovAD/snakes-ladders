#pragma once

#include <vector>
#include <optional>
#include "Verify.hpp"


template <typename Object>
class Board {
public:
    explicit Board(unsigned size) noexcept {
        m_cells.resize(size + 1);
    }

    auto size() const noexcept {
        return m_cells.size();
    }

    auto objectsCount() const noexcept {
        return m_objectCount;
    }

    void add(Object obj, unsigned pos) noexcept(false) {
        expect(pos < size(), "Out of range");
        expect(!m_cells[pos], "Already field");

        m_cells[pos] = std::move(obj);
        ++m_objectCount;
    }

    std::optional<Object> const& get(unsigned pos) const noexcept {
        expect(pos < size(), "Out of range");
        return m_cells[pos];
    }

    std::optional<Object>& get(unsigned pos) noexcept(false) {
        expect(pos < size(), "Out of range");
        return m_cells[pos];
    }

    template <typename GameState>
    auto verify(GameState const& state) const noexcept {
        Verify out;
        for (auto const& obj : m_cells) {
            if (obj) {
                out = out && obj->verify(state);
            }
        }
        return out;
    }
private:
    static void expect(bool test, std::string const& msg) noexcept(false) {
        if (!test) [[unlikely]] {
            throw std::runtime_error(msg);
        }
    }
    std::vector<std::optional<Object>> m_cells;
    unsigned m_objectCount = 0;
};