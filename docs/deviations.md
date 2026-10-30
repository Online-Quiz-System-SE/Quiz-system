# Deviations Log - Online Quiz System v1.0

This log lists every known difference between the delivered implementation and the
Software Requirements Specification (SRS v1.1), the Software Architecture and Design
Specification (SAD v1.0) and the Software Test Plan (STP v1.0).

Impact: **None** = no requirement is weakened; **Low** = cosmetic or documentation only;
**Medium** = a stated item is not implemented as written.

## 1. Deviations from the SRS (v1.1)

| ID | SRS reference | Specified | Implemented | Reason | Impact |
|---|---|---|---|---|---|
| SRS-D1 | 2.2, OQ-F-001 | Start a quiz from a single question set | User first selects one of five topics (ML, DBMS, SE, DSA, GK); 5 questions are drawn at random from that topic | Larger question bank and more varied quizzes | None (extends OQ-F-001) |
| SRS-D2 | OQ-F-002 | Display questions in sequence | Questions are shuffled; the same 5-question set is not repeated on consecutive attempts when possible | Variety on retry | None |
| SRS-D3 | OQ-F-006 | Mark answer correct or incorrect | The correct answer and its text are also shown after every response | Immediate feedback (SAD 4.5) | None |
| SRS-D4 | OQ-F-016 | Restart the quiz after the result | Result menu offers "Retry this topic", "Return to topic selection" and "Return to main menu" | Restart extended to topic choice | None |
| SRS-D5 | OQ-F-014 | Delete a selected question | Deletion requires a Y/N confirmation; extra "View all questions" admin option | Prevent accidental data loss | None |
| SRS-D6 | 3.3 | Question data in a local file | Plain-text file `data/questions.txt`, format `TOPIC\|text\|A\|B\|C\|D\|answer`; the `\|` character is not allowed in question or option text | Simple, editable format | None |
| SRS-D7 | 4.3, OQ-SR-002 | Administrator credentials | Single test account `admin` / `admin123` hard-coded in `src/main.cpp`; no credential file | Academic scope | Medium: credentials must be changed before real use |
| SRS-D8 | OQ-SR-005 | Credentials not displayed | Password masked with `*` on Windows (`_getch`) and macOS/Linux (`termios`). If input is not a terminal (e.g. redirected from a file) the password is read without echo and without `*` | Terminal mode cannot be changed for redirected input | None (password is still never printed) |
| SRS-D9 | 6 | Acceptance test suites listed as "Authentication, Withdrawal, Deposit, Transfer, Performance, Security, Accessibility" | Acceptance tests follow the STP (quiz flow, scoring, question management, invalid input, admin access) | Withdrawal/Deposit/Transfer is leftover text from the SRS template and does not apply | Low (documentation error) |

## 2. Deviations from the SAD (v1.0)

| ID | SAD reference | Specified | Implemented | Reason | Impact |
|---|---|---|---|---|---|
| SAD-D1 | 3.3 Component diagram | ConsoleUI, InputHandler, QuizEngine, Scorer, ReviewManager, BankBuilder, ConfigLoader, QuestionBankRepository, ResultLogger, IntegrityChecker, AnswerKeyCodec | Five modules: `quiz_controller` (UI + flow), `quiz_logic` (checking + scoring), `question_loader` (data), `admin_validation` (validation + file updates), `main.cpp` (admin login and menu) | Smaller design fits the project scope; layered separation (presentation, application, data) is kept | Low |
| SAD-D2 | 3.3, 3.6 | Data stores `questions.qbk`, `quiz.cfg`, `results.csv` | Only `data/questions.txt`; no config file, results file or answer-key encoding | Not required by any SRS requirement | Low |
| SAD-D3 | 3.4 Admin Module | Separate Admin Module component | Admin functions are free functions in `src/main.cpp` and `src/admin_validation.cpp`, passed to `QuizController` as a callback | Keeps `QuizController` testable with injected input/output | Low |
| SAD-D4 | 4.2 Admin sequence | Credentials and question data pass through Quiz Controller to Quiz Logic | Admin flow is handled in `main.cpp`; `QuizController` only calls the admin callback | Separates admin from player flow (SAD 3.9) | Low |
| SAD-D5 | 4.3 API | `QuizLogic::getQuestion()`, `ScoreResult::calculateScore()`, `AdminModule::addQuestion()` | `QuizLogic::checkAnswer()`, `updateScore()`, `getScore()`, `resetScore()`; `QuizController::run()`, `startQuiz()`; `saveQuestion()`, `modifyQuestion()`, `deleteQuestion()`, `isValidQuestion()`, `areCredentialsNonEmpty()` | Interfaces refined during implementation | Low |
| SAD-D6 | 4.3 | Score as correct count and percentage | Score shown as correct count out of total (`3 / 5`); no percentage | Sufficient for OQ-F-010/011 | Low |
| SAD-D7 | 4.4 | Optional result logging | Not implemented | Not required by the SRS | None |

## 3. Deviations from the STP (v1.0)

| ID | STP reference | Specified | Actual | Reason | Impact |
|---|---|---|---|---|---|
| STP-D1 | 5, 8 | Test scripts as separate input files (`./quiz < input.txt`) | One script, `tests/system/run_system_tests.sh`, holds the scripted input for all 26 test cases and pipes it into the real binary | Easier to maintain and re-run | None |
| STP-D2 | 6 Test data | `questions.txt` with 5 questions and a known answer key | Each case uses a temporary fixture (5 or 6 ML questions, every answer B); the real `data/questions.txt` is never modified | Questions are shown in random order, so a per-position key cannot be scripted | None |
| STP-D3 | TC-06 | Q1 key B, Q2 key C | Uniform key B; Q1 answered B (correct), Q2 answered A (incorrect) | Random question order (see STP-D2) | None |
| STP-D4 | TC-01 | Start Quiz shows Question 1 | Topic selection appears between Start Quiz and Question 1 | See SRS-D1 | None |
| STP-D5 | TC-15 | Message "Access denied. Admin login required." | Add, modify and delete have no entry point outside the admin menu; such input gives "Invalid menu choice." and data is unchanged | Management is only reachable after login | None (requirement met, wording differs) |
| STP-D6 | TC-23 | Message "Invalid credentials" | "Access denied. Invalid username or password." for wrong credentials and "Username and password cannot be empty." for empty fields | More specific messages | None (wording only) |
| STP-D7 | TC-25 | `0` treated as a hidden menu number | `0` is the documented Exit option; `9`, `99`, `-1`, `admin` are rejected | `0` is a valid menu item | None |
| STP-D8 | 6 Environment, TC-21 | Executed on Windows (MinGW) and Linux (GCC) | Automated run on macOS with Apple clang; Windows and Linux checked manually | Team hardware | Low |
| STP-D9 | TC-17 | Stopwatch timing of 20 inputs | Total processing time of 20 scripted inputs measured in the test script (15 ms) | More precise than a stopwatch | None |
| STP-D10 | 7 Schedule | Execution 05-Oct-2026 to 18-Oct-2026 | Scripted execution and regression run on 30-Oct-2026 | Integration of feature branches took longer than planned | Low |
| STP-D11 | 9 Roles | QA Lead, Test Engineer, Developer, Product Owner named | Names not filled in the STP | To be completed by the team | Low |
| STP-D12 | 6 | Admin system tests run on Windows | Scripted admin tests run on macOS/Linux only; `_getch` on Windows reads the console, not redirected input | Platform limitation | Low |
