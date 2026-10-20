#include "quiz_logic.h"
#include <cctype>

bool QuizLogic::checkAnswer(const Question& q, char choice) const {
    return std::toupper(static_cast<unsigned char>(q.answer)) ==
           std::toupper(static_cast<unsigned char>(choice));
}