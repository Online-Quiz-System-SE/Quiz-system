#include "quiz_logic.h"

bool QuizLogic::checkAnswer(const Question& q, char choice) const {
    return q.answer == choice;   // Chinmay fills in properly in QZ-07
}