#include <cassert>
#include <iostream>
#include "quiz_logic.h"

int main() {
    QuizLogic logic;
    Question q{"Q", {"a", "b", "c", "d"}, 'B'};
    assert(logic.checkAnswer(q, 'B'));
    assert(!logic.checkAnswer(q, 'A'));
    std::cout << "All tests passed\n";
    return 0;
}
