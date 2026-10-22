
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>

#include "quiz_logic.h"
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

std::vector<Question> loadQuestions() {
    std::vector<Question> questions;
    std::ifstream file(QUESTION_FILE);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() != 6 || fields[5].empty()) {
            continue;
        }

        Question q;
        q.text = fields[0];
        q.options = {fields[1], fields[2], fields[3], fields[4]};
        q.answer = upperCase(fields[5])[0];

        if (isValidQuestion(q)) {
            questions.push_back(q);
        }
    }

    return questions;
}

bool saveAllQuestions(const std::vector<Question>& questions) {
    std::ofstream file(QUESTION_FILE, std::ios::trunc);

    if (!file.is_open()) {
        std::cout << "Error: Could not open question file.\n";
        return false;
    }

    for (const Question& q : questions) {
        if (!isValidQuestion(q)) {
            continue;
        }

        file << q.text;
        for (const std::string& option : q.options) {
            file << '|' << option;
        }
        file << '|' << upperCase(std::string(1, q.answer)) << '\n';
    }

    return file.good();
}

bool adminLogin() {
    std::string username;
    std::string password;

    std::cout << "Admin username: ";
    if (!std::getline(std::cin, username)) return false;

    std::cout << "Admin password: ";
    if (!std::getline(std::cin, password)) return false;

    if (username == ADMIN_USERNAME &&
        password == ADMIN_PASSWORD) {
        std::cout << "Admin login successful.\n";
        return true;
    }

    std::cout << "Access denied. Admin login required.\n";
    return false;
}

bool readQuestion(Question& q) {
    std::string answer;

    std::cout << "Enter question text: ";
    if (!std::getline(std::cin, q.text)) return false;

    q.options.resize(4);

    for (int i = 0; i < 4; ++i) {
        std::cout << "Enter option " << char('A' + i) << ": ";
        if (!std::getline(std::cin, q.options[i])) return false;
    }

    std::cout << "Enter correct answer (A-D): ";
    if (!std::getline(std::cin, answer)) return false;

    if (answer.size() != 1) {
        std::cout << "Invalid answer. Use A, B, C, or D.\n";
        return false;
    }

    q.answer = upperCase(answer)[0];

    if (!isValidQuestion(q)) {
        std::cout << "Invalid question. Please check the text, options, and answer.\n";
        return false;
    }

    return true;
}

void addQuestion() {
    Question q;

    if (!readQuestion(q)) return;

    // saveQuestion validates the question and appends it to the file.
    if (saveQuestion(q, QUESTION_FILE)) {
        std::cout << "Question added successfully.\n";
    } else {
        std::cout << "Could not save question.\n";
    }
}

void modifyQuestion() {
    std::vector<Question> questions = loadQuestions();

    if (questions.empty()) {
        std::cout << "No questions available to modify.\n";
        return;
    }

    for (std::size_t i = 0; i < questions.size(); ++i) {
        std::cout << i + 1 << ". " << questions[i].text << '\n';
    }

    std::cout << "Enter question number to modify: ";
    std::string input;
    if (!std::getline(std::cin, input)) return;

    std::size_t pos = 0;
    int number;

    try {
        number = std::stoi(input, &pos);
        if (pos != input.size() ||
            number < 1 ||
            number > static_cast<int>(questions.size())) {
            throw std::invalid_argument("invalid number");
        }
    } catch (...) {
        std::cout << "Invalid question number.\n";
        return;
    }

    Question updated;
    std::cout << "Enter the updated question details:\n";

    if (!readQuestion(updated)) return;

    questions[number - 1] = updated;

    if (saveAllQuestions(questions)) {
        std::cout << "Question modified successfully.\n";
    }
}

void deleteQuestion() {
    std::vector<Question> questions = loadQuestions();

    if (questions.empty()) {
        std::cout << "No questions available to delete.\n";
        return;
    }

    for (std::size_t i = 0; i < questions.size(); ++i) {
        std::cout << i + 1 << ". " << questions[i].text << '\n';
    }

    std::cout << "Enter question number to delete: ";
    std::string input;
    if (!std::getline(std::cin, input)) return;

    std::size_t pos = 0;
    int number;

    try {
        number = std::stoi(input, &pos);
        if (pos != input.size() ||
            number < 1 ||
            number > static_cast<int>(questions.size())) {
            throw std::invalid_argument("invalid number");
        }
    } catch (...) {
        std::cout << "Invalid question number.\n";
        return;
    }

    std::cout << "Delete question " << number << "? (y/n): ";
    std::string confirm;
    if (!std::getline(std::cin, confirm)) return;

    if (confirm != "y" && confirm != "Y") {
        std::cout << "Deletion cancelled.\n";
        return;
    }

    questions.erase(questions.begin() + number - 1);

    if (saveAllQuestions(questions)) {
        std::cout << "Question deleted successfully.\n";
    }
}

void adminMenu() {
    while (true) {
        std::cout << "\n--- Admin Menu ---\n"
                  << "1. Add question\n"
                  << "2. Modify question\n"
                  << "3. Delete question\n"
                  << "4. Logout\n"
                  << "Enter choice: ";

        std::string input;
        if (!std::getline(std::cin, input)) return;

        if (input == "4") {
            std::cout << "Admin logged out.\n";
            return;
        } else if (input == "1") {
            addQuestion();
        } else if (input == "2") {
            modifyQuestion();
        } else if (input == "3") {
            deleteQuestion();
        } else {
            std::cout << "Invalid menu option.\n";
        }
    }
}

void runQuiz() {
    const std::vector<Question> questions = loadQuestions();

    if (questions.empty()) {
        std::cout << "No valid questions are available.\n";
        return;
    }

    QuizLogic logic;
    std::cout << "\nStarting quiz...\n";

    for (std::size_t i = 0; i < questions.size(); ++i) {
        const Question& q = questions[i];

        std::cout << "\nQuestion " << i + 1 << ": "
                  << q.text << '\n';

        for (std::size_t j = 0; j < q.options.size(); ++j) {
            std::cout << char('A' + j) << ". "
                      << q.options[j] << '\n';
        }

        while (true) {
            std::cout << "Your answer (A-D): ";
            std::string input;
            if (!std::getline(std::cin, input)) return;

            if (input.size() == 1) {
                char choice = upperCase(input)[0];

                if (choice >= 'A' && choice <= 'D') {
                    logic.updateScore(q, choice);
                    break;
                }
            }

            std::cout << "Invalid option. Please enter A, B, C, or D.\n";
        }
    }

    std::cout << "\nQuiz completed!\nYour score: "
              << logic.getScore() << " / "
              << questions.size() << '\n';
}

int main() {
    std::cout << "Welcome to the Online Quiz System!\n";

    while (true) {
        std::cout << "\n--- Main Menu ---\n"
                  << "1. Start quiz\n"
                  << "2. Admin login\n"
                  << "3. Exit\n"
                  << "Enter choice: ";

        std::string input;
        if (!std::getline(std::cin, input)) return 0;

        if (input == "1") {
            runQuiz();
        } else if (input == "2") {
            if (adminLogin()) {
                adminMenu();
            }
        } else if (input == "3") {
            std::cout << "Goodbye!\n";
            return 0;
        } else {
            std::cout << "Invalid menu option.\n";
        }
    }
}
