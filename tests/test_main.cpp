#include <cassert>
#include <iostream>
#include "quiz_logic.h"

int main() {
    QuizLogic logic;

    Question q{"What is 2 + 2?", {"1", "2", "4", "5"}, 'C'};

    assert(logic.checkAnswer(q, 'C'));
    assert(!logic.checkAnswer(q, 'A'));
    assert(logic.checkAnswer(q, 'c'));

    std::cout << "F-006 tests passed!" << std::endl;

    return 0;
}