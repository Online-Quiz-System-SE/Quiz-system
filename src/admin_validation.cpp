#include "admin_validation.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>

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

bool modifyQuestion(const std::string& filePath,
                    std::size_t questionIndex,
                    const Question& updatedQuestion) {
    if (!isValidQuestion(updatedQuestion)) {
        return false;
    }

    std::ifstream input(filePath);
    if (!input.is_open()) {
        return false;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    if (input.bad()) {
        return false;
    }
    input.close();

    if (questionIndex >= lines.size()) {
        return false;
    }

    std::ostringstream outputLine;
    outputLine << updatedQuestion.text;

    for (const auto& option : updatedQuestion.options) {
        outputLine << '|' << option;
    }

    char answer = static_cast<char>(
        std::toupper(
            static_cast<unsigned char>(updatedQuestion.answer)
        )
    );

    outputLine << '|' << answer;
    lines[questionIndex] = outputLine.str();

    std::ofstream output(filePath, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    for (const auto& savedLine : lines) {
        output << savedLine << '\n';
    }

    return output.good();
}
