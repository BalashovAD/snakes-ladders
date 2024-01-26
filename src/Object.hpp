#pragma once

#include "Player.hpp"

#include <string>

template <typename ...Objects>
class ObjectWrapper {
public:
    using Variant = std::variant<Objects...>;

    template <class T> requires(std::constructible_from<Variant, T>)
    explicit ObjectWrapper(T t) noexcept
        : m_variant(t) {

    }

    template <typename Player, typename Game>
    void trigger(Player& player, Game& state) {
        std::visit([&](auto& obj) {
            obj(player, state);
        }, m_variant);
    }

    template <typename GameState>
    Verify verify(GameState const& state) const noexcept {
        return std::visit([&](auto const& obj) {
            return obj.verify(state);
        }, m_variant).description(name());
    }


    decltype(auto) name() const noexcept {
        return std::visit([](auto& obj) {
            return obj.name();
        }, m_variant);
    }
private:
    Variant m_variant;
};