# System Validation Report – Online Quiz System

**Deliverable:** 3  
**Issue:** QZ-23 (#25)  
**Test basis:** Software Test Plan v2.0; SRS v2.0  
**Validation date:** 12 November 2026  
**Validated commit:** `6b59415` (latest `main`, PR #65 merge)  
**Fresh validator:** Sunil  
**Team:** Debangana, Chinmay, Satvik, Sunil

## 1. Objective and Scope

This report records the historical system-test execution and the fresh validation performed on the latest `main` commit. Validation covers quiz flow, answer validation and scoring, topic selection, result-menu actions, retry behaviour, question administration, access control, response time, reliability, and security.

The historical 26-case regression run dated 30 October 2026 is retained as historical evidence. The fresh Windows validation on 12 November 2026 is reported separately and does not imply that every historical manual test was completed.

## 2. Test Environment

### Historical execution (30 October 2026)

| Item | Recorded value |
|---|---|
| Operating system | macOS Darwin 25.6.0 |
| Compiler | Apple clang 21.0.0 |
| Build flags | `-std=c++17 -Wall -Wextra` |
| Build result | Successful, 0 warnings |
| Test script | `tests/system/run_system_tests.sh` |

### Fresh validation (12 November 2026)

| Item | Recorded value |
|---|---|
| Operating system | Windows (PowerShell) |
| Compiler | g++ (MinGW) |
| Build flags | `-std=c++17 -Wall -Wextra -Iinclude` |
| Build command | `g++ -std=c++17 -Wall -Wextra -Iinclude (Get-ChildItem src -Filter *.cpp \| ForEach-Object { $_.FullName }) -o quiz-windows.exe` |
| Build result | Successful; no compiler errors or warnings displayed |
| Tested commit | `6b59415` |
| Application | `quiz-windows.exe` |

## 3. Execution Summary

### Historical regression run

The recorded regression run executed all 26 test cases. All 23 automated test cases passed, with no automated failures. TC-19, TC-21, and TC-26 required manual execution or completion.

| Metric | Result |
|---|---:|
| Historical test cases executed | 26 / 26 |
| Automated passed | 23 |
| Automated failed | 0 |
| Manual / partial | 3 (TC-19, TC-21, TC-26) |

### Fresh Windows validation

The application was compiled and launched on the latest `main`. The quiz topics and result-menu paths were manually exercised.

| Validation item | Result |
|---|---|
| Windows compilation | PASS |
| Application launch | PASS |
| Main menu displayed | PASS |
| Topic selection menu displayed | PASS |
| ML quiz selected and started | PASS |
| DBMS and other available topics selected and started | PASS |
| Questions displayed and answers accepted | PASS |
| Invalid numeric and blank answer input rejected | PASS |
| Correct/incorrect answer feedback displayed | PASS |
| Score displayed at quiz completion | PASS |
| Result menu displayed | PASS |
| Retry this topic | PASS |
| Return to topic selection | PASS |
| Return to main menu | PASS |

## 4. Test Case Results (TC-01 to TC-26: Historical)

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
| TC-12 | OQ-F-012 | Added question saved and loaded (6 questions) | PASS* |
| TC-13 | OQ-F-013 | Updated question text and answer used in quiz | PASS* |
| TC-14 | OQ-F-014 | Confirmed deletion leaves 5 questions | PASS* |
| TC-15 | OQ-F-015 | Admin operations inaccessible without login; data unchanged | PASS* |
| TC-16 | OQ-F-016 | Retry resets to Q1 and score 0 | PASS |
| TC-17 | OQ-NF-001 | 20 inputs processed in 14 ms total | PASS |
| TC-18 | OQ-NF-002 | Score 3 repeated consistently across 3 runs | PASS |
| TC-19 | OQ-NF-003 | Three new users complete quiz without extra help | MANUAL / PENDING |
| TC-20 | OQ-NF-004 | Answers hidden before answering; no admin in user menu | PASS |
| TC-21 | OQ-NF-005 | Windows MinGW and Linux GCC portability checks | PARTIAL |
| TC-22 | OQ-SR-001 | Login shown first; management only after login | PASS |
| TC-23 | OQ-SR-002 | Invalid credentials rejected; correct login succeeds | PASS* |
| TC-24 | OQ-SR-003 | Invalid question records rejected; file unchanged | PASS |
| TC-25 | OQ-SR-004 | Invalid admin/menu values rejected | PASS |
| TC-26 | OQ-SR-005 | Password output check passed; terminal masking manual | PARTIAL |

*Historical report recorded the behaviour as passing; wording differed from the test-plan wording in TC-15 and TC-23. TC-12 to TC-14 are historical results and are not claimed as newly rerun on 12 November.

## 5. Fresh Test Case Results (TC-27 to TC-32)

| TC | Validation area | Procedure / expected result | Observed result | Status |
|---|---|---|---|---|
| TC-27 | Topic menu display | Open Take Quiz; all topic choices and return option are displayed | ML, DBMS, SE, DSA, GK and return option displayed | PASS |
| TC-28 | Topic selection | Select ML and verify quiz starts | ML quiz started and questions displayed | PASS |
| TC-29 | Topic selection | Select DBMS and verify quiz starts | DBMS quiz started successfully | PASS |
| TC-30 | Topic selection | Select SE, DSA and GK and verify each starts | All selected topics started successfully | PASS |
| TC-31 | Result menu – retry | Finish a quiz and select Retry this topic | Quiz restarted for the selected topic | PASS |
| TC-32 | Result menu – navigation | Select Return to topic selection, then return to main menu | Topic selection and main menu displayed as expected | PASS |

## 6. Manual Tests Pending / Limitations

- **TC-19:** Recruit three new users and record whether they complete a quiz without extra help.
- **TC-21:** Linux GCC execution has not been freshly performed as part of the 12 November validation. Windows was freshly compiled and exercised.
- **TC-26:** Verify that password characters are masked with `*` while typing in a real terminal; this remains manual/pending unless separately evidenced.
- The fresh validation covered quiz/topic/result-menu behaviour. It did not rerun every admin, security, response-time, or reliability test from TC-01 to TC-26.

## 7. Defects and Deviations

| ID / Issue | Severity | Status | Details |
|---|---|---|---|
| Admin-added question topic not saved (PR #50) | Medium | FIXED | Fixed in PR #50 and verified in the historical regression run |
| `test_question_loader` issue (PR #56) | — | FIXED | Team leader reported the test issue fixed by PR #56; no new independent test result is claimed here |
| D-03: newline-related bug | Medium | OPEN | Remains unresolved; track as an open defect |

Additional notes:
- A uniform answer key was used in historical test fixtures because questions are displayed in random order.
- TC-15 and TC-23 application wording differs from the test plan, while observed access-control behaviour matches.
- The fresh Windows validation did not include a Linux run.

## 8. Exit Criteria

| Criterion | Status |
|---|---|
| 32 test cases documented (TC-01 to TC-32) | Met |
| Historical 26-case execution recorded | Met |
| Fresh latest-main validation recorded with commit hash | Met |
| Fresh topic-selection and result-menu cases TC-27 to TC-32 passed | Met |
| All high-priority and security automated cases passed | Historical run recorded as passed; TC-26 masking check pending |
| No open critical defect | Met (D-03 is Medium and remains open) |
| Manual TC-19, TC-21, and TC-26 completed | Pending |
| RTM result columns updated | Pending confirmation/update |

## 9. Conclusion

The historical regression run dated 30 October 2026 recorded 23 automated passes and no automated failures across the 26-case suite, with TC-19, TC-21, and TC-26 requiring manual completion or follow-up.

On 12 November 2026, fresh Windows validation was performed against commit `6b59415` on `main`. Compilation and application launch succeeded, and topic selection, quiz execution, scoring/result display, retry, return to topic selection, and return to the main menu were verified successfully. TC-27 to TC-32 passed in this fresh validation.

D-03 remains an open Medium defect. TC-19, Linux execution under TC-21, terminal masking under TC-26, and RTM updates remain follow-up items.

**Source records:** `docs/test_execution_log.md`, `docs/test_summary_report.md` (historical execution dated 30-10-2026), and fresh Windows manual validation recorded on 12-11-2026 against commit `6b59415`.
