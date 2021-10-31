#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input {Down, Up};
enum class State {Playing, GameOver, Rest, Win};

struct GameData {
    State m_state{State::Rest};
    std::bitset<2> m_input; // [up, down]
    int score = 0; // current game score
    int high_score = 0; // high score
    int final_score = 0; // final score of current try
    bool hs = false; // flag to see if there is a new record
    int n_passed_pipes = 0; // number of pipes already passed
};

#endif