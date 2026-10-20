#pragma once
#include <iostream>
#include <vector>
#include "quiz_logic.h"

class QuizController {
public:
    explicit QuizController(std::vector<Question> questions,
                            std::istream& in = std::cin,
                            std::ostream& out = std::cout);
    void run();          // main menu loop
    void startQuiz();    // OQ-F-001
    int score() const { return score_; }

private:
    void showMenu() const;
    void showQuestion(std::size_t i) const;

    std::vector<Question> questions_;
    std::istream& in_;
    std::ostream& out_;
    int score_ = 0;
    std::size_t current_ = 0;
};
