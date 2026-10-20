#include "quiz_controller.h"
#include <string>
#include <utility>

QuizController::QuizController(std::vector<Question> q, std::istream& in, std::ostream& out)
    : questions_(std::move(q)), in_(in), out_(out) {}

void QuizController::showMenu() const {
    out_ << "\n===== Online Quiz System =====\n"
         << "1. Start Quiz\n2. Admin\n3. Exit\n"
         << "Enter choice: ";
}

void QuizController::run() {
    std::string line;
    while (true) {
        showMenu();
        if (!std::getline(in_, line)) return;
        if (line == "1")      startQuiz();
        else if (line == "2") out_ << "Admin login coming in a later story.\n";
        else if (line == "3") { out_ << "Goodbye!\n"; return; }
        else                  out_ << "Invalid choice. Enter 1, 2 or 3.\n";
    }
}

void QuizController::startQuiz() {
    score_ = 0;
    current_ = 0;
    if (questions_.empty()) {
        out_ << "No questions available.\n";
        return;
    }
    out_ << "\nNew quiz started. Score: 0\n";
    showQuestion(current_);
}

void QuizController::showQuestion(std::size_t i) const {
    const Question& q = questions_.at(i);
    out_ << "\nQuestion " << (i + 1) << " of " << questions_.size() << ": " << q.text << '\n';
    const char labels[] = {'A', 'B', 'C', 'D'};
    for (std::size_t k = 0; k < q.options.size() && k < 4; ++k)
        out_ << "  " << labels[k] << ". " << q.options[k] << '\n';
}
