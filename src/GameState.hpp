#pragma once

#include <vector>

template <typename Player>
class GameState {
public:
    explicit GameState(unsigned size) noexcept
        : m_size(size) {

    }

    auto size() const noexcept {
        return m_size;
    }

    void addLog(std::string msg) noexcept {
        m_messages.emplace_back(msg);
    }

    auto& getLog() noexcept {
        return m_messages;
    }
private:
    unsigned m_size;
    std::vector<std::string> m_messages;
};