
#include "question_loader.h"
#include "admin_validation.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {
std::string trim(const std::string& value) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }

    const std::size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool validTopic(const std::string& topic) {
    return topic == "ML" || topic == "DBMS" ||
           topic == "SE" || topic == "DSA" ||
           topic == "GK";
}
}

std::vector<Question> loadQuestions(const std::string& filename) {
    std::vector<Question> questions;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open question file: "
                  << filename << '\n';
        return questions;
    }

    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue;
        }

        std::stringstream stream(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(stream, field, '|')) {
            fields.push_back(trim(field));
        }

        // Format:
        // Topic|Question|Option A|Option B|Option C|Option D|Answer
        if (fields.size() != 7) {
            std::cerr << "Warning: Skipping malformed record at line "
                      << lineNumber << ". Expected seven fields.\n";
            continue;
        }

        if (!validTopic(fields[0])) {
            std::cerr << "Warning: Invalid topic at line "
                      << lineNumber << ".\n";
            continue;
        }

        if (fields[6].size() != 1) {
            std::cerr << "Warning: Invalid answer at line "
                      << lineNumber << ".\n";
            continue;
        }

        Question question;
        question.topic = fields[0];
        question.text = fields[1];
        question.options = {
            fields[2], fields[3], fields[4], fields[5]
        };
        question.answer = static_cast<char>(
            std::toupper(static_cast<unsigned char>(fields[6][0]))
        );

        if (!isValidQuestion(question)) {
            std::cerr << "Warning: Skipping invalid question at line "
                      << lineNumber << ".\n";
            continue;
        }

        questions.push_back(question);
    }

    if (file.bad()) {
        std::cerr << "Error: Failed while reading question file: "
                  << filename << '\n';
    }

    return questions;
}

std::vector<Question> loadQuestionsByTopic(
    const std::string& filename,
    const std::string& topic) {

    std::vector<Question> allQuestions = loadQuestions(filename);
    std::vector<Question> selectedQuestions;

    if (!validTopic(topic)) {
        return selectedQuestions;
    }

    for (const Question& question : allQuestions) {
        if (question.topic == topic) {
            selectedQuestions.push_back(question);
        }
    }

    return selectedQuestions;
}
