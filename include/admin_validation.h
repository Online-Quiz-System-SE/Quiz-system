#pragma once

#include "quiz_logic.h"
#include <string>
#include <vector>

bool isValidQuestion(const Question& question);
bool saveQuestion(const Question& question, const std::string& filePath);
bool loadQuestions(std::vector<Question>& questions,
                   const std::string& filePath);