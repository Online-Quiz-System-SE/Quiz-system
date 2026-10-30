
#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "quiz_logic.h"

class QuizController {
public:
    using QuestionLoader = std::function<std::vector<Question>()>;
    using AdminAction = std::function<void()>;

    QuizController(
        QuestionLoader loader,
        AdminAction admin,
        std::istream& in = std::cin,
        std::ostream& out = std::cout
    );

    void run();
    void startQuiz();
    int score() const { return score_; }

private:
    void showMenu() const;
    void showQuestion(std::size_t i) const;
    char readChoice();
    std::string selectTopic();
    bool runQuizAttempt(const std::string& topic);
    std::vector<Question> selectRandomQuestions(
        const std::vector<Question>& questions,
        const std::string& topic
    );

    QuestionLoader loader_;
    AdminAction admin_;
    std::istream& in_;
    std::ostream& out_;

    std::vector<Question> questions_;
    std::vector<std::string> previousQuestionSet_;
    int score_ = 0;
};
