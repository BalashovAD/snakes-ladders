#include <iostream>
#include <thread>

#include "Game.hpp"
#include "Player.hpp"
#include "Object.hpp"
#include "Snake.hpp"
#include "Action.hpp"

using Pl = Player<>;
using Obj = ObjectWrapper<Snake, Ladder, Crocodile>;

int main() {
    unsigned edgeSize = 0;
    std::unique_ptr<DiceI> spDice;

    std::cin >> edgeSize;

    std::string diceMode;
    unsigned diceCount;
    std::cin >> diceMode >> diceCount;
    if (diceMode == "max") {
        spDice = std::make_unique<Dice<details::Max>>(diceCount);
    } else if (diceMode == "min") {
        spDice = std::make_unique<Dice<details::Min>>(diceCount);
    } else if (diceMode == "sum") {
        spDice = std::make_unique<Dice<details::Sum>>(diceCount);
    }

    Game<Pl, Obj> game(edgeSize, std::move(spDice));

    unsigned snakes;
    std::cin >> snakes;
    for (auto i = 0; i != snakes; ++i) {
        unsigned a, b;
        std::cin >> a >> b;
        game.getBoard().add(Obj{Snake{a, b}}, a);
    }

    unsigned ladders;
    std::cin >> ladders;
    for (auto i = 0; i != ladders; ++i) {
        unsigned a, b;
        std::cin >> a >> b;
        game.getBoard().add(Obj{Ladder{a, b}}, a);
    }

    unsigned players;
    std::cin >> players;
    for (auto i = 0; i != players; ++i) {
        unsigned pos;
        std::string name;
        std::cin >> name >> pos;
        game.addPlayer(Pl{name, pos});
    }

    if (auto v = game.verify(); !v) {
        std::cout << "Error in config: " << v << std::endl;
        return 1;
    }
    std::cout << "Config loaded: " << game << std::endl;

    while(!game.gameLoop(DiceRoll{}, CheckTrigger{}, CheckCollision{}, PrintLog{}, WinCheck{})) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
