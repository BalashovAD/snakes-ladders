#pragma once

#include <string>
#include <iostream>
#include <sstream>


template <typename ...Extensions>
class Player : public Extensions... {
public:
    explicit Player(std::string name, unsigned pos = 1) noexcept
        : m_name(std::move(name))
        , m_pos(pos) {

    }

    unsigned pos() const noexcept {
        return m_pos;
    }


    template <typename GameState>
    auto verify(GameState const& state) const noexcept {
        return (VERIFY(0 < m_pos) && VERIFY(m_pos < state.size()) && VERIFY(!m_name.empty())).description(name());
    }

    template <typename GameState>
    void move(GameState& state, int shift, std::string const& reason) {
        auto old = m_pos;
        m_pos = std::clamp(m_pos + shift, 0u, state.size());
        std::stringstream str;
        str << "Move " << m_name << " from " << old << " to " << m_pos;
        state.addLog(str.str());
    }

    template <typename GameState>
    void moveTo(GameState& state, unsigned pos, std::string const& reason) {
        move(state, pos - m_pos, reason);
    }

    std::string const& name() const noexcept {
        return m_name;
    }
private:
    std::string m_name;
    unsigned m_pos;
};