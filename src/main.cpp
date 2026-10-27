
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

#include "quiz_logic.h"
#include "quiz_controller.h"
#include "admin_validation.h"
const std::string QUESTION_FILE = "data/questions.txt";

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

        // Ignore empty lines.
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        // Each record must contain a question, four options,
        // and one correct answer.
        if (fields.size() != 6) {
            std::cerr << "Warning: Skipping malformed record at line "
                      << lineNumber
                      << ". Expected six fields.\n";
            continue;
        }

        // The answer must contain exactly one character.
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
}}