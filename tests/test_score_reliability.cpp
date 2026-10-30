// QZ-20 | Req: OQ-NF-002 | TC: TC-18
// Score reliability: the same answer script gives the same score on 3 runs,
// and the score never resets or changes unexpectedly mid-quiz.

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "quiz_logic.h"
#include "quiz_controller.h"

static const std::string ANSWER_FILE = "tests/data/answers_3of5.txt";

// Fixed question set: every correct answer is 'B', so the score does not
// depend on which 5 questions are randomly selected or their order.
static std::vector<Question> fixedQuestions() {
    std::vector<Question> questions;

    for (int i = 1; i <= 10; ++i) {
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

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    assert(file.is_open() && "Run tests from the repository root");

    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

static int countOccurrences(const std::string& text, const std::string& part) {
    int count = 0;
    std::size_t pos = text.find(part);

    while (pos != std::string::npos) {
        ++count;
        pos = text.find(part, pos + part.size());
    }

    return count;
}

struct RunResult {
    int score;
    std::string output;
};

static RunResult runScript(const std::string& script) {
    std::istringstream in(script);
    std::ostringstream out;

    QuizController controller(
        [] { return fixedQuestions(); },
        [] {},
        in,
        out
    );

    controller.run();

    return {controller.score(), out.str()};
}

// AC 1: answers_3of5.txt gives score 3 on all 3 runs.
void test_same_score_on_three_runs() {
    const std::string script = readFile(ANSWER_FILE);

    for (int run = 1; run <= 3; ++run) {
        RunResult result = runScript(script);

        std::cout << "Run " << run << ": score = " << result.score << '\n';

        assert(result.score == 3);
        assert(result.output.find("Your score: 3 / 5") != std::string::npos);
    }
}

// AC 2: score never resets or changes mid-quiz (controller level).
void test_score_stable_within_quiz() {
    RunResult result = runScript(readFile(ANSWER_FILE));

    // Exactly one final score per quiz attempt: no hidden restart.
    assert(countOccurrences(result.output, "Your score:") == 1);

    // Feedback matches the final score: 3 correct, 2 incorrect.
    assert(countOccurrences(result.output, "Correct! Your answer is right.") == 3);
    assert(countOccurrences(result.output, "Incorrect!") == 2);
}

// AC 2: score never resets or changes mid-quiz (logic level).
void test_score_progression() {
    QuizLogic logic;
    Question q{"Q", {"a", "b", "c", "d"}, 'B'};

    const char answers[] = {'B', 'B', 'B', 'A', 'A'};
    const int expected[] = {1, 2, 3, 3, 3};

    for (int i = 0; i < 5; ++i) {
        int before = logic.getScore();

        logic.updateScore(q, answers[i]);

        // Score never decreases, and increases by at most one per answer.
        assert(logic.getScore() >= before);
        assert(logic.getScore() - before <= 1);
        assert(logic.getScore() == expected[i]);
    }
}

int main() {
    test_same_score_on_three_runs();
    test_score_stable_within_quiz();
    test_score_progression();

    std::cout << "QZ-20 score reliability tests passed\n";
    return 0;
}
