# System Testing Report – Online Quiz System

**Deliverable:** 3  
**Issue:** QZ-23 (#25)  
**Test basis:** Software Test Plan v1.0, Section 13.2  
**Recorded execution date:** 30 October 2026  
**Team:** Debangana, Chinmay, Satvik, Sunil

## 1. Objective and Scope

The purpose of system testing was to verify the integrated Online Quiz System against its Software Test Plan and requirements in SRS v1.1. Testing covered quiz flow, answer validation and scoring, question administration, access control, retry behaviour, response time, reliability, and security.

## 2. Test Environment

| Item | Recorded value |
|---|---|
| Operating system | macOS Darwin 25.6.0 |
| Compiler | Apple clang 21.0.0 |
| Build flags | `-std=c++17 -Wall -Wextra` |
| Build result | Successful, 0 warnings |
| Test script | `tests/system/run_system_tests.sh` |

## 3. Execution Summary

The recorded regression run executed all 26 test cases. All 23 automated test cases passed, with no automated failures. Three cases require manual execution or completion.

| Metric | Result |
|---|---:|
| Test cases executed | 26 / 26 (100%) |
| Automated passed | 23 / 23 |
| Automated failed | 0 |
| Manual / partial | 3 (TC-19, TC-21, TC-26) |
| Requirement coverage | 26 / 26 (100%) |

## 4. Test Case Results

| TC | Requirement | Result / Observation | Status |
|---|---|---|---|
| TC-01 | OQ-F-001 | Quiz starts; Question 1 and score 0 displayed | PASS |
| TC-02 | OQ-F-002 | One question displayed at a time | PASS |
| TC-03 | OQ-F-003 | Exactly options A–D displayed | PASS |
| TC-04 | OQ-F-004 | Valid answer B accepted | PASS |
| TC-05 | OQ-F-005 | Invalid inputs rejected; A accepted | PASS |
| TC-06 | OQ-F-006 | Correct and incorrect answers identified | PASS |
| TC-07 | OQ-F-007 | Score increments by one per correct answer | PASS |
| TC-08 | OQ-F-008 | Wrong answer leaves score unchanged | PASS |
| TC-09 | OQ-F-009 | Invalid answer stays on question; valid answer advances | PASS |
| TC-10 | OQ-F-010 | Three correct answers produce score 3 | PASS |
| TC-11 | OQ-F-011 | Result screen displays score 3 / 5 | PASS |
| TC-12 | OQ-F-012 | Added question saved and loaded (6 questions) | PASS |
| TC-13 | OQ-F-013 | Updated question text and answer used in quiz | PASS |
| TC-14 | OQ-F-014 | Confirmed deletion leaves 5 questions | PASS |
| TC-15 | OQ-F-015 | Admin operations inaccessible without login; data unchanged | PASS* |
| TC-16 | OQ-F-016 | Retry resets to Q1 and score 0 | PASS |
| TC-17 | OQ-NF-001 | 20 inputs processed in 14 ms total | PASS |
| TC-18 | OQ-NF-002 | Score 3 repeated consistently across 3 runs | PASS |
| TC-19 | OQ-NF-003 | Three new users complete quiz without extra help | MANUAL |
| TC-20 | OQ-NF-004 | Answers hidden before answering; no admin in user menu | PASS |
| TC-21 | OQ-NF-005 | Windows MinGW and Linux GCC portability checks | PARTIAL |
| TC-22 | OQ-SR-001 | Login shown first; management only after login | PASS |
| TC-23 | OQ-SR-002 | Invalid credentials rejected; correct login succeeds | PASS* |
| TC-24 | OQ-SR-003 | Invalid question records rejected; file unchanged | PASS |
| TC-25 | OQ-SR-004 | Invalid admin/menu values rejected | PASS |
| TC-26 | OQ-SR-005 | Password output check passed; terminal masking manual | PARTIAL |

*Behaviour passed; application wording differs from the test-plan wording in TC-15 and TC-23.

## 5. Manual Tests Pending

- **TC-19:** Recruit three new users and record whether they complete a quiz without extra help.
- **TC-21:** Build and run on Windows using MinGW and on Linux using GCC; record platform results.
- **TC-26:** Verify that password characters are masked with `*` while typing in a real terminal.

These checks are not claimed as completed.

## 6. Defects and Deviations

- **Medium defect (fixed):** Admin-added questions were saved without their topic and therefore did not appear in quizzes. Fixed in PR #50 and verified by the regression run.
- **Low defect (open):** `tests/test_question_loader.cpp` fails on `main` because it expects 2 questions while the loader rejects 5 of 6 records as malformed. Recorded as not caused by this system-test cycle.
- A uniform answer key was used in test fixtures because questions are displayed in random order.
- TC-15 and TC-23 application wording differs from the test plan, while observed access-control behaviour matches.
- Windows/Linux portability checks were not completed in the recorded macOS run.

## 7. Exit Criteria

| Criterion | Status |
|---|---|
| All 26 test cases executed | Met |
| All high-priority and security automated cases passed | Met in recorded run; TC-26 masking check pending |
| No open critical defect | Met |
| RTM result columns updated | Pending confirmation/update |

## 8. Conclusion

The recorded regression run indicates that the Online Quiz System passed all 23 automated test cases with no automated failures. TC-19, TC-21 and the terminal-masking portion of TC-26 require manual completion, and the RTM result columns must be confirmed updated. Subject to these follow-ups, the system is ready for the acceptance demonstration.

**Source records:** `docs/test_execution_log.md` and `docs/test_summary_report.md` (recorded execution dated 30-10-2026).
