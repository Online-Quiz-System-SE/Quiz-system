#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <limits>

#include "quiz_logic.h"
#include "quiz_controller.h"
#include "admin_validation.h"

const std::string QUESTION_FILE = "data/questions.txt";

std::vector<Question> loadQuestions(const std::string& filename) {
    std::vector<Question> questions;
    std::ifstream file(filename);
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
        q.answer = fields[5][0];

        if (isValidQuestion(q)) {
            questions.push_back(q);
        }
    }

    return questions;
}

void modifyExistingQuestion() {
    std::vector<Question> questions = loadQuestions(QUESTION_FILE);

    if (questions.empty()) {
        std::cout << "No questions are available to modify.\n";
        return;
    }

    std::cout << "\n--- Modify a Question ---\n";

    for (std::size_t i = 0; i < questions.size(); ++i) {
        std::cout << i + 1 << ". " << questions[i].text << '\n';
    }

    std::cout << "Enter the question number to modify: ";

    std::size_t selection;

    if (!(std::cin >> selection)) {
        std::cout << "Invalid selection.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (selection < 1 || selection > questions.size()) {
        std::cout << "Question number out of range.\n";
        return;
    }

    Question updatedQuestion = questions[selection - 1];

    std::cout << "Current question: " << updatedQuestion.text << '\n';
    std::cout << "Enter updated question text: ";
    std::getline(std::cin, updatedQuestion.text);

    std::cout << "Current correct answer: "
              << static_cast<char>(std::toupper(
                     static_cast<unsigned char>(updatedQuestion.answer)))
              << '\n';

    std::string answer;
    std::cout << "Enter updated correct answer (A/B/C/D): ";
    std::getline(std::cin, answer);

    if (answer.size() != 1) {
        std::cout << "Invalid answer. Use A, B, C, or D.\n";
        return;
    }

    updatedQuestion.answer = static_cast<char>(
        std::toupper(static_cast<unsigned char>(answer[0])));

    if (!isValidQuestion(updatedQuestion)) {
        std::cout << "Invalid question text or answer. No changes were saved.\n";
        return;
    }

    if (modifyQuestion(QUESTION_FILE, selection - 1, updatedQuestion)) {
        std::cout << "Question updated successfully!\n";
    } else {
        std::cout << "Failed to update the question.\n";
    }
}
void deleteExistingQuestion() {
    std::vector<Question> questions = loadQuestions(QUESTION_FILE);

    if (questions.empty()) {
        std::cout << "No questions are available to delete.\n";
        return;
    }

    std::cout << "\n--- Delete a Question ---\n";

    for (std::size_t i = 0; i < questions.size(); ++i) {
        std::cout << i + 1 << ". " << questions[i].text << '\n';
    }

    std::cout << "Enter the question number to delete: ";

    std::size_t selection;

    if (!(std::cin >> selection)) {
        std::cout << "Invalid selection.\n";
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n');

    if (selection < 1 || selection > questions.size()) {
        std::cout << "Question number out of range.\n";
        return;
    }

    std::cout << "\nSelected question: "
              << questions[selection - 1].text << '\n';

    std::cout << "Are you sure you want to delete this question? (Y/N): ";

    std::string confirmation;
    std::getline(std::cin, confirmation);

    if (confirmation.size() != 1) {
        std::cout << "Invalid confirmation. No changes were made.\n";
        return;
    }

    char choice = static_cast<char>(
        std::toupper(static_cast<unsigned char>(confirmation[0])));

    if (choice == 'N') {
        std::cout << "Deletion cancelled. No changes were made.\n";
        return;
    }

    if (choice != 'Y') {
        std::cout << "Invalid confirmation. Enter Y or N.\n";
        return;
    }

    if (deleteQuestion(QUESTION_FILE, selection - 1)) {
        std::cout << "Question deleted successfully!\n";
    } else {
        std::cout << "Failed to delete the question.\n";
    }
}
int main() {
    QuizController controller(
        [] { return loadQuestions(QUESTION_FILE); },
        [] { modifyExistingQuestion(); });

    controller.setDeleteAction([] {
        deleteExistingQuestion();
    });

    controller.run();
    return 0;
}