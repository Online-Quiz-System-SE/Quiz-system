# Code Coverage Report - Online Quiz System

| Item | Value |
|---|---|
| Report | Code Coverage Report (SE Mini-Project Deliverables Part 3) |
| Tester | Satvik |
| Date | 06-11-2026 (coverage run); Windows verification on 08-11-2026 |
| Commit tested | 6cc2884 (main) |
| Environment | Original coverage run: macOS, Apple clang 21.0.0, gcovr 8.8 with xcrun llvm-cov gcov. Verification attempt: Windows, MSYS2 GCC, gcovr 8.8. |
| Build flags | Original run: -std=c++17 --coverage -O0 -Iinclude |
| Scope | All files in src/ (test code excluded) |
| Detailed report | docs/Deliverable_3/branch_coverage/branch_coverage.html (gcovr report containing line, branch, and function summaries) |

## 1. What was run

The saved coverage report records one coverage-enabled build with counts accumulated across the three runs below. For branch reporting, compiler-generated exception and unreachable branches were excluded (--exclude-throw-branches --exclude-unreachable-branches).

**Run 1 (baseline)**

1. Unit tests: test_main, test_admin_validation, test_question_loader, and test_score_reliability (4 / 4 passed in the saved coverage run).
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

All 41 functions in src/ were executed in the saved run. One compiler-generated function in main.cpp (global variable initialisation) is excluded automatically by gcovr.

The separate branch coverage report (branch_coverage_report.md) records 534 of 866 branch outcomes taken (61.7%). Branch coverage is lower than line coverage because a line can run while only some outcomes of its decisions are exercised.

## 3. Lines not covered

There are 40 instrumented source lines that were not executed in the saved run:

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

- The 94.0% figures above are the saved gcovr results for revision 6cc2884; they have not been independently reproduced on this laptop.
- I reran the four unit-test programs on Windows using MSYS2 GCC and gcovr 8.8. All four passed. This unit-test-only run reported 42.2% line coverage (246 / 583 instrumented lines). It did not include the system and targeted test runs listed above, and main.cpp had no recorded executions, so this result is not directly comparable with the saved full-run result.
- A full Windows rerun was blocked: Git Bash could not start the system-test script because Windows denied creation of its MSYS named object (NtCreateDirectoryObject, 0xC0000022), and WSL was unavailable. The partial unit-test result should not be compared directly with the saved full-run figures.
- The figures describe revision 6cc2884; they may not represent later changes to the source.
- System tests TC-19, TC-21, and the password-masking portion of TC-26 are marked manual or partial in the test records.
- Coverage measures code execution; it does not prove that every requirement is correct or that every possible input has been tested.

## 5. Summary

- Saved overall source line coverage: **94.0% (625 / 665)**.
- The four unit-test programs passed in the saved report and in the Windows rerun.
- quiz_controller.cpp and quiz_logic.cpp have 100% line coverage in the saved report.
- main.cpp has the largest uncovered-line count (31) in the saved report.
- All 41 source functions were executed in the saved run.
