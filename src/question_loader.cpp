
#include "question_loader.h"
#include "admin_validation.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

        // Skip empty lines.
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        // A question must have exactly six fields.
        if (fields.size() != 6) {
            std::cerr << "Warning: Skipping malformed record at line "
                      << lineNumber << ". Expected six fields.\n";
            continue;
        }

        // The answer must contain exactly one character.
        if (fields[5].size() != 1) {
            std::cerr << "Warning: Skipping malformed record at line "
                      << lineNumber << ". Invalid answer field.\n";
            continue;
        }

        Question q;
        q.text = fields[0];
        q.options = {
            fields[1],
            fields[2],
            fields[3],
            fields[4]
        };

        q.answer = static_cast<char>(
            std::toupper(static_cast<unsigned char>(fields[5][0]))
        );

        if (!isValidQuestion(q)) {
            std::cerr << "Warning: Skipping invalid question at line "
                      << lineNumber << ".\n";
            continue;
        }

        questions.push_back(q);
    }

    if (file.bad()) {
        std::cerr << "Error: Failed while reading question file: "
                  << filename << '\n';
    }

    return questions;
}
