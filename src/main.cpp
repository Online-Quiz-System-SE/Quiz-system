
#include <iostream>
#include <string>
#include <vector>

#include "question_loader.h"
#include "quiz_logic.h"
#include "quiz_controller.h"
#include "admin_validation.h"

const std::string QUESTION_FILE = "data/questions.txt";

int main() {
    // Load questions from the question bank.
    auto loader = []() -> std::vector<Question> {
        return loadQuestions(QUESTION_FILE);
    };

    // Admin action placeholder.
    // Replace this with your existing admin menu function if available.
    auto adminAction = []() {
        std::cout << "Admin functionality is not configured yet.\n";
    };

    // Create the quiz controller and start the menu.
    QuizController controller(loader, adminAction);
    controller.run();

    return 0;
}
