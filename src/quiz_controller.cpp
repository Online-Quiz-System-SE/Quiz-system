#include "quiz_controller.h"
#include <cctype>
#include <string>
#include <utility>

QuizController::QuizController(QuestionLoader loader, AdminAction admin,
                               std::istream& in, std::ostream& out)
    : loader_(std::move(loader)), admin_(std::move(admin)), in_(in), out_(out) {}

void QuizController::showMenu() const {
    out_ << "\n=== Online Quiz System ===\n"
         << "1. Admin: Modify a question\n"
         << "2. Take Quiz\n"
         << "0. Exit\n"
         << "Choose an option: ";
}

void QuizController::run() {
    std::string line;
    while (true) {
        showMenu();
        if (!std::getline(in_, line)) return;
        if (line == "0")      return;
        else if (line == "1") { if (admin_) admin_(); }
        else if (line == "2") startQuiz();
        else                  out_ << "Invalid menu choice.\n";
    }
}

void QuizController::startQuiz() {
    score_ = 0;
    // Reload every time so admin edits are used immediately
    questions_ = loader_ ? loader_() : std::vector<Question>{};
    if (questions_.empty()) {
        out_ << "No valid questions could be loaded.\n";
        return;
    }

    QuizLogic logic;
    out_ << "\nWelcome to the Online Quiz System!\n";
    for (std::size_t i = 0; i < questions_.size(); ++i) {
        showQuestion(i);                     // one question at a time
        char choice = readChoice();
        if (choice == '\0') return;          // input ended
        logic.updateScore(questions_[i], choice);
        score_ = logic.getScore();
    }
    out_ << "\nQuiz completed!\n"
         << "Your score: " << score_ << " / " << questions_.size() << '\n';
}

void QuizController::showQuestion(std::size_t i) const {
    const Question& q = questions_.at(i);
    out_ << "\nQuestion " << (i + 1) << " of " << questions_.size() << ": " << q.text << '\n';
    const char labels[] = {'A', 'B', 'C', 'D'};
    for (std::size_t k = 0; k < q.options.size() && k < 4; ++k)
        out_ << "  " << labels[k] << ". " << q.options[k] << '\n';
}

char QuizController::readChoice() {
    std::string line;
    while (true) {
        out_ << "Your answer (A-D): ";
        if (!std::getline(in_, line)) return '\0';
        std::string s;
        for (char ch : line)
            if (!std::isspace(static_cast<unsigned char>(ch))) s += ch;
        if (s.size() == 1) {
            char c = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
            if (c >= 'A' && c <= 'D') return c;
        }
        out_ << "Invalid option. Enter A, B, C, or D.\n";
    }
}
