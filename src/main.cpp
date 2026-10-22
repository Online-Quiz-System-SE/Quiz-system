#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

#include "quiz_logic.h"

std::vector<Question> loadQuestions(const std::string& filename) {
    std::vector<Question> questions;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() != 6) {
            continue;
        }

        Question q;
        q.text = fields[0];
        q.options = {fields[1], fields[2], fields[3], fields[4]};
        q.answer = fields[5][0];

        questions.push_back(q);
    }

    return questions;
}

int main() {
    const std::string filename = "data/questions.txt";
    std::vector<Question> questions = loadQuestions(filename);

    if (questions.empty()) {
        std::cerr << "Error: No valid questions could be loaded.\n";
        return 1;
    }

    QuizLogic logic;

    std::cout << "Welcome to the Online Quiz System!\n\n";

    for (std::size_t i = 0; i < questions.size(); ++i) {
        const Question& q = questions[i];

        std::cout << "Question " << i + 1 << ": " << q.text << '\n';

        for (std::size_t j = 0; j < q.options.size(); ++j) {
            std::cout << static_cast<char>('A' + j)
                      << ". " << q.options[j] << '\n';
        }

        char choice;

        while (true) {
            std::cout << "Your answer (A-D): ";

            if (!(std::cin >> choice)) {
                std::cerr << "\nError reading input.\n";
                return 1;
            }

            choice = static_cast<char>(
                std::toupper(static_cast<unsigned char>(choice))
            );

            if (choice >= 'A' && choice <= 'D') {
                break;
            }

            std::cout << "Invalid option. Please enter A, B, C, or D.\n";
        }

        logic.updateScore(q, choice);
        std::cout << '\n';
    }

    // F-010: Calculate the final score.
    const int finalScore = logic.getScore();
    const int totalQuestions = static_cast<int>(questions.size());

    // F-011: Display the final score.
    std::cout << "Quiz completed!\n";
    std::cout << "Your score: " << finalScore
              << " / " << totalQuestions << '\n';

    return 0;
}
