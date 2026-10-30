#include "quiz_logic.h"
#include <cctype>

bool QuizLogic::checkAnswer(const Question& q, char choice) const {
    return std::toupper(static_cast<unsigned char>(q.answer)) ==
           std::toupper(static_cast<unsigned char>(choice));
}

void QuizLogic::updateScore(const Question& q, char choice) {
    if (checkAnswer(q, choice)) {
        score++;
    }
}

int QuizLogic::getScore() const {
    return score;
}

void QuizLogic::resetScore() {
    score = 0;
}