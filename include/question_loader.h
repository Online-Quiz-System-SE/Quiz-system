
#pragma once

#include <string>
#include <vector>
#include "quiz_logic.h"

// Load all valid questions from the specified file.
std::vector<Question> loadQuestions(const std::string& filename);

// Load valid questions belonging to one topic.
std::vector<Question> loadQuestionsByTopic(
    const std::string& filename,
    const std::string& topic
);
