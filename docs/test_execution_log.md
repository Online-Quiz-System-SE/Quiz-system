# Test Execution Log - Online Quiz System

| Item | Value |
|---|---|
| Issue | QZ-23 (#25) |
| Test plan | Software Test Plan v1.0, section 13.2 |
| Date | 2026-10-30 23:57 |
| Commit | 030af1c (feature/QZ-23-system-tests) |
| OS | Darwin 25.6.0 |
| Compiler | Apple clang version 21.0.0 (clang-2100.1.1.101) |
| Build | OK, 0 warnings (-std=c++17 -Wall -Wextra) |
| Script | tests/system/run_system_tests.sh |

## Results

| TC | Requirement | Priority | Result | Notes |
|---|---|---|---|---|
| TC-01 | OQ-F-001 | High | PASS | Quiz started, Question 1 shown, score started at 0 |
| TC-02 | OQ-F-002 | High | PASS | One question at a time |
| TC-03 | OQ-F-003 | High | PASS | Every question shows exactly options A-D |
| TC-04 | OQ-F-004 | High | PASS | Valid answer B accepted |
| TC-05 | OQ-F-005 | Medium | PASS | E, 5, @, AB and empty input rejected; A accepted |
| TC-06 | OQ-F-006 | High | PASS | Q1 (B) marked correct, Q2 (A) marked incorrect |
| TC-07 | OQ-F-007 | High | PASS | Score increases by exactly one per correct answer |
| TC-08 | OQ-F-008 | High | PASS | Wrong answer left the score unchanged (4 of 5) |
| TC-09 | OQ-F-009 | High | PASS | Stays on Q1 after E, moves to Q2 after A |
| TC-10 | OQ-F-010 | High | PASS | answers_3of5 gives final score 3 |
| TC-11 | OQ-F-011 | High | PASS | Result screen shows Your score: 3 / 5 |
| TC-12 | OQ-F-012 | Medium | PASS | Question saved (6 questions) and loaded |
| TC-13 | OQ-F-013 | Medium | PASS | Q2 text and answer updated and used in the quiz |
| TC-14 | OQ-F-014 | Medium | PASS | Q6 deleted after Y confirmation; 5 remain |
| TC-15 | OQ-F-015 | High, Security | PASS | Add/modify/delete unreachable without login; data unchanged. Note: app shows Invalid menu choice (STP wording: Access denied) |
| TC-16 | OQ-F-016 | Medium | PASS | Retry starts from Q1 with score reset to 0 |
| TC-17 | OQ-NF-001 | Medium | PASS | 20 inputs processed in 14 ms total (each well under 1 s) |
| TC-18 | OQ-NF-002 | High | PASS | Score 3 on all 3 runs; one final score per run |
| TC-19 | OQ-NF-003 | Medium | MANUAL | Needs 3 new users; record results in RTM |
| TC-20 | OQ-NF-004 | High, Security | PASS | No answers before answering; no admin functions in user menu |
| TC-21 | OQ-NF-005 | Medium | PARTIAL | Built and ran on Darwin with clang++; Windows MinGW and Linux GCC to be checked manually |
| TC-22 | OQ-SR-001 | High, Security | PASS | Login prompt shown first; management only after login |
| TC-23 | OQ-SR-002 | High, Security | PASS | Wrong password, wrong username and empty fields rejected; correct login works |
| TC-24 | OQ-SR-003 | Medium, Security | PASS | Empty text, missing 4th option and answer E rejected; file unchanged |
| TC-25 | OQ-SR-004 | High, Security | PASS | admin, 9, 99, -1 rejected; menu numbers rejected during quiz |
| TC-26 | OQ-SR-005 | Medium, Security | PARTIAL | Passwords never appear in output (automated). Masking with * must be checked by hand in a real terminal |

## Summary

- Executed: 26 / 26
- Passed: 23
- Failed: 0
- Manual or partial: 3
- Exit criteria: All automated High-priority and security cases passed

## Test data and deviations

- Each case uses a temporary copy of data/questions.txt (5 or 6 ML fixture questions, every correct answer B), so the real question bank is never modified.
- Questions are shown in random order, so a fixed per-position answer key (STP: Q1 = B, Q2 = C) cannot be scripted; a uniform key B keeps every scripted outcome deterministic.
- Admin tests use the test credentials admin / admin123.
- TC-15 and TC-23: the application wording differs from the STP expected text, but the behaviour (no access, no data change) matches.
- TC-19, TC-21 and the masking part of TC-26 need manual execution.
