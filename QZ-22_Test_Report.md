# QZ-22: Cross-Platform Build and Quiz Verification Report

**Project:** Online Quiz System  
**Test ID:** QZ-22  
**Tested commit:** `6b59415`  
**Validation date:** 12 November 2026  
**Tester:** Sunil  

## 1. Objective

Verify that the Online Quiz System builds and runs correctly on Windows and Linux, including quiz-topic selection, question handling, scoring, and navigation/result-menu options.

## 2. Test Environment

| Platform | Environment | Compiler |
|---|---|---|
| Windows | Windows PowerShell | MinGW `g++` |
| Linux | Ubuntu terminal | `g++` |

## 3. Build Commands

**Windows (PowerShell)**

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude (Get-ChildItem src -Filter *.cpp | ForEach-Object { $_.FullName }) -o quiz-windows.exe
```

**Linux**

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o quiz-linux
```

## 4. Test Results

| TC | Test case | Windows | Linux |
|:--:|---|:--:|:--:|
| 1 | Latest commit checked (`6b59415`) | PASS | PASS |
| 2 | Compilation without errors | PASS | PASS |
| 3 | Program launches | PASS | PASS |
| 4 | Main menu displayed | PASS | PASS |
| 5 | Take Quiz opens topic selection | PASS | PASS |
| 6 | Machine Learning (ML) quiz completed | PASS | PASS |
| 7 | Database Management Systems (DBMS) quiz completed | PASS | PASS |
| 8 | Software Engineering (SE) quiz completed | PASS | PASS |
| 9 | Data Structures and Algorithms (DSA) quiz completed | PASS | PASS |
| 10 | General Knowledge (GK) quiz completed | PASS | PASS |
| 11 | Five questions displayed per quiz | PASS | PASS |
| 12 | Answers accepted | PASS | PASS |
| 13 | Correct/incorrect feedback displayed | PASS | PASS |
| 14 | Score displayed after quiz | PASS | PASS |
| 15 | Retry current topic | PASS | PASS |
| 16 | Return to topic selection | PASS | PASS |
| 17 | Return to main menu | PASS | PASS |
| 18 | Exit program cleanly | PASS | PASS |

## 5. Observations

- The application compiled successfully on both platforms using C++17.
- All five quiz topics (ML, DBMS, SE, DSA, and GK) were selected and completed.
- Each quiz displayed five questions, accepted answers, provided correctness feedback, and displayed a final score.
- Retry, return-to-topic-selection, return-to-main-menu, and clean-exit paths were verified.
- Scores vary according to the answers entered and are not expected to be identical between runs.

## 6. Conclusion

**QZ-22: PASS.** The application built and ran successfully on Windows and Linux for commit `6b59415`. All five quiz topics and the tested quiz navigation/result-menu paths passed validation.

## 7. Test Evidence

- **Windows:** Terminal evidence of compilation, quiz execution, topic selection, and result-menu navigation.
- **Linux:** Terminal evidence of compilation, quiz execution for all five topics, and navigation/result-menu checks.
