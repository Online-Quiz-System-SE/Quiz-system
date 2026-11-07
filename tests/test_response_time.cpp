
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "quiz_controller.h"

std::vector<Question> fixedQuestions() {
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

int main() {
    std::string script = "2\n1\n";

    // 4 quiz attempts = 20 responses.
    for (int attempt = 0; attempt < 4; ++attempt) {
        for (int answer = 0; answer < 5; ++answer) {
            script += "B\n";
        }

        // Retry topic for first 3 attempts.
        // Return to main menu after final attempt.
        script += (attempt < 3) ? "1\n" : "0\n";
    }

    script += "0\n";

    std::istringstream in(script);
    std::ostringstream out;

    QuizController controller(
        [] { return fixedQuestions(); },
        [] {},
        in,
        out
    );

    controller.run();

    std::string output = out.str();

    std::ofstream log("tests/response_time_test.log");

    if (!log.is_open()) {
        std::cerr << "Cannot create response time log\n";
        return 1;
    }

    log << output;
    log.close();

    std::istringstream lines(output);
    std::string line;

    int count = 0;
    int underOneSecond = 0;

    while (std::getline(lines, line)) {
        if (line.find("Response time: ") == 0) {
            long long milliseconds =
                std::stoll(line.substr(14));

            ++count;

            if (milliseconds < 1000) {
                ++underOneSecond;
            }

            std::cout << line << '\n';
        }
    }

    std::cout << "\nTotal responses: " << count << '\n';
    std::cout << "Under 1 second: " << underOneSecond << '\n';

    if (count != 20 || underOneSecond < 19) {
        std::cerr << "QZ-19 FAILED\n";
        return 1;
    }

    std::cout << "QZ-19 PASSED\n";
    return 0;
}
