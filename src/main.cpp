#include "quiz_controller.h"

int main() {
    // Temporary: swap for the questions.txt loader once QZ-03 (#4) is merged
    std::vector<Question> questions = {
        {"Which data structure is FIFO?", {"Stack", "Queue", "Tree", "Graph"}, 'B'},
        {"Which keyword declares a constant in C++?", {"static", "volatile", "const", "mutable"}, 'C'},
    };
    QuizController(questions).run();
    return 0;
}
