#pragma once

#include "GameState.hpp"

class MovementObject {
public:
    MovementObject(unsigned head, unsigned tail, std::string const& name)
        : m_head(head)
        , m_tail(tail) {
        m_name = name + "<" + std::to_string(head) + " -> " + std::to_string(tail) + ">";
    }

    template <typename GameState>
    auto verify(GameState const& gameState) const noexcept {
        return VERIFY(m_head < gameState.size()) && VERIFY(m_tail <= gameState.size());
    }

    template <typename Player>
    void operator()(Player& player, GameState<Player>& gameState) noexcept {
        gameState.addLog("Trigger: " + name());
        player.moveTo(gameState, m_tail, name());
    }

    std::string const& name() const noexcept {
        return m_name;
    }
protected:
    unsigned m_head;
    unsigned m_tail;
    std::string m_name;
};

class Snake : public MovementObject{
public:
    using Super = MovementObject;
    Snake(unsigned head, unsigned tail)
        : MovementObject(head, tail, "Snake") {}

    template <typename GameState>
    auto verify(GameState const& gameState) const noexcept {
        return Super::verify(gameState) && VERIFY(m_tail < m_head) && VERIFY(m_head > 0);
    }
};

class Ladder : public MovementObject {
public:
    using Super = MovementObject;
    Ladder(unsigned head, unsigned tail)
        : MovementObject(head, tail, "Ladder") {}

    template <typename GameState>
    auto verify(GameState const& gameState) const noexcept {
        return Super::verify(gameState) && VERIFY(m_tail > m_head);
    }
};

class Crocodile : public MovementObject {
public:
    static constexpr auto MOVE = 5;

    using Super = MovementObject;
    explicit Crocodile(unsigned head)
        : MovementObject(head, std::min(0u, head - MOVE), "Crocodile") {}
};