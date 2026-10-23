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

// F-006, F-007, F-008: answer checking and scoring
void test_answer_and_score() {
    QuizLogic logic;
    Question q1{"What is 2 + 2?", {"1", "2", "4", "5"}, 'C'};
    Question q2{"What is 3 + 3?", {"5", "6", "7", "8"}, 'B'};

    assert(logic.checkAnswer(q1, 'C'));
    assert(!logic.checkAnswer(q1, 'A'));
    assert(logic.checkAnswer(q1, 'c'));

    assert(logic.getScore() == 0);
    logic.updateScore(q1, 'C');
    assert(logic.getScore() == 1);

    logic.updateScore(q2, 'A');
    assert(logic.getScore() == 1);
    logic.updateScore(q2, 'C');
    assert(logic.getScore() == 1);

    logic.updateScore(q2, 'B');
    assert(logic.getScore() == 2);
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
    test_answer_and_score();
    test_start_quiz();
    std::cout << "All tests passed\n";
    return 0;
}
