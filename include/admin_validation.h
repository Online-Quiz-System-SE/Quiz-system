#pragma once

#include "quiz_logic.h"
#include <string>

bool isValidQuestion(const Question& question);
bool saveQuestion(const Question& question, const std::string& filePath);
