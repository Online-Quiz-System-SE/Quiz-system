#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <limits>
#include <sys/stat.h>

#include "question_loader.h"
#include "quiz_logic.h"
#include "quiz_controller.h"
#include "admin_validation.h"

const std::string QUESTION_FILE = "data/questions.txt";
const std::string ADMIN_USERNAME = "admin";
const std::string ADMIN_PASSWORD = "admin123";

std::string upperCase(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(
            std::toupper(static_cast<unsigned char>(ch))
        );
    }
    return text;
}

std::vector<Question> loadQuestionsFromFile() {
    return loadQuestions(QUESTION_FILE);
}

bool saveAllQuestions(const std::vector<Question>& questions) {
    std::ofstream file(QUESTION_FILE, std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open question file for writing.\n";
        return false;
    }

    for (const Question& question : questions) {
        if (!isValidQuestion(question)) {
            continue;
        }

        if (question.text.find('|') != std::string::npos) {
            std::cerr << "Error: Question text cannot contain '|'.\n";
            return false;
        }

        file << question.text;

        for (const std::string& option : question.options) {
            if (option.find('|') != std::string::npos) {
                std::cerr << "Error: Options cannot contain '|'.\n";
                return false;
            }
            file << '|' << option;
        }

        file << '|' << upperCase(
            std::string(1, question.answer)
        ) << '\n';
    }

    return file.good();
}

bool adminLogin() {
    std::string username;
    std::string password;

    std::cout << "\n--- Admin Login ---\n";
    std::cout << "Username: ";

    if (!std::getline(std::cin, username)) {
        return false;
    }

    std::cout << "Password: ";

    if (!std::getline(std::cin, password)) {
        return false;
    }

    if (username == ADMIN_USERNAME &&
        password == ADMIN_PASSWORD) {
        std::cout << "Admin login successful.\n";
        return true;
    }

    std::cout << "Access denied. Invalid credentials.\n";
    return false;
}

bool readQuestion(Question& question) {
    std::string answer;

    std::cout << "Enter question text: ";

    if (!std::getline(std::cin, question.text)) {
        return false;
    }

    question.options.resize(4);

    for (int i = 0; i < 4; ++i) {
        std::cout << "Enter option "
                  << static_cast<char>('A' + i) << ": ";

        if (!std::getline(std::cin, question.options[i])) {
            return false;
        }
    }

    std::cout << "Enter correct answer (A-D): ";

    if (!std::getline(std::cin, answer)) {
        return false;
    }

    if (answer.size() != 1) {
        std::cout << "Invalid answer. Enter A, B, C, or D.\n";
        return false;
    }

    question.answer = upperCase(answer)[0];

    if (!isValidQuestion(question)) {
        std::cout << "Invalid question. Check the question text, "
                     "options, and answer.\n";
        return false;
    }

    if (question.text.find('|') != std::string::npos) {
        std::cout << "Question text cannot contain '|'.\n";
        return false;
    }

    for (const std::string& option : question.options) {
        if (option.find('|') != std::string::npos) {
            std::cout << "Options cannot contain '|'.\n";
            return false;
        }
    }

    return true;
}

void addQuestion() {
    Question question;

    if (!readQuestion(question)) {
        return;
    }


    if (saveQuestion(question, QUESTION_FILE)) {
        std::cout << "Question added successfully.\n";
    } else {
        std::cout << "Error: Could not save the question.\n";
    }
}

int readQuestionNumber(std::size_t count) {
    std::string input;

    if (!std::getline(std::cin, input)) {
        return -1;
    }

    try {
        std::size_t position = 0;
        int number = std::stoi(input, &position);

        if (position != input.size() ||
            number < 1 ||
            static_cast<std::size_t>(number) > count) {
            std::cout << "Invalid question number.\n";
            return -1;
        }

        return number;
    } catch (...) {
        std::cout << "Invalid question number.\n";
        return -1;
    }
}

void displayQuestions(const std::vector<Question>& questions) {
    std::cout << "\n--- Question Bank ---\n";

    for (std::size_t i = 0; i < questions.size(); ++i) {
        std::cout << i + 1 << ". "
                  << questions[i].text << '\n';
    }
}

void modifyQuestionInBank() {
    std::vector<Question> questions = loadQuestionsFromFile();

    if (questions.empty()) {
        std::cout << "No questions available to modify.\n";
        return;
    }

    displayQuestions(questions);

    std::cout << "Enter question number to modify: ";
    int number = readQuestionNumber(questions.size());

    if (number < 1) {
        return;
    }

    Question updatedQuestion;

    std::cout << "\nEnter the updated question details:\n";

    if (!readQuestion(updatedQuestion)) {
        return;
    }

    questions[static_cast<std::size_t>(number - 1)] =
        updatedQuestion;

    if (saveAllQuestions(questions)) {
        std::cout << "Question modified successfully.\n";
    } else {
        std::cout << "Error: Could not save the changes.\n";
    }
}

void deleteQuestionFromBank() {
    std::vector<Question> questions = loadQuestionsFromFile();

    if (questions.empty()) {
        std::cout << "No questions available to delete.\n";
        return;
    }

    displayQuestions(questions);

    std::cout << "Enter question number to delete: ";
    int number = readQuestionNumber(questions.size());

    if (number < 1) {
        return;
    }

    std::cout << "Are you sure you want to delete question "
              << number << "? (y/n): ";

    std::string confirmation;

    if (!std::getline(std::cin, confirmation)) {
        return;
    }

    if (confirmation != "y" && confirmation != "Y") {
        std::cout << "Deletion cancelled.\n";
        return;
    }

    questions.erase(questions.begin() + (number - 1));

    if (saveAllQuestions(questions)) {
        std::cout << "Question deleted successfully.\n";
    } else {
        std::cout << "Error: Could not save the changes.\n";
    }
}

void adminMenu() {
    while (true) {
        std::cout << "\n--- Admin Menu ---\n"
                  << "1. Add question\n"
                  << "2. Modify question\n"
                  << "3. Delete question\n"
                  << "4. View questions\n"
                  << "5. Logout\n"
                  << "Enter choice: ";

        std::string input;

        if (!std::getline(std::cin, input)) {
            return;
        }

        if (input == "1") {
            addQuestion();
        } else if (input == "2") {
            modifyQuestionInBank();
        } else if (input == "3") {
            deleteQuestionFromBank();
        } else if (input == "4") {
            displayQuestions(loadQuestionsFromFile());
        } else if (input == "5") {
            std::cout << "Admin logged out.\n";
            return;
        } else {
            std::cout << "Invalid menu option.\n";
        }
    }
}

int main() {
  

    auto loader = []() -> std::vector<Question> {
        return loadQuestionsFromFile();
    };

    auto adminAction = []() {
        if (adminLogin()) {
            adminMenu();
        }
    };

    QuizController controller(loader, adminAction);
    controller.run();

    return 0;
}
