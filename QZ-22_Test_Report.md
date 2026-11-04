# QZ-22: Windows and Linux Build & Quiz Verification

## Objective
Verify that the Online Quiz System builds and runs correctly on Windows and Linux without special hardware.

## Environment
- Windows: Windows PowerShell with g++ (MinGW)
- Linux: Ubuntu terminal with g++

## Build Commands

### Windows
```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude (Get-ChildItem src -Filter *.cpp | ForEach-Object { $_.FullName }) -o quiz-windows.exe
```

### Linux
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o quiz-linux
```

## Test Results

| Test | Windows | Linux |
|---|---|---|
| Compilation without errors | PASS | PASS |
| Program launches | PASS | PASS |
| Main menu displayed | PASS | PASS |
| Quiz starts | PASS | PASS |
| Five questions displayed | PASS | PASS |
| Answers accepted | PASS | PASS |
| Score displayed | PASS (4/5) | PASS (3/5) |
| Returns to main menu | PASS | PASS |
| Exits with Goodbye | PASS | PASS |

## Conclusion
The Online Quiz System compiled and completed a full quiz run on both Windows and Linux. Five questions were displayed, answers were accepted, scores were shown, and the program exited successfully on both platforms.

## Evidence
- Windows terminal screenshot: build and full quiz run.
- Linux terminal screenshot: build and full quiz run.
