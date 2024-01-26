#pragma once

#include "GameState.hpp"
#include "Player.hpp"
#include "Game.hpp"


struct DiceRoll {
    template <typename Player, typename Game>
    ActionResult operator()(Player& player, Game& game) const noexcept {
        unsigned roll = game.roll();
        game.addLog("Roll: " + std::to_string(roll));
        player.move(game, roll, "roll");
        return ActionResult::CONTINUE;
    }
};

struct CheckTrigger {
    template <typename Player, typename Game>
    ActionResult operator()(Player& player, Game& game) const noexcept {
        auto& trigger = game.getBoard().get(player.pos());
        if (trigger) {
            trigger->trigger(player, game);
        }
        return ActionResult::CONTINUE;
    }
};

struct WinCheck {
    template <typename Player, typename Game>
    ActionResult operator()(Player& player, Game& game) const noexcept {
        if (player.pos() == game.size()) {
            return ActionResult::GAME_OVER;
        } else {
            return ActionResult::CONTINUE;
        }
    }
};

struct PrintLog {
    template <typename Player, typename Game>
    ActionResult operator()(Player& player, Game& game) const noexcept {
        std::cout << "Player: " << player.name() << " turn\n";
        for (auto const& msg : game.getLog()) {
            std::cout << "  " << msg << "\n";
        }
        game.getLog().clear();
        std::cout.flush();
        return ActionResult::CONTINUE;
    }
};

struct CheckCollision {
    template <typename Player, typename Game>
    ActionResult operator()(Player& player, Game& game) const noexcept {
        for (auto& p : game.getPlayers()) {
            if (p.pos() == player.pos() && p.name() != player.name()) {
                game.addLog("Collided with " + p.name() + ", dropping to 1");
                p.moveTo(game, 1, "collision");
            }
        }
        return ActionResult::CONTINUE;
    }
};