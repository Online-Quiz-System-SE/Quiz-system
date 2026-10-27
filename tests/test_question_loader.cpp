#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "question_loader.h"

int main() {
    const std::string filename = "test_questions.txt";

    {
        std::ofstream file(filename);
        file << "What is 2 + 2?|1|2|4|5|C\n";
        file << "\n";
        file << "Malformed record|A|B\n";
        file << "Invalid answer|A|B|C|D|AB\n";
        file << "Empty question?|A|B|C|D|A\n";
        file << "What is 3 + 3?|5|6|7|8|B\n";
    }

    std::vector<Question> questions = loadQuestions(filename);

    assert(questions.size() == 2);

    assert(questions[0].text == "What is 2 + 2?");
    assert(questions[0].options.size() == 4);
    assert(questions[0].answer == 'C');

    assert(questions[1].text == "What is 3 + 3?");
    assert(questions[1].answer == 'B');

    std::remove(filename.c_str());

    std::cout << "Question loader tests passed!\n";
    return 0;
}