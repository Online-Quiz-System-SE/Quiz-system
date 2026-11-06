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
        // Format: TOPIC|Question|Option A|Option B|Option C|Option D|Answer
        file << "ML|What is 2 + 2?|1|2|4|5|C\n";      // valid
        file << "\n";                                  // blank line: ignored
        file << "ML|Malformed record|A|B\n";           // too few fields
        file << "ML|Invalid answer|A|B|C|D|AB\n";      // answer not one letter
        file << "ML||A|B|C|D|A\n";                     // empty question text
        file << "XYZ|Unknown topic|A|B|C|D|A\n";       // invalid topic
        file << "What is 3 + 3?|5|6|7|8|B\n";          // old 6-field format
        file << "DSA|What is 3 + 3?|5|6|7|8|B\n";      // valid
    }

    std::vector<Question> questions = loadQuestions(filename);

    assert(questions.size() == 2);

    assert(questions[0].topic == "ML");
    assert(questions[0].text == "What is 2 + 2?");
    assert(questions[0].options.size() == 4);
    assert(questions[0].answer == 'C');

    assert(questions[1].topic == "DSA");
    assert(questions[1].text == "What is 3 + 3?");
    assert(questions[1].answer == 'B');

    // Topic filter returns only the matching topic
    std::vector<Question> dsa = loadQuestionsByTopic(filename, "DSA");
    assert(dsa.size() == 1);
    assert(dsa[0].text == "What is 3 + 3?");
    assert(loadQuestionsByTopic(filename, "XYZ").empty());

    std::remove(filename.c_str());

    std::cout << "Question loader tests passed!\n";
    return 0;
}
