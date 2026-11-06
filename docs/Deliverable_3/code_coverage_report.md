# Code Coverage Report - Online Quiz System

| Item | Value |
|---|---|
| Report | Code Coverage Report (SE Mini-Project Deliverables Part 3) |
| Tester | Satvik |
| Date | 06-11-2026 (coverage run) |
| Commit tested | 6cc2884 (main) |
| Environment | macOS, Apple clang version 21.0.0 (clang-2100.1.1.101), gcovr 8.8 with xcrun llvm-cov gcov |
| Build flags | -std=c++17 --coverage -O0 -Iinclude |
| Scope | All files in src/ (test code excluded) |
| Detailed report | docs/Deliverable_3/branch_coverage/branch_coverage.html (gcovr report containing line, branch, and function summaries) |

## 1. What was run

All results come from one coverage-enabled build; coverage counts were accumulated across the three runs below. For branch reporting, compiler-generated exception and unreachable branches were excluded (--exclude-throw-branches --exclude-unreachable-branches).

**Run 1 (baseline)**

1. Unit tests: test_main, test_admin_validation, test_question_loader, and test_score_reliability (4 / 4 passed in the coverage report).
2. System tests TC-01 to TC-26: 26 executed, 23 passed, 0 failed, and 3 marked manual/partial.
3. One quiz attempt on each of the five topics, including retry, return to topic selection, and an invalid topic.

**Run 2 (targeted tests)**

4. tests/test_branch_coverage.cpp: question modification/deletion, file-open failures, empty question bank, fewer than five questions in a topic, invalid menu choices, end of input at prompts, and re-shuffle on retry.
5. Scripted admin inputs: topics 2-5, delimiter characters in question text/options, invalid topic and answer, cancelled deletion, invalid question numbers, viewing all questions, and invalid admin menu choice.

**Run 3 (error paths)**

6. Input ending at admin prompts, wrong username, invalid question data during modification, and admin operations with an empty question bank.
7. Add/modify/delete with a read-only question file, starting with a missing question file, and password entry with Backspace in a real terminal.

## 2. Results

| File | Lines | Covered | Line coverage |
|---|---:|---:|---:|
| src/admin_validation.cpp | 87 | 81 | **93.1%** |
| src/main.cpp | 296 | 265 | **89.5%** |
| src/question_loader.cpp | 71 | 68 | **95.8%** |
| src/quiz_controller.cpp | 198 | 198 | **100.0%** |
| src/quiz_logic.cpp | 13 | 13 | **100.0%** |
| **Total** | **665** | **625** | **94.0%** |

All 41 functions in src/ were executed. One compiler-generated function in main.cpp (global variable initialisation) is excluded automatically by gcovr.

The separate branch coverage report (branch_coverage_report.md) records 534 of 866 branch outcomes taken (61.7%). Branch coverage is lower than line coverage because a line can run while only some outcomes of its decisions are exercised.

## 3. Lines not covered

There are 40 instrumented source lines that were not executed in the recorded run:

| File | Lines not covered | Calculation |
|---|---:|---|
| src/admin_validation.cpp | 6 | 87 total - 81 covered |
| src/main.cpp | 31 | 296 total - 265 covered |
| src/question_loader.cpp | 3 | 71 total - 68 covered |
| src/quiz_controller.cpp | 0 | 198 total - 198 covered |
| src/quiz_logic.cpp | 0 | 13 total - 13 covered |
| **Total** | **40** | **665 total - 625 covered** |

The detailed gcovr HTML report shows the exact uncovered lines. The separate branch report notes remaining paths around terminal password masking, disk read/write failures, and input/stream failure cases. Some stream outcomes are compiler-generated and cannot occur during ordinary use.

## 4. Notes and limitations

- These figures are transcribed from the repository's saved gcovr report. The tests were not rebuilt or rerun while preparing this document.
- The figures describe revision 6cc2884; they may not represent later changes to the source.
- The coverage report records the four unit-test programs as passing. The repository's older test summary, dated 30-10-2026, lists an open issue for test_question_loader. That summary predates this coverage run, so the later coverage result is reported here as recorded; the test history should be reconciled if a single current status is needed.
- System tests TC-19, TC-21, and the password-masking portion of TC-26 are marked manual or partial in the test records.
- Coverage measures code execution; it does not prove that every requirement is correct or that every possible input has been tested.

## 5. Summary

- Overall source line coverage: **94.0% (625 / 665)**.
- quiz_controller.cpp and quiz_logic.cpp have 100% line coverage.
- main.cpp has the largest uncovered-line count (31).
- All 41 source functions were executed in the recorded run.
