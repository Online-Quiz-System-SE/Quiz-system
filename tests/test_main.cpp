#include <cassert>
#include <iostream>
#include "quiz_logic.h"

int main() {
    QuizLogic logic;

    Question q1{"What is 2 + 2?", {"1", "2", "4", "5"}, 'C'};
    Question q2{"What is 3 + 3?", {"5", "6", "7", "8"}, 'B'};

    // F-006: Answer checking
    assert(logic.checkAnswer(q1, 'C'));
    assert(!logic.checkAnswer(q1, 'A'));
    assert(logic.checkAnswer(q1, 'c'));

    // F-007: Score incrementing
    assert(logic.getScore() == 0);

    logic.updateScore(q1, 'C');
    assert(logic.getScore() == 1);

    logic.updateScore(q2, 'A');
    assert(logic.getScore() == 1);

    logic.updateScore(q2, 'B');
    assert(logic.getScore() == 2);

    std::cout << "F-006 and F-007 tests passed!" << std::endl;

    return 0;
}
