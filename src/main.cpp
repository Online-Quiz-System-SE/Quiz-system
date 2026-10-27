
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <limits>

#include "question_loader.h"
#include "quiz_logic.h"
#include "quiz_controller.h"
#include "admin_validation.h"

const std::string QUESTION_FILE = "data/questions.txt";
const std::string ADMIN_USERNAME = "admin";
const std::string ADMIN_PASSWORD = "admin123";

// Convert a string to uppercase.
std::string upperCase(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(
            std::toupper(static_cast<unsigned char>(ch))
        );
    }

    return text;
}

// Load questions from the question bank.
std::vector<Question> loadQuestionsFromFile() {
    std::vector<Question> questions;
    std::ifstream file(QUESTION_FILE);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open "
                  << QUESTION_FILE << '\n';
        return questions;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() != 6 || fields[5].size() != 1) {
            std::cerr << "Skipping malformed question record.\n";
            continue;
        }

        Question question;
        question.text = fields[0];

        question.options = {
            fields[1],
            fields[2],
            fields[3],
            fields[4]
        };

        question.answer = upperCase(fields[5])[0];

        if (isValidQuestion(question)) {
            questions.push_back(question);
        } else {
            std::cerr << "Skipping invalid question record.\n";
        }
    }

    return questions;
}

// Save all questions to the question bank.
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

        file << question.text;

        for (const std::string& option : question.options) {
            file << '|' << option;
        }

        file << '|' << upperCase(
            std::string(1, question.answer)
        ) << '\n';
    }

    return file.good();
}

// Authenticate the administrator.
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

// Read and validate a question.
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
        std::cout << "Invalid question. Check the text, options, "
                     "and correct answer.\n";
        return false;
    }

    // The file format uses | as a field separator.
    if (question.text.find('|') != std::string::npos) {
        std::cout << "Question text cannot contain the | character.\n";
        return false;
    }

    for (const std::string& option : question.options) {
        if (option.find('|') != std::string::npos) {
            std::cout << "Options cannot contain the | character.\n";
            return false;
        }
    }

    return true;
}

// Add a question.
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

// Read and validate a question number.
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

// Display all available questions.
void displayQuestions(const std::vector<Question>& questions) {
    for (std::size_t i = 0; i < questions.size(); ++i) {
        std::cout << i + 1 << ". "
                  << questions[i].text << '\n';
    }
}

// Modify a question.
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

// Delete a question.
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

    questions.erase(
        questions.begin() + (number - 1)
    );

    if (saveAllQuestions(questions)) {
        std::cout << "Question deleted successfully.\n";
    } else {
        std::cout << "Error: Could not save the changes.\n";
    }
}

// Administrator menu.
void adminMenu() {
    while (true) {
        std::cout << "\n--- Admin Menu ---\n"
                  << "1. Add question\n"
                  << "2. Modify question\n"
                  << "3. Delete question\n"
                  << "4. Logout\n"
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
            std::cout << "Admin logged out.\n";
            return;
        } else {
            std::cout << "Invalid menu option.\n";
        }
    }
}

// Main application entry point.
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
