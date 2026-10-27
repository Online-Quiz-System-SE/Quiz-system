#pragma once

#include "quiz_logic.h"
#include <string>

bool isValidQuestion(const Question& question);
bool saveQuestion(const Question& question, const std::string& filePath);
bool modifyQuestion(const std::string& filePath,
                    std::size_t questionIndex,
                    const Question& updatedQuestion);

bool areCredentialsNonEmpty(const std::string& username,
                            const std::string& password);