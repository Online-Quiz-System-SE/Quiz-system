
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "quiz_logic.h"
#include "quiz_controller.h"
#include "admin_validation.h"

const std::string QUESTION_FILE = "data/questions.txt";
const std::string ADMIN_USERNAME = "admin";
const std::string ADMIN_PASSWORD = "admin123";

// Read the password while displaying * instead of actual characters.
std::string readPasswordMasked() {
    std::string password;
    std::cout << "Admin password: " << std::flush;

#ifdef _WIN32
    while (true) {
        int input = _getch();

        if (input == '\r') {
            break;
        }

        // Ignore special keys such as arrow keys.
        if (input == 0 || input == 224) {
            _getch();
            continue;
        }

        if (input == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else if (input >= 32 && input <= 126) {
            password += static_cast<char>(input);
            std::cout << '*' << std::flush;
        }
    }
#else
    termios oldSettings{};

    if (tcgetattr(STDIN_FILENO, &oldSettings) == -1) {
        std::getline(std::cin, password);
        return password;
    }

    termios newSettings = oldSettings;
    newSettings.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON));

    if (tcsetattr(STDIN_FILENO, TCSANOW, &newSettings) == -1) {
        std::getline(std::cin, password);
        return password;
    }

    while (true) {
        int input = getchar();

        if (input == EOF || input == '\n' || input == '\r') {
            break;
        }

        if (input == 127 || input == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else if (input >= 32 && input <= 126) {
            password += static_cast<char>(input);
            std::cout << '*' << std::flush;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
#endif

    std::cout << '\n';
    return password;
}

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

    if (!file.is_open()) {
        return questions;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() != 6 || fields[5].size() != 1) {
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
    if (!std::getline(std::cin, username)) {
        return false;
    }

    password = readPasswordMasked();

    // Reject empty or whitespace-only credentials.
    if (!areCredentialsNonEmpty(username, password)) {
        std::cout << "Username and password cannot be empty.\n";
        return false;
    }

    if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
        std::cout << "Admin login successful.\n";
        return true;
    }

    std::cout << "Access denied. Admin login required.\n";
    return false;
}

bool readQuestion(Question& q) {
    std::string answer;

    std::cout << "Enter question text: ";
    if (!std::getline(std::cin, q.text)) {
        return false;
    }

    q.options.resize(4);

    for (int i = 0; i < 4; ++i) {
        std::cout << "Enter option " << char('A' + i) << ": ";

        if (!std::getline(std::cin, q.options[i])) {
            return false;
        }
    }

    std::cout << "Enter correct answer (A-D): ";

    if (!std::getline(std::cin, answer)) {
        return false;
    }

    if (answer.size() != 1) {
        std::cout << "Invalid answer. Use A, B, C, or D.\n";
        return false;
    }

    q.answer = upperCase(answer)[0];

    if (!isValidQuestion(q)) {
        std::cout
            << "Invalid question. Please check the text, options, and answer.\n";
        return false;
    }

    return true;
}

void addQuestion() {
    Question q;

    if (!readQuestion(q)) {
        return;
    }

    if (saveQuestion(q, QUESTION_FILE)) {
        std::cout << "Question added successfully.\n";
    } else {
        std::cout << "Could not save question.\n";
    }
}

static int readQuestionNumber(std::size_t count) {
    std::string input;

    if (!std::getline(std::cin, input)) {
        return -1;
    }

    try {
        std::size_t pos = 0;
        int number = std::stoi(input, &pos);

        if (pos != input.size() ||
            number < 1 ||
            number > static_cast<int>(count)) {
            throw std::invalid_argument("invalid number");
        }

        return number;
    } catch (...) {
        std::cout << "Invalid question number.\n";
        return -1;
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

    int number = readQuestionNumber(questions.size());

    if (number < 0) {
        return;
    }

    Question updated;

    std::cout << "Enter the updated question details:\n";

    if (!readQuestion(updated)) {
        return;
    }

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

    int number = readQuestionNumber(questions.size());

    if (number < 0) {
        return;
    }

    std::cout << "Delete question " << number << "? (y/n): ";

    std::string confirm;

    if (!std::getline(std::cin, confirm)) {
        return;
    }

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

        if (!std::getline(std::cin, input)) {
            return;
        }

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

int main() {
    QuizController controller(
        [] { return loadQuestions(); },
        [] {
            if (adminLogin()) {
                adminMenu();
            }
        }
    );

    controller.run();
    return 0;
}
