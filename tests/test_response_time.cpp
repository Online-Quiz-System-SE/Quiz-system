#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "quiz_controller.h"

// QZ-19 / OQ-NF-001: each user input should be processed within 1 second.
static std::vector<Question> fixedQuestions() {
    std::vector<Question> questions;
    for (int i = 1; i <= 10; ++i) {
        Question q{"ML Question " + std::to_string(i),
                   {"Option A", "Option B", "Option C", "Option D"}, 'B'};
        q.topic = "ML";
        questions.push_back(q);
    }
    return questions;
}

int main() {
    // Main menu -> Take Quiz -> ML, then 4 attempts of 5 answers = 20 responses
    std::string script = "2\n1\n";
    for (int attempt = 0; attempt < 4; ++attempt) {
        script += "B\nB\nB\nB\nB\n";
        script += (attempt < 3) ? "1\n" : "0\n";
    }
    script += "0\n";

    std::istringstream in(script);
    std::ostringstream out;
    QuizController controller([] { return fixedQuestions(); }, [] {}, in, out);

    auto start = std::chrono::steady_clock::now();
    controller.run();
    auto end = std::chrono::steady_clock::now();

    const std::string output = out.str();
    int responses = 0;
    for (std::size_t pos = output.find("Correct answer:"); pos != std::string::npos;
         pos = output.find("Correct answer:", pos + 1)) {
        ++responses;
    }

    const double totalMs =
        std::chrono::duration<double, std::milli>(end - start).count();
    const double averageMs = responses > 0 ? totalMs / responses : 0.0;

    std::cout << "Responses processed: " << responses << '\n'
              << "Total time: " << totalMs << " ms\n"
              << "Average per response: " << averageMs << " ms\n";

    if (responses != 20 || averageMs >= 1000.0) {
        std::cerr << "QZ-19 FAILED\n";
        return 1;
    }

    std::cout << "QZ-19 PASSED\n";
    return 0;
}