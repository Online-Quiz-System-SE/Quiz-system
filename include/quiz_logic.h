#pragma once
#include <string>
#include <vector>

struct Question {
    std::string text;
    std::vector<std::string> options;  // A–D
    char answer;                        // 'A'..'D'
};

class QuizLogic {
public:
    bool checkAnswer(const Question& q, char choice) const;
};