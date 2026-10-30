
#pragma once

#include <string>
#include <vector>

struct Question {
    std::string text;
    std::vector<std::string> options;
    char answer;
    std::string topic = "";
};

class QuizLogic {
private:
    int score = 0;

public:
    bool checkAnswer(const Question& q, char choice) const;
    void updateScore(const Question& q, char choice);
    int getScore() const;
    void resetScore();
};
