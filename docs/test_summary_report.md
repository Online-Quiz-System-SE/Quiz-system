# Test Summary Report - Online Quiz System v1.0

| Item | Value |
|---|---|
| Project | Online Quiz System |
| Test plan | Software Test Plan v1.0 (IEEE 829 based) |
| Requirements | SRS v1.1 (16 functional, 5 non-functional, 5 security) |
| Authors | Debangana, Chinmay, Satvik, Sunil |
| Date | 30-10-2026 |
| Build | `main` after PR #50, compiled with `-std=c++17 -Wall -Wextra`, 0 warnings |
| Detailed results | `docs/test_execution_log.md` |

## 1. Summary

All 26 test cases were executed. After one defect fix, every automated test case passes,
and all High-priority and security test cases pass. The exit criteria of STP section 5 are
met once the three manual checks below are recorded.

## 2. Test execution

| Run | Date | Build | Passed | Failed | Manual / partial |
|---|---|---|---|---|---|
| 1 | 30-10-2026 | before PR #50 | 22 | 1 (TC-12) | 3 |
| 2 (regression) | 30-10-2026 | after PR #50 | 23 | 0 | 3 |

| Metric | Value |
|---|---|
| Test cases executed | 26 / 26 (100%) |
| Automated test cases passed | 23 / 23 (100%) |
| Manual / partial test cases | 3 (TC-19, TC-21, TC-26) |
| High-priority and security cases passed | all |
| Requirement coverage | 26 / 26 requirements (100%) |

## 3. Results by requirement group

| Group | Test cases | Result |
|---|---|---|
| Quiz operation (OQ-F-001 to OQ-F-011, OQ-F-016) | TC-01 to TC-11, TC-16 | 12 / 12 pass |
| Question management (OQ-F-012 to OQ-F-014) | TC-12 to TC-14 | 3 / 3 pass (TC-12 after fix) |
| Access control (OQ-F-015) | TC-15 | pass |
| Non-functional (OQ-NF-001 to OQ-NF-005) | TC-17 to TC-21 | TC-17, TC-18, TC-20 pass; TC-19, TC-21 manual |
| Security (OQ-SR-001 to OQ-SR-005) | TC-22 to TC-26 | TC-22 to TC-25 pass; TC-26 output check pass, masking manual |

Response time (TC-17): 20 inputs processed in 15 ms in total, well within 1 second per input.
Reliability (TC-18): the same answer script gave a score of 3 on all three runs.

## 4. Manual test cases

| TC | Check | Result | Tester / date |
|---|---|---|---|
| TC-19 | Three new users complete a quiz without extra help | [fill in] | [fill in] |
| TC-21 | Builds and runs on Windows (MinGW) and Linux (GCC) | [fill in] | [fill in] |
| TC-26 | Password shown as `*` while typing in a real terminal | [fill in] | [fill in] |

## 5. Defects

| ID | Description | Severity | Found by | Status | Days open |
|---|---|---|---|---|---|
| [issue #] | Admin-added questions were saved without their topic, so they never appeared in a quiz | Medium | TC-12 | Fixed in PR #50, verified by regression run | 0 |
| [issue #] | `tests/test_question_loader.cpp` fails on `main` (expects 2 questions, loader rejects 5 of 6 records as malformed). Not caused by any test-cycle change | Low | Unit test run | Open | 0 |

Defects by severity: Critical 0, High 0, Medium 1 (fixed), Low 1 (open).
No critical defect is open.

## 6. Deviations

Deviations from the SRS, SAD and STP found during testing are recorded in `docs/deviations.md`.
The main test-related ones: a uniform answer key in the test fixture because questions are
shown in random order (STP-D2, STP-D3), different message wording in TC-15 and TC-23
(STP-D5, STP-D6), and automated execution on macOS with Windows and Linux checked manually
(STP-D8).

## 7. Exit criteria (STP section 5)

| Criterion | Status |
|---|---|
| All 26 test cases executed | Met |
| All High-priority and security test cases passed | Met (TC-26 masking confirmed manually) |
| No open critical defect | Met |
| RTM updated with results | [Met once the STP and SRS RTM Result columns are updated] |

## 8. Recommendation

The system meets its tested requirements and is ready for the acceptance demo, provided
the manual results in section 4 are recorded. Before any use outside the course, the
hard-coded test credentials should be replaced (deviation SRS-D7).

