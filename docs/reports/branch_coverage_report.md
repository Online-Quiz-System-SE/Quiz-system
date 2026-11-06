# Branch Coverage Report - Online Quiz System

| Item | Value |
|---|---|
| Report | Branch Coverage Report (SE Mini-Project Deliverables Part 3) |
| Tester | Debangana |
| Date | 06-11-2026 |
| Commit tested | `6cc2884` (main) |
| Environment | macOS, Apple clang version 21.0.0 (clang-2100.1.1.101), gcovr 8.8 with `xcrun llvm-cov gcov` |
| Build flags | `-std=c++17 --coverage -O0 -Iinclude` |
| Scope | All files in `src/` (test code excluded) |
| Detailed report | `docs/reports/branch_coverage/branch_coverage.html` |
| Raw summary | `docs/reports/branch_coverage.txt` |

## 1. What was run

All results come from one coverage build; coverage counts add up across the runs below. Compiler-generated exception and unreachable branches were excluded (`--exclude-throw-branches --exclude-unreachable-branches`).

**Run 1 (baseline)**
1. Unit tests: `test_main`, `test_admin_validation`, `test_question_loader`, `test_score_reliability` (4 / 4 passed).
2. System tests TC-01 to TC-26 (`tests/system/run_system_tests.sh` with a coverage-enabled compiler): 26 executed, 23 pass, 0 fail, 3 manual/partial.
3. One quiz attempt on each of the five topics, including retry, return to topic selection and an invalid topic.

**Run 2 (targeted tests)**
4. New unit test `tests/test_branch_coverage.cpp`: `modifyQuestion()`, `deleteQuestion()`, file-open failures, empty question bank, topic with fewer than five questions, invalid main/topic/result menu choices, end of input at each prompt, re-shuffle on retry.
5. Scripted admin inputs: admin topics 2-5, `|` in question text and options, invalid topic and answer, delete cancelled with `N`, invalid question numbers, view all questions, invalid admin menu choice.

**Run 3 (error paths)**
6. Input ending at each admin prompt (username, admin menu, topic, question text, option, answer, question number, Y/N).
7. Wrong username; invalid question data while modifying.
8. Admin view/modify/delete with an empty question bank.
9. Add, modify and delete with a read-only question file (save fails).
10. Program started with the question file missing.
11. Password entry with Backspace through a real terminal (`script`).

## 2. Results

| File | Branches | Taken | Branch coverage |
|---|---|---|---|
| `src/quiz_logic.cpp` | 2 | 2 | **100%** |
| `src/quiz_controller.cpp` | 204 | 142 | **69.6%** |
| `src/admin_validation.cpp` | 144 | 90 | **62.5%** |
| `src/question_loader.cpp` | 120 | 73 | **60.8%** |
| `src/main.cpp` | 396 | 227 | **57.3%** |
| **Total** | **866** | **534** | **61.7%** |

| Run | Branch coverage |
|---|---|
| Run 1 (baseline) | 44.7% (387 / 866) |
| Run 2 (targeted tests) | 58.8% (509 / 866) |
| Run 3 (error paths) | **61.7% (534 / 866)** |

All 41 functions in `src/` were executed. One compiler-generated function in `main.cpp` (global variable initialisation) is excluded automatically by gcovr.

**Note on the figure:** gcov counts every possible outcome of each condition, including compiler-generated branches for every `std::getline`, `std::string` and stream operation (for example, the end-of-input and read-failure case of each input). Many of these cannot occur in normal use, so not all 866 branches are reachable by tests.

## 3. Remaining untested branches

| Area | File : lines | Reason |
|---|---|---|
| Masked password keystrokes | `main.cpp` : 62-97 | Runs only when input is a real terminal; tests use redirected input. Checked manually in TC-26 |
| Disk read/write failure | `question_loader.cpp` : 98-100; `admin_validation.cpp` : 88, 118, 125, 150, 165, 172 | Needs a failing file system to trigger |
| Hidden input/stream branches | All files (each `getline`, `cin` and string operation) | Compiler-generated failure branches that cannot be reached in normal use |

## 4. Summary

- Branch coverage increased from 44.7% to 61.7% over three runs of targeted tests.
- `quiz_logic.cpp` has 100% branch coverage, and every function in `src/` was executed.
- The remaining untested branches are the password masking code, disk failure handling and compiler-generated input/stream branches.
