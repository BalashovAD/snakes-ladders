### What's this

his repository contains a quick Proof of Concept (PoC) inspired by [this LeetCode discussion](https://leetcode.com/discuss/interview-question/system-design/4615686/phonepe-sde2-machine-coding-round-24-hours)
It was developed over 6 hours to demonstrate a highly modular and template-based approach to coding. 
The design allows for easy modifications through the addition or replacement of `Actions`, `Objects`, or `Player::Extensions`. 
Implementing new game rules incurs zero cost for the basic game version. 

### How to add new functionality
To introduce new game objects, they must conform to the ObjectWrapper interface:
```cpp
// default constructable and include the following public methods
void trigger(Player& player, Game& state);
decltype(auto) verify(GameState const& state) const noexcept -> Verify; // or bool-like
decltype(auto) name() const noexcept -> string; // or string-like
```
New game rules and behavior modifiers should adhere to the Action interface:
```cpp
ActionResult operator()(Player& player, Game& game) const noexcept;
```
To modify the roll mechanism, implement a custom version of DiceI.

### Game loop
The current game loop operates with the following main rules in sequence:
- Roll dice
- Trigger objects (once)
- Check collision (after snakes or ladders)
- Logs and winner

The code snippet for the game loop is as follows:
```cpp
while(!game.gameLoop(DiceRoll{}, CheckTrigger{}, CheckCollision{}, PrintLog{}, WinCheck{})) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
```
For instance, if you want to check for collisions after each move, you can modify the game loop as shown below. 
This alteration ensures that collisions are evaluated both before and after triggering objects:
```cpp
while(!game.gameLoop(DiceRoll{}, CheckCollision{}, CheckTrigger{}, CheckCollision{}, PrintLog{}, WinCheck{})) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
```

### Usage
See input in `cfg.txt`
```
Config loaded: Players(2)
	Gaurav - 1
	Cheater - 10
Objects(17)
	Ladder<2 -> 37>
	Ladder<10 -> 32>
	Ladder<27 -> 46>
	Snake<33 -> 6>
	Snake<41 -> 20>
	Snake<49 -> 9>
	Ladder<51 -> 68>
	Snake<56 -> 53>
	Ladder<61 -> 79>
	Snake<62 -> 5>
	Ladder<65 -> 84>
	Ladder<71 -> 91>
	Ladder<81 -> 100>
	Snake<88 -> 16>
	Snake<93 -> 73>
	Snake<95 -> 75>
	Snake<98 -> 64>

Player: Gaurav turn
  Roll: 9
  Move Gaurav from 1 to 10
  Trigger: Ladder<10 -> 32>
  Move Gaurav from 10 to 32
Player: Cheater turn
  Roll: 9
  Move Cheater from 10 to 19
Player: Gaurav turn
  Roll: 10
  Move Gaurav from 32 to 42
Player: Cheater turn
  Roll: 9
  Move Cheater from 19 to 28
Player: Gaurav turn
  Roll: 8
  Move Gaurav from 42 to 50
Player: Cheater turn
  Roll: 5
  Move Cheater from 28 to 33
  Trigger: Snake<33 -> 6>
  Move Cheater from 33 to 6
Player: Gaurav turn
  Roll: 5
  Move Gaurav from 50 to 55
Player: Cheater turn
  Roll: 8
  Move Cheater from 6 to 14
Player: Gaurav turn
  Roll: 11
  Move Gaurav from 55 to 66
Player: Cheater turn
  Roll: 7
  Move Cheater from 14 to 21
Player: Gaurav turn
  Roll: 3
  Move Gaurav from 66 to 69
Player: Cheater turn
  Roll: 5
  Move Cheater from 21 to 26
Player: Gaurav turn
  Roll: 13
  Move Gaurav from 69 to 82
Player: Cheater turn
  Roll: 4
  Move Cheater from 26 to 30
Player: Gaurav turn
  Roll: 14
  Move Gaurav from 82 to 96
Player: Cheater turn
  Roll: 10
  Move Cheater from 30 to 40
Player: Gaurav turn
  Roll: 4
  Move Gaurav from 96 to 100
Winner: Gaurav
```