#include <cassert>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "admin_validation.h"
#include "question_loader.h"
#include "quiz_controller.h"

// Runs the quiz menu with scripted input and returns everything printed
static std::string runMenu(const std::vector<Question>& bank, const std::string& input) {
    std::istringstream in(input);
    std::ostringstream out;
    QuizController c([bank] { return bank; }, nullptr, in, out);
    c.run();
    return out.str();
}
static bool has(const std::string& s, const std::string& part) {
    return s.find(part) != std::string::npos;
}

int main() {
    const std::string f = "branch_test_questions.txt";
    Question good{"Q?", {"a", "b", "c", "d"}, 'B', "ML"};
    Question bad{"", {"a", "b", "c", "d"}, 'B', "ML"};     // empty text
    Question upd{"Updated?", {"a", "b", "c", "d"}, 'C', "ML"};
    std::remove(f.c_str());

    // saveQuestion: valid, invalid, file cannot be opened
    assert(saveQuestion(good, f));
    assert(saveQuestion(good, f));
    assert(!saveQuestion(bad, f));
    assert(!saveQuestion(good, "no_such_folder/x.txt"));

    // modifyQuestion: success, invalid question, index out of range, missing file
    assert(modifyQuestion(f, 0, upd));
    assert(!modifyQuestion(f, 0, bad));
    assert(!modifyQuestion(f, 99, upd));
    assert(!modifyQuestion("missing.txt", 0, upd));

    // deleteQuestion: success, index out of range, missing file
    assert(deleteQuestion(f, 1));
    assert(!deleteQuestion(f, 99));
    assert(!deleteQuestion("missing.txt", 0));
    std::remove(f.c_str());

    // loader: missing file
    assert(loadQuestions("missing.txt").empty());

    // QuizController branches
    std::vector<Question> five(5, good), four(4, good), six;
    for (int i = 0; i < 6; ++i) six.push_back({"Q" + std::to_string(i), {"a","b","c","d"}, 'B', "ML"});

    assert(has(runMenu({}, "2\n0\n"), "No valid questions could be loaded"));   // empty bank
    assert(has(runMenu(four, "2\n1\n0\n0\n"), "Not enough valid questions"));   // < 5 questions
    assert(has(runMenu(five, "2\n2\n0\n0\n"), "Not enough valid questions"));   // topic DBMS has none
    runMenu(five, "2\n3\n0\n2\n4\n0\n2\n5\n0\n0\n");                            // topics SE, DSA, GK
    runMenu(five, "1\n0\n");                                                     // admin action not set
    assert(has(runMenu(five, "7\n0\n"), "Invalid menu choice"));                 // invalid main menu
    assert(has(runMenu(five, "2\nx\n0\n0\n"), "Invalid topic selection"));       // invalid topic
    assert(has(runMenu(five, "2\n1\nB\nB\nB\nB\nB\n7\n0\n0\n"), "Invalid menu choice")); // invalid result menu
    runMenu(five, "");                       // end of input at main menu
    runMenu(five, "2\n");                    // end of input at topic menu
    runMenu(five, "2\n1\nB\n");              // end of input during quiz
    runMenu(five, "2\n1\nB\nB\nB\nB\nB\n");  // end of input at result menu

    // Re-shuffle loop: with 6 questions a retry often picks the same set
    std::string retries = "2\n1\n";
    for (int i = 0; i < 30; ++i) retries += "B\nB\nB\nB\nB\n1\n";
    runMenu(six, retries);

    std::cout << "Branch coverage tests passed!\n";
    return 0;
}