#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include "quiz_logic.h"
#include "quiz_controller.h"

static std::vector<Question> sampleQuestions() {
    return {
        {"Q1?", {"a", "b", "c", "d"}, 'B'},
        {"Q2?", {"e", "f", "g", "h"}, 'C'},
    };
}

// Runs the menu with scripted input; returns everything printed
static std::string runSession(const std::string& input, int* score = nullptr,
                              bool* adminCalled = nullptr) {
    std::istringstream in(input);
    std::ostringstream out;
    QuizController c([] { return sampleQuestions(); },
                     [adminCalled] { if (adminCalled) *adminCalled = true; },
                     in, out);
    c.run();
    if (score) *score = c.score();
    return out.str();
}

static bool has(const std::string& s, const std::string& part) {
    return s.find(part) != std::string::npos;
}

void test_check_answer() {
    QuizLogic logic;
    Question q{"Q", {"a", "b", "c", "d"}, 'B'};
    assert(logic.checkAnswer(q, 'B'));
    assert(!logic.checkAnswer(q, 'A'));
}

// F-006, F-007, F-008: answer checking and scoring
void test_answer_and_score() {
    QuizLogic logic;
    Question q1{"What is 2 + 2?", {"1", "2", "4", "5"}, 'C'};
    Question q2{"What is 3 + 3?", {"5", "6", "7", "8"}, 'B'};

    assert(logic.checkAnswer(q1, 'C'));
    assert(!logic.checkAnswer(q1, 'A'));
    assert(logic.checkAnswer(q1, 'c'));

    assert(logic.getScore() == 0);
    logic.updateScore(q1, 'C');
    assert(logic.getScore() == 1);
    logic.updateScore(q2, 'A');
    assert(logic.getScore() == 1);
    logic.updateScore(q2, 'C');
    assert(logic.getScore() == 1);
    logic.updateScore(q2, 'B');
    assert(logic.getScore() == 2);
}

// TC-01: starting a quiz sets score to 0 and shows question 1
void test_start_quiz() {
    int score = -1;
    std::string out = runSession("2\n", &score);
    assert(score == 0);
    assert(has(out, "Question 1 of 2: Q1?"));
}

// TC-02: only the current question is shown until the player answers
void test_one_question_at_a_time() {
    std::string out = runSession("2\n");
    assert(has(out, "Question 1 of 2"));
    assert(!has(out, "Question 2 of 2"));
}

// TC-03: four options labelled A-D
void test_options_a_to_d() {
    std::string out = runSession("2\n");
    assert(has(out, "A. a") && has(out, "B. b") && has(out, "C. c") && has(out, "D. d"));
}

// TC-04, TC-05: invalid input is rejected and re-prompted; lowercase accepted
void test_invalid_answer_reprompts() {
    int score = -1;
    std::string out = runSession("2\nx\nAB\nb\nC\n0\n", &score);
    assert(has(out, "Invalid option. Enter A, B, C, or D."));
    assert(score == 2);
}

// TC-07, TC-08, TC-10: score counts only correct answers; final score shown
void test_final_score() {
    int score = -1;
    std::string out = runSession("2\nB\nA\n0\n", &score);
    assert(score == 1);
    assert(has(out, "Your score: 1 / 2"));
}

// Admin option calls the admin action; invalid menu input is rejected
void test_menu_routing() {
    bool adminCalled = false;
    std::string out = runSession("1\n9\n0\n", nullptr, &adminCalled);
    assert(adminCalled);
    assert(has(out, "Invalid menu choice."));
}

// TC-09: quiz moves to the next question only after a valid answer
void test_moves_to_next_question() {
    std::string invalidOnly = runSession("2\nx\n");
    assert(!has(invalidOnly, "Question 2 of 2"));

    std::string afterValid = runSession("2\nB\n");
    assert(has(afterValid, "Question 2 of 2: Q2?"));
}

int main() {
    test_check_answer();
    test_answer_and_score();
    test_start_quiz();
    test_one_question_at_a_time();
    test_options_a_to_d();
    test_invalid_answer_reprompts();
    test_final_score();
    test_menu_routing();
    test_moves_to_next_question();
    std::cout << "All tests passed\n";
    return 0;
}
