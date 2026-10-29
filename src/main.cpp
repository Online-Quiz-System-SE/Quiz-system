#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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
    return loadQuestions(QUESTION_FILE);
}

// Save all questions to the question bank.
bool saveAllQuestions(const std::vector<Question>& questions) {
    // Validate all questions before opening the file for writing.
    for (const Question& question : questions) {
        if (!isValidQuestion(question)) {
            std::cerr << "Error: A question contains invalid data.\n";
            return false;
        }

        if (question.text.find('|') != std::string::npos) {
            std::cerr << "Error: Question text cannot contain '|'.\n";
            return false;
        }

        for (const std::string& option : question.options) {
            if (option.find('|') != std::string::npos) {
                std::cerr << "Error: Options cannot contain '|'.\n";
                return false;
            }
        }
    }

    std::ofstream file(QUESTION_FILE, std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the question bank for writing.\n";
        return false;
    }

    for (const Question& question : questions) {
        file << question.text;

        for (const std::string& option : question.options) {
            file << '|' << option;
        }

        file << '|'
             << upperCase(std::string(1, question.answer))
             << '\n';
    }

    if (!file.good()) {
        std::cerr << "Error: Failed while writing the question bank.\n";
        return false;
    }

    return true;
}

// Authenticate the administrator.
bool adminLogin() {
    std::string username;
    std::string password;

    std::cout << "\n========== ADMIN LOGIN ==========\n";

    std::cout << "Enter administrator username: ";

    if (!std::getline(std::cin, username)) {
        return false;
    }

    std::cout << "Enter administrator password: ";

    if (!std::getline(std::cin, password)) {
        return false;
    }

    if (username == ADMIN_USERNAME &&
        password == ADMIN_PASSWORD) {
        std::cout << "Admin login successful.\n";
        return true;
    }

    std::cout << "Access denied. Invalid username or password.\n";
    return false;
}

// Read and validate a new question.
bool readQuestion(Question& question) {
    std::string answer;

    std::cout << "\nEnter question text: ";

    if (!std::getline(std::cin, question.text)) {
        return false;
    }

    question.options.resize(4);

    for (int i = 0; i < 4; ++i) {
        std::cout << "Enter option "
                  << static_cast<char>('A' + i)
                  << " text: ";

        if (!std::getline(std::cin, question.options[i])) {
            return false;
        }
    }

    std::cout << "Enter the correct option letter (A, B, C, or D): ";

    if (!std::getline(std::cin, answer)) {
        return false;
    }

    if (answer.size() != 1) {
        std::cout
            << "Invalid answer. Enter exactly one letter: A, B, C, or D.\n";
        return false;
    }

    question.answer = upperCase(answer)[0];

    if (!isValidQuestion(question)) {
        std::cout
            << "Invalid question. The question and all four options "
               "must be non-empty, and the correct answer must be A, B, C, or D.\n";
        return false;
    }

    if (question.text.find('|') != std::string::npos) {
        std::cout
            << "The question text cannot contain the '|' character.\n";
        return false;
    }

    for (const std::string& option : question.options) {
        if (option.find('|') != std::string::npos) {
            std::cout
                << "Answer options cannot contain the '|' character.\n";
            return false;
        }
    }

    return true;
}

// Add a question to the question bank.
void addQuestion() {
    Question question;

    if (!readQuestion(question)) {
        std::cout << "Question was not added. Please check your input.\n";
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
            std::cout
                << "Invalid question number. Enter a number from 1 to "
                << count << ".\n";
            return -1;
        }

        return number;
    } catch (...) {
        std::cout
            << "Invalid input. Enter a whole number corresponding "
               "to a question in the list.\n";
        return -1;
    }
}

// Display all questions in the question bank.
void displayQuestions(const std::vector<Question>& questions) {
    std::cout << "\n========== QUESTION BANK ==========\n";

    if (questions.empty()) {
        std::cout << "The question bank is currently empty.\n";
        return;
    }

    for (std::size_t i = 0; i < questions.size(); ++i) {
        std::cout << i + 1 << ". "
                  << questions[i].text << '\n';
    }

    std::cout << "\nTotal questions: "
              << questions.size() << '\n';
}

// Modify an existing question.
void modifyQuestionInBank() {
    std::vector<Question> questions = loadQuestionsFromFile();

    if (questions.empty()) {
        std::cout << "No questions are available to modify.\n";
        return;
    }

    displayQuestions(questions);

    std::cout << "Enter the question number to modify (1-"
              << questions.size() << "): ";

    int number = readQuestionNumber(questions.size());

    if (number < 1) {
        return;
    }

    Question updatedQuestion;

    std::cout << "\nEnter the updated question details.\n";

    if (!readQuestion(updatedQuestion)) {
        std::cout
            << "Question modification cancelled. "
               "Please check your input and try again.\n";
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

// Delete a question after confirmation.
void deleteQuestionFromBank() {
    std::vector<Question> questions = loadQuestionsFromFile();

    if (questions.empty()) {
        std::cout << "No questions are available to delete.\n";
        return;
    }

    displayQuestions(questions);

    std::cout << "Enter the question number to delete (1-"
              << questions.size() << "): ";

    int number = readQuestionNumber(questions.size());

    if (number < 1) {
        return;
    }

    std::cout << "Are you sure you want to delete question "
              << number
              << "? Enter Y to confirm or N to cancel: ";

    std::string confirmation;

    if (!std::getline(std::cin, confirmation)) {
        return;
    }

    confirmation = upperCase(confirmation);

    if (confirmation != "Y") {
        std::cout << "Deletion cancelled. No questions were deleted.\n";
        return;
    }

    questions.erase(questions.begin() + (number - 1));

    if (saveAllQuestions(questions)) {
        std::cout << "Question deleted successfully.\n";
    } else {
        std::cout << "Error: Could not save the changes.\n";
    }
}

// Display and process administrator menu choices.
void adminMenu() {
    while (true) {
        std::cout
            << "\n========== ADMIN MENU ==========\n"
            << "1. Add a question\n"
            << "2. Modify an existing question\n"
            << "3. Delete a question\n"
            << "4. View all questions\n"
            << "5. Log out\n"
            << "Enter your menu choice (1-5): ";

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
            std::cout << "Admin logged out successfully.\n";
            return;
        } else {
            std::cout
                << "Invalid menu choice. Enter a number from 1 to 5.\n";
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