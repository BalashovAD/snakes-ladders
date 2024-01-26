#pragma once

#include <vector>
#include <iostream>
#include <set>

#include "Board.hpp"
#include "GameState.hpp"
#include "Dice.hpp"
#include "Verify.hpp"

enum class ActionResult {
    CONTINUE,
    STOP,
    GAME_OVER,
};

template <typename Player, typename Object>
class Game : public GameState<Player> {
    auto verifyFn() const noexcept {
        return [&](auto const& fn) {
            return fn.verify(*this);
        };
    }
public:
    Game(unsigned size, std::unique_ptr<DiceI> spDice) noexcept
        : GameState<Player>(size * size)
        , m_board(size * size)
        , m_spDice(std::move(spDice)) {

    }

    unsigned roll() const noexcept {
        return m_spDice->roll();
    }

    void addPlayer(Player player) noexcept {
        m_players.emplace_back(player);
    }

    auto const& getPlayers() const noexcept {
        return m_players;
    }

    auto& getPlayers() noexcept {
        return m_players;
    }

    auto const& getBoard() const noexcept {
        return m_board;
    }

    auto& getBoard() noexcept {
        return m_board;
    }

    auto verify() const noexcept {

        auto const uniqueNames = [&]() -> Verify {
            std::set<std::string> names;
            for (auto const& p : m_players) {
                if (!names.emplace(p.name()).second) {
                    return Verify("Not unique name").description(p.name());
                }
            }
            return {};
        };

        return VERIFY(m_spDice)
            && VERIFY(m_players.size() > 0)
            && uniqueNames()
            && std::all_of(m_players.begin(), m_players.end(), verifyFn())
            && m_board.verify(*this);
    }

    template <typename ...Actions>
    bool gameLoop(Actions const&...  actions) noexcept {
        static_assert(sizeof...(Actions) > 0);

        assert(verify());
        auto& currentPlayer = m_players[m_currentPlayerId];

        if (loopImpl(currentPlayer, actions...) == ActionResult::GAME_OVER) {
            std::cout << "Winner: " << currentPlayer.name() << std::endl;
            return true;
        }

        m_currentPlayerId = (m_currentPlayerId + 1) % m_players.size();
        return false;
    }

    friend std::ostream& operator<<(std::ostream& os, Game const& gm) {
        os << "Players(" << gm.m_players.size() << ")\n";
        for (auto const& player : gm.m_players) {
            os << "\t" << player.name() << " - " << player.pos() << "\n";
        }
        os << "Objects(" << gm.m_board.objectsCount() << ")\n";
        for (auto i = 0; i != gm.m_board.size(); ++i) {
            if (auto const& obj = gm.m_board.get(i); obj) {
                os << "\t" << obj->name() << "\n";
            }
        }
        return os;
    }
private:
    template <typename ActionT, typename ...Actions>
    ActionResult loopImpl(Player& player, ActionT const& action, Actions const&...  actions) noexcept {

        auto state = action(player, *this);
        if (state == ActionResult::CONTINUE) {
            if constexpr (sizeof...(actions) > 0) {
                return loopImpl(player, actions...);
            } else {
                return state;
            }
        } else {
            return state;
        }
    }

    std::vector<Player> m_players;
    unsigned m_currentPlayerId = 0;
    Board<Object> m_board;
    std::unique_ptr<DiceI> m_spDice;
};