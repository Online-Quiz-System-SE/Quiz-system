#include "admin_validation.h"

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
