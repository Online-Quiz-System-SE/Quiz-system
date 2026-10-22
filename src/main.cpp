
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <cctype>

#include "admin_validation.h"
#include "quiz_logic.h"

const std::string QUESTION_FILE = "data/questions.txt";

void addQuestion() {
    Question question;
    question.options.resize(4);

    std::cout << "Enter question text: ";
    std::getline(std::cin, question.text);

    const char labels[] = {'A', 'B', 'C', 'D'};

    for (int i = 0; i < 4; ++i) {
        std::cout << "Enter option " << labels[i] << ": ";
        std::getline(std::cin, question.options[i]);
    }

    std::string answer;
    std::cout << "Enter correct answer (A/B/C/D): ";
    std::getline(std::cin, answer);

    if (answer.size() != 1) {
        std::cout << "Invalid answer. Enter A, B, C, or D.\n";
        return;
    }

    question.answer = static_cast<char>(
        std::toupper(static_cast<unsigned char>(answer[0]))
    );

    if (!isValidQuestion(question)) {
        std::cout << "Invalid question. Check the text, options, and answer.\n";
        return;
    }

    if (saveQuestion(question, QUESTION_FILE)) {
        std::cout << "Question added successfully!\n";
    } else {
        std::cout << "Could not save the question.\n";
    }
}

void takeQuiz() {
    std::vector<Question> questions;

    if (!loadQuestions(questions, QUESTION_FILE)) {
        std::cout << "Could not read the question file.\n";
        return;
    }

    if (questions.empty()) {
        std::cout << "No valid questions are available.\n";
        return;
    }

    QuizLogic logic;

    for (const Question& question : questions) {
        std::cout << "\n" << question.text << "\n";

        for (std::size_t i = 0; i < question.options.size(); ++i) {
            std::cout << static_cast<char>('A' + i)
                      << ". " << question.options[i] << "\n";
        }

        std::cout << "Your answer (A/B/C/D): ";
        char answer;
        std::cin >> answer;
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

        logic.updateScore(question, answer);
    }

    std::cout << "\nQuiz complete! Your score: "
              << logic.getScore() << "/" << questions.size() << "\n";
}

int main() {
    while (true) {
        std::cout << "\nOnline Quiz System\n";
        std::cout << "1. Admin: Add a question\n";
        std::cout << "2. Take Quiz\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice)) {
            std::cout << "Invalid menu choice.\n";
            return 1;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            addQuestion();
        } else if (choice == 2) {
            takeQuiz();
        } else {
            std::cout << "Invalid menu choice.\n";
        }
    }

    return 0;
}
