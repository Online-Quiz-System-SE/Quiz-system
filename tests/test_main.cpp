#include <cassert>
#include <iostream>
#include <sstream>
#include "quiz_logic.h"
#include "quiz_controller.h"

void test_check_answer() {
    QuizLogic logic;
    Question q{"Q", {"a", "b", "c", "d"}, 'B'};
    assert(logic.checkAnswer(q, 'B'));
    assert(!logic.checkAnswer(q, 'A'));
}

// TC-01: starting a quiz sets score to 0 and shows question 1
void test_start_quiz() {
    std::vector<Question> qs = {{"Q1?", {"a", "b", "c", "d"}, 'B'}};
    std::istringstream in("1\n3\n");
    std::ostringstream out;
    QuizController c(qs, in, out);
    c.run();
    assert(c.score() == 0);
    assert(out.str().find("Question 1 of 1: Q1?") != std::string::npos);
}

int main() {
    test_check_answer();
    test_start_quiz();
    std::cout << "All tests passed\n";
    return 0;
}
