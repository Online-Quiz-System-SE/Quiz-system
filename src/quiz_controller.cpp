#include "quiz_controller.h"

#include <algorithm>
#include <cctype>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

QuizController::QuizController(
    QuestionLoader loader,
    AdminAction admin,
    std::istream& in,
    std::ostream& out
)
    : loader_(std::move(loader)),
      admin_(std::move(admin)),
      in_(in),
      out_(out) {}

void QuizController::showMenu() const {
    out_ << "\n=== Online Quiz System ===\n"
         << "1. Admin login\n"
         << "2. Take Quiz\n"
         << "0. Exit\n"
         << "Choose an option: ";
}

void QuizController::run() {
    std::string line;

    while (true) {
        showMenu();

        if (!std::getline(in_, line)) {
            return;
        }

        if (line == "0") {
            out_ << "Goodbye!\n";
            return;
        } else if (line == "1") {
            if (admin_) {
                admin_();
            }
        } else if (line == "2") {
            startQuiz();
        } else {
            out_ << "Invalid menu choice.\n";
        }
    }
}

std::string QuizController::selectTopic() {
    out_ << "\n=== Select a Quiz Topic ===\n"
         << "1. Machine Learning (ML)\n"
         << "2. Database Management Systems (DBMS)\n"
         << "3. Software Engineering (SE)\n"
         << "4. Data Structures and Algorithms (DSA)\n"
         << "5. General Knowledge (GK)\n"
         << "0. Return to Main Menu\n"
         << "Choose a topic: ";

    std::string choice;

    if (!std::getline(in_, choice)) {
        return "";
    }

    if (choice == "1") return "ML";
    if (choice == "2") return "DBMS";
    if (choice == "3") return "SE";
    if (choice == "4") return "DSA";
    if (choice == "5") return "GK";

    if (choice == "0") {
        return "";
    }

    out_ << "Invalid topic selection.\n";
    return "INVALID";
}

std::vector<Question> QuizController::selectRandomQuestions(
    const std::vector<Question>& questions,
    const std::string& topic
) {
    std::vector<Question> topicQuestions;

    for (const Question& question : questions) {
        if (question.topic == topic) {
            topicQuestions.push_back(question);
        }
    }

    if (topicQuestions.size() < 5) {
        return {};
    }

    static std::mt19937 generator(std::random_device{}());

    std::shuffle(
        topicQuestions.begin(),
        topicQuestions.end(),
        generator
    );

    std::vector<Question> selected(
        topicQuestions.begin(),
        topicQuestions.begin() + 5
    );

    std::vector<std::string> currentSet;

    for (const Question& question : selected) {
        currentSet.push_back(question.text);
    }

    std::sort(currentSet.begin(), currentSet.end());

    // Avoid repeating the immediately previous set when possible.
    if (currentSet == previousQuestionSet_ &&
        topicQuestions.size() > 5) {

        for (int attempt = 0; attempt < 10; ++attempt) {
            std::shuffle(
                topicQuestions.begin(),
                topicQuestions.end(),
                generator
            );

            selected.assign(
                topicQuestions.begin(),
                topicQuestions.begin() + 5
            );

            currentSet.clear();

            for (const Question& question : selected) {
                currentSet.push_back(question.text);
            }

            std::sort(currentSet.begin(), currentSet.end());

            if (currentSet != previousQuestionSet_) {
                break;
            }
        }
    }

    previousQuestionSet_ = currentSet;

    return selected;
}

void QuizController::startQuiz() {
    std::vector<Question> bank =
        loader_ ? loader_() : std::vector<Question>{};

    if (bank.empty()) {
        out_ << "No valid questions could be loaded.\n";
        return;
    }

    while (true) {
        std::string topic = selectTopic();

        if (topic.empty()) {
            return;
        }

        if (topic == "INVALID") {
            continue;
        }

        previousQuestionSet_.clear();

        while (true) {
            questions_ = selectRandomQuestions(bank, topic);

            if (questions_.size() != 5) {
                out_ << "Not enough valid questions for topic "
                     << topic << ". At least five are required.\n";
                break;
            }

            if (!runQuizAttempt(topic)) {
                return;
            }

            out_ << "\nWhat would you like to do?\n"
                 << "1. Retry this topic\n"
                 << "2. Return to topic selection\n"
                 << "0. Return to main menu\n"
                 << "Choose an option: ";

            std::string choice;

            if (!std::getline(in_, choice)) {
                return;
            }

            if (choice == "1") {
                continue;
            }

            if (choice == "2") {
                break;
            }

            if (choice == "0") {
                return;
            }

            out_ << "Invalid menu choice.\n";
        }
    }
}

bool QuizController::runQuizAttempt(const std::string& topic) {
    score_ = 0;

    QuizLogic logic;
    logic.resetScore();

    out_ << "\nWelcome to the Online Quiz System!\n"
         << "Topic: " << topic << '\n'
         << "You will answer five randomly selected questions.\n";

    for (std::size_t i = 0; i < questions_.size(); ++i) {
        showQuestion(i);

        char choice = readChoice();

        if (choice == '\0') {
            return false;
        }

        const Question& question = questions_[i];

        if (logic.checkAnswer(question, choice)) {
            out_ << "\nCorrect! Your answer is right.\n";
        } else {
            out_ << "\nIncorrect!\n";
        }

        // Display the correct answer after every response.
        char correctAnswer = static_cast<char>(
            std::toupper(
                static_cast<unsigned char>(question.answer)
            )
        );

        out_ << "Correct answer: " << correctAnswer;

        if (correctAnswer >= 'A' && correctAnswer <= 'D' &&
            question.options.size() >= 4) {
            out_ << ". "
                 << question.options[
                        static_cast<std::size_t>(
                            correctAnswer - 'A'
                        )
                    ];
        }

        out_ << '\n';

        logic.updateScore(question, choice);
        score_ = logic.getScore();
    }

    out_ << "\nQuiz completed!\n"
         << "Your score: " << score_ << " / "
         << questions_.size() << '\n';

    return true;
}

void QuizController::showQuestion(std::size_t i) const {
    const Question& question = questions_.at(i);

    out_ << "\nQuestion " << (i + 1)
         << " of " << questions_.size()
         << ": " << question.text << '\n';

    const char labels[] = {'A', 'B', 'C', 'D'};

    for (std::size_t k = 0;
         k < question.options.size() && k < 4;
         ++k) {
        out_ << "  " << labels[k] << ". "
             << question.options[k] << '\n';
    }
}

char QuizController::readChoice() {
    std::string line;

    while (true) {
        out_ << "Your answer (A-D): ";

        if (!std::getline(in_, line)) {
            return '\0';
        }

        std::string cleaned;

        for (char ch : line) {
            if (!std::isspace(static_cast<unsigned char>(ch))) {
                cleaned += ch;
            }
        }

        if (cleaned.size() == 1) {
            char choice = static_cast<char>(
                std::toupper(
                    static_cast<unsigned char>(cleaned[0])
                )
            );

            if (choice >= 'A' && choice <= 'D') {
                return choice;
            }
        }

        out_ << "Invalid option. Enter A, B, C, or D.\n";
    }
}