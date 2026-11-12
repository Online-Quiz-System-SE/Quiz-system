# System Validation Report – Online Quiz System

**Deliverable:** 3  
**Issue:** QZ-23 (#25)  
**Test basis:** Software Test Plan v2.0 (section 13.2); SRS v2.0  
**Validated commit:** `6b59415` (latest `main`)  
**Validation date:** 12 November 2026  
**Testers:** Sunil (Windows/Linux), Debangana (macOS)  
**Team:** Debangana, Chinmay, Satvik, Sunil

## 1. Objective and Scope

This report validates the Online Quiz System against all 32 system test cases in Software Test Plan v2.0: quiz flow, answer validation and scoring, topic bank and randomisation, result menu, question administration, access control, response time, reliability, portability and security.

`src/` is identical in commits `d3c2bc9` and `6b59415` (the later commits only add demo videos), so results from both commits apply to the validated build.

## 2. Test Runs

| Run | Date | Tester | Environment | Commit | Scope |
|---|---|---|---|---|---|
| H | 30-10-2026 | Team | macOS, Apple clang 21.0 | main after PR #50 | TC-01 to TC-26 (system test script) |
| R3 | 06-11-2026 | Team | Ubuntu 22.04, g++ 11.4 | 05bc7d4 | TC-01 to TC-32 (STP v2.0, run R3) |
| F1 | 11-11-2026 | Debangana | macOS, Apple clang | d3c2bc9 | All 6 unit tests; system test script TC-01 to TC-26; manual quiz and admin run |
| F2 | 12-11-2026 | Sunil | Windows (MinGW g++), Ubuntu (g++) | 6b59415 | QZ-22 cross-platform run: build, all five topics, result menu, exit |

Build flags: `-std=c++17 -Wall -Wextra -Iinclude`.

## 3. Execution Summary

| Metric | Result |
|---|---|
| Test cases | 32 (TC-01 to TC-32) |
| Passed | 31 |
| Failed | 0 |
| Pending (manual) | 1 (TC-19) |
| Unit tests (F1) | 6 / 6 passed |
| System test script (F1) | 26 executed, 23 pass, 0 fail, 3 manual (since completed: TC-21, TC-26) |

## 4. Test Case Results – TC-01 to TC-26

| TC | Requirement | Observation | Status | Evidence |
|---|---|---|---|---|
| TC-01 | OQ-F-001 | Quiz starts; Question 1 and score 0 displayed | PASS | H, F1 |
| TC-02 | OQ-F-002 | One question displayed at a time | PASS | H, F1 |
| TC-03 | OQ-F-003 | Exactly options A–D displayed | PASS | H, F1 |
| TC-04 | OQ-F-004 | Valid answer accepted (upper and lower case) | PASS | H, F1 |
| TC-05 | OQ-F-005 | Invalid inputs rejected; valid answer then accepted | PASS | H, F1, F2 |
| TC-06 | OQ-F-006 | Correct and incorrect answers identified, correct option shown | PASS | H, F1, F2 |
| TC-07 | OQ-F-007 | Score increments by one per correct answer | PASS | H, F1 |
| TC-08 | OQ-F-008 | Wrong answer leaves score unchanged | PASS | H, F1 |
| TC-09 | OQ-F-009 | Invalid answer stays on question; valid answer advances | PASS | H, F1 |
| TC-10 | OQ-F-010 | Three correct answers give score 3 | PASS | H, F1 |
| TC-11 | OQ-F-011 | Result screen shows "Your score: 3 / 5" | PASS | H, F1, F2 |
| TC-12 | OQ-F-012 | Added question saved with topic and loaded | PASS | H, F1 |
| TC-13 | OQ-F-013 | Updated question text and answer used in quiz | PASS | H, F1 |
| TC-14 | OQ-F-014 | Confirmed deletion removes the question | PASS | H, F1 |
| TC-15 | OQ-F-015 | Admin operations unreachable without login; data unchanged | PASS* | H, F1 |
| TC-16 | OQ-F-016 | Retry restarts at Q1 with score 0 | PASS | H, F1, F2 |
| TC-17 | OQ-NF-001 | 20 inputs processed in 14 ms total | PASS | H, F1 |
| TC-18 | OQ-NF-002 | Score 3 on all 3 runs | PASS | H, F1 |
| TC-19 | OQ-NF-003 | Three new users complete a quiz without help | PENDING | – |
| TC-20 | OQ-NF-004 | No answers before answering; no admin options in user menu | PASS | H, F1 |
| TC-21 | OQ-NF-005 | Builds and runs on macOS (clang), Windows (MinGW) and Linux (g++) | PASS | F1, F2 (QZ-22 report) |
| TC-22 | OQ-SR-001 | Login prompt shown first; management only after login | PASS | H, F1 |
| TC-23 | OQ-SR-002 | Wrong and empty credentials rejected; correct login works | PASS* | H, F1 |
| TC-24 | OQ-SR-003 | Invalid question data rejected; file unchanged | PASS | H, F1 |
| TC-25 | OQ-SR-004 | Invalid admin/menu values rejected | PASS | H, F1 |
| TC-26 | OQ-SR-005 | Password never printed (automated); shown as `********` while typing in a real terminal (macOS, 11-11-2026) | PASS | H, F1 |

*Behaviour matches; application wording differs from the Test Plan ("Invalid menu choice." for TC-15; "Access denied. Invalid username or password." for TC-23).

## 5. Test Case Results – TC-27 to TC-32 (topic features)

| TC | Requirement | Test (STP v2.0) | Observation | Status | Evidence |
|---|---|---|---|---|---|
| TC-27 | OQ-F-017 | Topic bank contents | 15 questions each for ML, DBMS, SE, DSA, GK (75 total) | PASS | F1 (record count), R3 |
| TC-28 | OQ-F-018 | Malformed records skipped | Four malformed records skipped with warnings and line numbers; only valid questions used | PASS | R3 |
| TC-29 | OQ-F-019 | Topic selection and filtering | All five topics start and show only their own questions; invalid topic gives "Invalid topic selection." | PASS | R3, F2 |
| TC-30 | OQ-F-020 | Five distinct random questions | 5 different questions per attempt; topic with 4 questions gives "Not enough valid questions…" | PASS | R3, F2 |
| TC-31 | OQ-F-021 | Retry gives a new set | Retry restarted the topic with a different set of questions | PASS | R3, F1, F2 |
| TC-32 | OQ-F-022 | Result menu navigation | Retry, return to topic selection and return to main menu all work | PASS | R3, F1, F2 |

## 6. Pending Tests

- **TC-19:** three new users still need to complete a quiz without help; record the result when done.

## 7. Defects

| ID | Description | Severity | Status |
|---|---|---|---|
| D-01 | Admin-added questions saved without their topic | Medium | FIXED (PR #50) |
| D-02 | `test_question_loader` used the old 6-field format | Low | FIXED (PR #56); passes in F1 |
| D-03 | `saveQuestion()` does not add a newline before appending; the first question added is joined to the last line of `data/questions.txt` and lost | Medium | OPEN |
| D-04 | An invalid choice at the result menu shows "Invalid menu choice." and then starts a new attempt instead of showing the menu again | Low | OPEN |

Notes: a uniform answer key is used in test fixtures because questions are shown in random order. D-03 was found during branch coverage testing; D-04 during the manual run F1.

## 8. Exit Criteria

| Criterion | Status |
|---|---|
| All 32 test cases executed | Met, except TC-19 (manual, pending) |
| All High-priority and security test cases passed | Met |
| No open Critical or High defect | Met (D-03 Medium and D-04 Low are open) |
| Validated on the latest `main` with commit hash | Met (`6b59415`) |
| RTM result columns updated | To be updated in Test Plan v2.0 |

## 9. Conclusion

The Online Quiz System at commit `6b59415` passed 31 of 32 system test cases with no failures, on macOS, Windows and Linux. TC-19 (usability with new users) is pending. Two defects remain open (D-03 Medium, D-04 Low). The system is ready for the acceptance demonstration.

**Source records:** `docs/test_execution_log.md`, `docs/test_summary_report.md`, `docs/Deliverable_3/QZ-22_Test_Report.md`, Software Test Plan v2.0 section 13.
