#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "quiz_logic.h"
#include "quiz_controller.h"
#include "admin_validation.h"

static std::vector<Question> sampleQuestions() {
    std::vector<Question> questions;

    for (int i = 1; i <= 15; ++i) {
        Question q{
            "ML Question " + std::to_string(i),
            {"Option A", "Option B", "Option C", "Option D"},
            'B'
        };
        q.topic = "ML";
        questions.push_back(q);
    }

    return questions;
}

static std::string runSession(
    const std::string& input,
    int* score = nullptr,
    bool* adminCalled = nullptr
) {
    std::istringstream in(input);
    std::ostringstream out;

    QuizController controller(
        [] { return sampleQuestions(); },
        [adminCalled] {
            if (adminCalled) {
                *adminCalled = true;
            }
        },
        in,
        out
    );

    controller.run();

    if (score) {
        *score = controller.score();
    }

    return out.str();
}

static bool has(const std::string& text, const std::string& part) {
    return text.find(part) != std::string::npos;
}

static bool endsWith(const std::string& text, const std::string& suffix) {
    return text.size() >= suffix.size() &&
           text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void test_check_answer() {
    QuizLogic logic;
    Question q{"Q", {"a", "b", "c", "d"}, 'B'};

    assert(logic.checkAnswer(q, 'B'));
    assert(!logic.checkAnswer(q, 'A'));
    assert(logic.checkAnswer(q, 'b'));
}

void test_answer_and_score() {
    QuizLogic logic;

    Question q1{"Q1", {"a", "b", "c", "d"}, 'B'};
    Question q2{"Q2", {"a", "b", "c", "d"}, 'C'};

    logic.updateScore(q1, 'B');
    logic.updateScore(q2, 'A');
    logic.updateScore(q2, 'C');

    assert(logic.getScore() == 2);

    logic.resetScore();
    assert(logic.getScore() == 0);
}

// QZ-15: Question validation
void test_question_validation() {
    Question valid{"What is 2 + 2?", {"1", "2", "4", "5"}, 'C'};
    assert(isValidQuestion(valid));

    Question emptyText{"   ", {"A", "B", "C", "D"}, 'A'};
    assert(!isValidQuestion(emptyText));

    Question wrongOptionCount{"Question?", {"A", "B", "C"}, 'A'};
    assert(!isValidQuestion(wrongOptionCount));

    Question emptyOption{"Question?", {"A", "", "C", "D"}, 'A'};
    assert(!isValidQuestion(emptyOption));

    Question invalidAnswer{"Question?", {"A", "B", "C", "D"}, 'E'};
    assert(!isValidQuestion(invalidAnswer));
}

// QZ-15: Saving a question
void test_save_question() {
    const std::string testFile = "test_questions_tmp.txt";
    std::remove(testFile.c_str());

    Question valid{"What is 2 + 2?", {"1", "2", "4", "5"}, 'C'};
    valid.topic = "GK";

    assert(saveQuestion(valid, testFile));

    {
        std::ifstream file(testFile);
        std::string savedLine;
        assert(std::getline(file, savedLine));
        assert(endsWith(savedLine, "What is 2 + 2?|1|2|4|5|C"));
    }

    // Invalid questions must not be saved.
    Question invalidAnswer{"Question?", {"A", "B", "C", "D"}, 'E'};
    assert(!saveQuestion(invalidAnswer, testFile));

    std::remove(testFile.c_str());
}

void test_topic_selection() {
    std::string out = runSession("2\n1\n0\n");

    assert(has(out, "Machine Learning (ML)"));
    assert(has(out, "Database Management Systems (DBMS)"));
    assert(has(out, "Software Engineering (SE)"));
    assert(has(out, "Data Structures and Algorithms (DSA)"));
    assert(has(out, "General Knowledge (GK)"));
    assert(has(out, "Topic: ML"));
}

void test_five_questions() {
    std::string out = runSession("2\n1\nB\nB\nB\nB\nB\n0\n");

    assert(has(out, "Question 1 of 5"));
    assert(has(out, "Question 5 of 5"));
    assert(has(out, "Your score: 5 / 5"));
}

void test_invalid_topic() {
    std::string out = runSession("2\n9\n0\n");

    assert(has(out, "Invalid topic selection."));
}

void test_invalid_answer_reprompts() {
    std::string out = runSession("2\n1\nX\nB\nB\nB\nB\nB\n0\n");

    assert(has(out, "Invalid option. Enter A, B, C, or D."));
    assert(has(out, "Your score: 5 / 5"));
}

void test_retry_resets_score() {
    std::string out = runSession(
        "2\n1\nB\nB\nB\nB\nB\n1\n"
        "A\nA\nA\nA\nA\n0\n"
    );

    assert(has(out, "Your score: 5 / 5"));
    assert(has(out, "Your score: 0 / 5"));
}

void test_menu_routing() {
    bool adminCalled = false;
    std::string out = runSession("1\n9\n0\n", nullptr, &adminCalled);

    assert(adminCalled);
    assert(has(out, "Invalid menu choice."));
}

int main() {
    test_check_answer();
    test_answer_and_score();
    test_question_validation();
    test_save_question();
    test_topic_selection();
    test_five_questions();
    test_invalid_topic();
    test_invalid_answer_reprompts();
    test_retry_resets_score();
    test_menu_routing();

    std::cout << "All tests passed\n";
    return 0;
}