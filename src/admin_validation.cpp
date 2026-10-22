#include "admin_validation.h"
#include <sstream>
#include <cctype>
#include <fstream>

bool isValidQuestion(const Question& question) {
    if (question.text.find_first_not_of(" \t\r\n") == std::string::npos) {
        return false;
    }

    if (question.options.size() != 4) {
        return false;
    }

    for (const auto& option : question.options) {
        if (option.find_first_not_of(" \t\r\n") == std::string::npos) {
            return false;
        }
    }

    char answer = static_cast<char>(
        std::toupper(static_cast<unsigned char>(question.answer))
    );

    return answer >= 'A' && answer <= 'D';
}

bool saveQuestion(const Question& question, const std::string& filePath) {
    if (!isValidQuestion(question)) {
        return false;
    }

    std::ofstream file(filePath, std::ios::app);

    if (!file.is_open()) {
        return false;
    }

    file << question.text;

    for (const auto& option : question.options) {
        file << '|' << option;
    }

    char answer = static_cast<char>(
        std::toupper(static_cast<unsigned char>(question.answer))
    );

    file << '|' << answer << '\n';

    return file.good();
}

bool loadQuestions(std::vector<Question>& questions,
                   const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return false;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::string> fields;
        std::string field;
        std::istringstream stream(line);

        while (std::getline(stream, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() != 6) {
            continue;
        }

        Question question;
        question.text = fields[0];

        for (int i = 1; i <= 4; ++i) {
            question.options.push_back(fields[i]);
        }

        if (fields[5].size() != 1) {
            continue;
        }

        question.answer = fields[5][0];

        if (isValidQuestion(question)) {
            questions.push_back(question);
        }
    }

    return !file.bad();
}
