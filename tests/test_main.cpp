
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "quiz_logic.h"
#include "admin_validation.h"

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

    // F-008: Incorrect answers must not increase the score
    logic.updateScore(q2, 'A');
    assert(logic.getScore() == 1);
    logic.updateScore(q2, 'C');
    assert(logic.getScore() == 1);
    logic.updateScore(q2, 'B');
    assert(logic.getScore() == 2);

    // QZ-15: Question validation
    assert(isValidQuestion(q1));

    Question emptyText{"   ", {"A", "B", "C", "D"}, 'A'};
    assert(!isValidQuestion(emptyText));

    Question wrongOptionCount{"Question?", {"A", "B", "C"}, 'A'};
    assert(!isValidQuestion(wrongOptionCount));

    Question emptyOption{"Question?", {"A", "", "C", "D"}, 'A'};
    assert(!isValidQuestion(emptyOption));

    Question invalidAnswer{"Question?", {"A", "B", "C", "D"}, 'E'};
    assert(!isValidQuestion(invalidAnswer));

    // QZ-15: Saving a question
    const std::string testFile = "test_questions_tmp.txt";
    std::remove(testFile.c_str());

    assert(saveQuestion(q1, testFile));

    std::ifstream file(testFile);
    std::string savedLine;
    assert(std::getline(file, savedLine));
    assert(savedLine ==
        "What is 2 + 2?|1|2|4|5|C");

    // Invalid questions must not be saved
    assert(!saveQuestion(invalidAnswer, testFile));

    file.close();
    std::remove(testFile.c_str());

    std::cout << "All quiz logic and question validation tests passed!"
              << std::endl;

    return 0;
}
