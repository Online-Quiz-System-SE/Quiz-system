#pragma once
#include <functional>
#include <iostream>
#include <vector>
#include "quiz_logic.h"

class QuizController {
public:
    using QuestionLoader = std::function<std::vector<Question>()>;
    using AdminAction    = std::function<void()>;

    QuizController(QuestionLoader loader, AdminAction admin,
                   std::istream& in = std::cin, std::ostream& out = std::cout);

    void run();         // main menu loop
    void startQuiz();   // OQ-F-001: start a new quiz with score 0
    void setDeleteAction(AdminAction action);
    int score() const { return score_; }

private:
    void showMenu() const;
    void showQuestion(std::size_t i) const;  // OQ-F-002, F-003
    char readChoice();                       // OQ-F-004, F-005; '\0' if input ends

    QuestionLoader loader_;
    AdminAction deleteAdmin_;
    AdminAction admin_;
    std::istream& in_;
    std::ostream& out_;
    std::vector<Question> questions_;
    int score_ = 0;
};
