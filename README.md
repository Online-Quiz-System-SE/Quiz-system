# Online Quiz System

A console-based multiple-choice quiz application written in C++17.
Players pick a topic and answer five randomly selected questions; administrators log in to add, modify, delete and view questions.

Team: Debangana, Chinmay, Satvik, Sunil

## Features

- Five topics: Machine Learning (ML), DBMS, Software Engineering (SE), DSA, General Knowledge (GK)
- Five random questions per attempt, one at a time, with options A to D
- Invalid answers (e.g. `E`, `5`, `@`, `AB`, empty) are rejected and the question stays active
- Immediate feedback after each answer, final score at the end (`Your score: 3 / 5`)
- Retry the same topic (score resets) or return to topic selection
- Admin login with masked password input (`*`)
- Admin menu: add, modify, delete (with confirmation) and view questions
- Input validation before any question is saved

## Requirements

- A C++17 compiler: clang++ (macOS), g++ (Linux) or MinGW g++ (Windows)
- No internet connection, database or special hardware

## Project structure

```
include/                 Header files
src/
  main.cpp               Entry point, admin login and admin menu
  quiz_controller.cpp    Main menu, topic selection, quiz flow, retry
  quiz_logic.cpp         Answer checking and scoring
  question_loader.cpp    Reads and validates data/questions.txt
  admin_validation.cpp   Question validation, save/modify/delete, credential checks
data/questions.txt       Question bank
tests/                   Unit tests, test data and system test script
docs/                    Test execution log, test summary report, deviations log
```

## Build

Run all commands from the repository root.

macOS:

```bash
clang++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o quiz
```

Linux:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o quiz
```

Windows (MinGW, Command Prompt):

```bat
g++ -std=c++17 -Wall -Wextra -Iinclude src\main.cpp src\quiz_controller.cpp src\quiz_logic.cpp src\question_loader.cpp src\admin_validation.cpp -o quiz.exe
```

## Run

Run the program from the repository root so it can find `data/questions.txt`.

```bash
./quiz          # macOS / Linux
quiz.exe        # Windows
```

Main menu:

```
1. Admin login
2. Take Quiz
0. Exit
```

### Admin access

Choose `1. Admin login` and enter the administrator credentials.
The development build uses the test account `admin` / `admin123`, which is hard-coded in `src/main.cpp`.
Change these before any real use (see `docs/deviations.md`).

### Question file format

Each line of `data/questions.txt` is one question:

```
TOPIC|Question text|Option A|Option B|Option C|Option D|Answer
```

Example:

```
DSA|Which data structure follows FIFO order?|Stack|Queue|Heap|Graph|B
```

`TOPIC` must be one of `ML`, `DBMS`, `SE`, `DSA`, `GK`, and `Answer` must be `A` to `D`.
Text cannot contain `|`. Each topic needs at least five valid questions.

## Tests

### Unit tests

Each test is compiled with all source files except `main.cpp`. On macOS replace `g++` with `clang++`.

```bash
SRC="src/admin_validation.cpp src/question_loader.cpp src/quiz_controller.cpp src/quiz_logic.cpp"

for t in test_main test_admin_validation test_question_loader test_score_reliability; do
  g++ -std=c++17 -Wall -Wextra -Iinclude tests/$t.cpp $SRC -o $t && ./$t
done
```

In zsh (macOS default shell) use `${=SRC}` instead of `$SRC`.

### System tests (TC-01 to TC-26)

```bash
bash tests/system/run_system_tests.sh
```

The script builds the program, runs every test case from the Software Test Plan against the real binary using a temporary copy of the question file (the real `data/questions.txt` is never changed), and writes the results to `docs/test_execution_log.md`.
TC-19, TC-21 and the masking part of TC-26 must be checked by hand.

## Documentation

- `docs/test_execution_log.md`: results of the latest system test run
- `docs/test_summary_report.md`: test summary report
- `docs/deviations.md`: differences between the implementation and the SRS, SAD and STP
- SRS v1.1, Software Architecture and Design Specification v1.0, Software Test Plan v1.0
