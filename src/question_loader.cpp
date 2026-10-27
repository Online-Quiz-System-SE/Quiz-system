#include "question_loader.h"
#include "admin_validation.h"

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

        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() != 6) {
            std::cerr << "Warning: Skipping malformed record at line "
                      << lineNumber
                      << ". Expected six fields.\n";
            continue;
        }

        if (fields[5].size() != 1) {
            std::cerr << "Warning: Skipping malformed record at line "
                      << lineNumber
                      << ". Invalid answer field.\n";
            continue;
        }

        Question q;
        q.text = fields[0];
        q.options = {fields[1], fields[2], fields[3], fields[4]};
        q.answer = fields[5][0];

        if (isValidQuestion(q)) {
            questions.push_back(q);
        } else {
            std::cerr << "Warning: Skipping invalid question at line "
                      << lineNumber << ".\n";
        }
    }

    if (file.bad()) {
        std::cerr << "Error: Failed while reading question file: "
                  << filename << '\n';
    }

    return questions;
}