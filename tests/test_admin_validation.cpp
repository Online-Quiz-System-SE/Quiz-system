#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "admin_validation.h"

int main() {
    const std::string testFile = "qz18_test_questions.txt";

    Question valid{
        "What is 2 + 2?",
        {"1", "2", "4", "5"},
        'C'
    };

    // Valid question
    assert(isValidQuestion(valid));

    // Empty question text
    Question emptyText{"", {"1", "2", "3", "4"}, 'A'};
    assert(!isValidQuestion(emptyText));

    // Whitespace-only question text
    Question whitespaceText{"   ", {"1", "2", "3", "4"}, 'A'};
    assert(!isValidQuestion(whitespaceText));

    // Missing option
    Question missingOption{"Question?", {"A", "B", "C"}, 'A'};
    assert(!isValidQuestion(missingOption));

    // Empty option
    Question emptyOption{"Question?", {"A", "", "C", "D"}, 'A'};
    assert(!isValidQuestion(emptyOption));

    // Invalid answer
    Question invalidAnswer{"Question?", {"A", "B", "C", "D"}, 'E'};
    assert(!isValidQuestion(invalidAnswer));

    // Invalid questions must not modify existing file contents
    {
        std::ofstream file(testFile);
        file << "EXISTING DATA\n";
    }

    assert(!saveQuestion(emptyText, testFile));
    assert(!saveQuestion(missingOption, testFile));
    assert(!saveQuestion(invalidAnswer, testFile));

    {
        std::ifstream file(testFile);
        std::string contents;
        std::getline(file, contents);
        assert(contents == "EXISTING DATA");
    }

    // A valid question should save successfully
    assert(saveQuestion(valid, testFile));

    {
        std::ifstream file(testFile);
        std::string savedQuestion;
        std::getline(file, savedQuestion);

        assert(savedQuestion == "EXISTING DATA");

        std::getline(file, savedQuestion);

        assert(savedQuestion == "What is 2 + 2?|1|2|4|5|C");
    }

    std::remove(testFile.c_str());

    std::cout << "QZ-18 validation tests passed!" << std::endl;

    return 0;
}