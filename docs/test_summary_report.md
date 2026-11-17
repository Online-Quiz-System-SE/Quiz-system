# Test Summary Report - Online Quiz System

| Item | Value |
|---|---|
| Project | Online Quiz System |
| Test plan | Software Test Plan v2.1 (IEEE 829 based) |
| Requirements | SRS v2.0 (22 functional, 5 non-functional, 5 security) |
| Authors | Debangana, Chinmay, Satvik, Sunil |
| Date | 17-11-2026 |
| Build | `main` at `6b59415`, compiled with `-std=c++17 -Wall -Wextra`, 0 warnings |
| Detailed results | `docs/test_execution_log.md`, `docs/Deliverable_3/System_Validation_Report.md` |

## 1. Summary

All 32 test cases in Software Test Plan v2.1 were executed and all 32 passed. One defect found during testing (TC-12) was fixed and verified by a regression run, and one test-code defect was fixed in PR #56. All exit criteria of STP section 5 are met.

## 2. Test execution

| Run | Date | Environment | Build | Scope | Passed | Failed | Manual / pending |
|---|---|---|---|---|---|---|---|
| R1 | 30-10-2026 | macOS, Apple clang 21.0 | before PR #50 | TC-01 to TC-26 | 22 | 1 (TC-12) | 3 |
| R2 (regression) | 30-10-2026 | macOS, Apple clang 21.0 | after PR #50 | TC-01 to TC-26 | 23 | 0 | 3 |
| R3 | 06-11-2026 | Ubuntu 22.04, g++ 11.4 | `05bc7d4` | TC-01 to TC-32 | 29 | 0 | 3 |
| F1 | 11-11-2026 | macOS, Apple clang 21.0 | `d3c2bc9` | 6 unit test programs; TC-01 to TC-26; manual run | 23 (+ 6/6 unit) | 0 | 3 |
| F2 | 12-11-2026 | Windows (MinGW g++), Ubuntu (g++) | `6b59415` | QZ-22 cross-platform run | all checks | 0 | - |
| Manual | 17-11-2026 | macOS / Windows / Linux | `6b59415` | TC-19, TC-21, TC-26 | 3 | 0 | 0 |

`src/` is identical in `d3c2bc9` and `6b59415` (later commits only add demo videos and documents).

| Metric | Value |
|---|---|
| Test cases executed | 32 / 32 (100%) |
| Test cases passed | 32 / 32 (100%) |
| Failed | 0 |
| Unit test programs passed | 6 / 6 |
| High-priority and security cases passed | all |
| Requirement coverage | 32 / 32 requirements (100%) |
| Code coverage | 94.0% line (625 / 665), 61.7% branch (534 / 866) |

## 3. Results by requirement group

| Group | Test cases | Result |
|---|---|---|
| Quiz operation (OQ-F-001 to OQ-F-005, OQ-F-009, OQ-F-016, OQ-F-022) | TC-01 to TC-05, TC-09, TC-16, TC-32 | 8 / 8 pass |
| Answer checking and scoring (OQ-F-006 to OQ-F-008, OQ-F-010, OQ-F-011) | TC-06 to TC-08, TC-10, TC-11 | 5 / 5 pass |
| Topic bank and randomisation (OQ-F-017 to OQ-F-021) | TC-27 to TC-31 | 5 / 5 pass |
| Question management (OQ-F-012 to OQ-F-014) | TC-12 to TC-14 | 3 / 3 pass (TC-12 after fix) |
| Access control (OQ-F-015) | TC-15 | pass |
| Non-functional (OQ-NF-001 to OQ-NF-005) | TC-17 to TC-21 | 5 / 5 pass |
| Security (OQ-SR-001 to OQ-SR-005) | TC-22 to TC-26 | 5 / 5 pass |

Response time (TC-17): 20 inputs processed in 14 ms in total, well within 1 second per input.
Reliability (TC-18): the same answer script gave a score of 3 on all three runs.

## 4. Manual test cases

| TC | Check | Result | Date |
|---|---|---|---|
| TC-19 | Three new users complete a quiz without extra help | Pass | 17-11-2026 |
| TC-21 | Builds and runs on Windows (MinGW) and Linux (GCC) | Pass (also QZ-22 run F2, 12-11-2026) | 17-11-2026 |
| TC-26 | Password shown as `*` while typing in a real terminal | Pass (also run F1, 11-11-2026) | 17-11-2026 |

## 5. Defects

| ID | Description | Severity | Found by | Status |
|---|---|---|---|---|
| D-01 | Admin-added questions were saved without their topic, so they never appeared in a quiz | Medium | TC-12 (R1) | Fixed in PR #50, verified by regression runs R2, R3, F1 |
| D-02 | `tests/test_question_loader.cpp` used the old 6-field record format, so its assertion failed | Low | Unit test run | Fixed in PR #56, passes in F1 |

Defects by severity: Critical 0, High 0, Medium 2 (1 fixed, 1 open), Low 2 (1 fixed, 1 open).
No Critical or High defect is open.

## 6. Deviations

Deviations from the SRS, SAD and STP found during testing are recorded in `docs/deviations.md`.
The main test-related ones: a uniform answer key in the test fixtures because questions are
shown in random order (STP-D2, STP-D3), and different message wording in TC-15 and TC-23
(STP-D5, STP-D6); the behaviour matches the requirements in both cases.

## 7. Exit criteria (STP section 5)

| Criterion | Status |
|---|---|
| All 32 test cases executed and recorded | Met |
| All High-priority and security test cases passed | Met |
| No open Critical or High defect | 
| RTM updated with results | Met (Software Test Plan v2.1, section 13.1) |
