#!/usr/bin/env bash
# QZ-23: Scripted system tests TC-01 to TC-26
# Source: Software Test Plan v1.0, section 13.2. Requirements: SRS v1.1.
# Runs the real quiz binary with scripted input (./quiz < input), as in STP section 5.
# Usage (from the repo root): bash tests/system/run_system_tests.sh

set -u

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
WORK="$(mktemp -d)"
BIN="$WORK/quiz"
QFILE="$WORK/data/questions.txt"
LOG="$ROOT/docs/test_execution_log.md"
trap 'rm -rf "$WORK"' EXIT
mkdir -p "$WORK/data" "$ROOT/docs"

# ---------------------------------------------------------------- build
CXX="${CXX:-$(command -v clang++ || command -v g++)}"
echo "Building with $CXX ..."
if ! "$CXX" -std=c++17 -Wall -Wextra -I"$ROOT/include" "$ROOT"/src/*.cpp \
        -o "$BIN" 2> "$WORK/build.log"; then
    cat "$WORK/build.log"
    echo "BUILD FAILED: testing suspended (STP section 12)."
    exit 1
fi
WARNINGS=$(grep -c "warning:" "$WORK/build.log")
echo "Build OK ($WARNINGS warnings)"
echo

# ---------------------------------------------------------------- helpers
OUT=""
ROWS=""
PASS=0; FAIL=0; OTHER=0; HIGH_FAILED=""

write_fixture() {   # $1 = number of ML questions, every correct answer is B
    : > "$QFILE"
    for i in $(seq 1 "$1"); do
        echo "ML|Fixture question $i|Option A$i|Option B$i|Option C$i|Option D$i|B" >> "$QFILE"
    done
    cp "$QFILE" "$WORK/backup.txt"
}

run() {             # $1 = scripted input (printf %b format)
    OUT="$(cd "$WORK" && printf '%b' "$1" | "$BIN" 2>&1)"
}

now_ms()     { perl -MTime::HiRes=time -e 'printf "%d\n", time*1000'; }
count()      { printf '%s' "$OUT" | grep -oF -- "$1" | wc -l | tr -d ' '; }
has()        { [ "$(count "$1")" -gt 0 ]; }
absent()     { [ "$(count "$1")" -eq 0 ]; }
eq()         { [ "$1" = "$2" ]; }
lt()         { [ "$1" -lt "$2" ]; }
before()     { local pre="${OUT%%"$2"*}"; [ "$pre" != "$OUT" ] || return 1
               case "$pre" in *"$1"*) return 0;; esac; return 1; }
not_in()     { case "$2" in *"$1"*) return 1;; esac; return 0; }
file_has()   { grep -qF -- "$1" "$QFILE"; }
file_lacks() { ! grep -qF -- "$1" "$QFILE"; }
file_lines() { wc -l < "$QFILE" | tr -d ' '; }
unchanged()  { cmp -s "$QFILE" "$WORK/backup.txt"; }

begin()  { OK=1; NOTES=""; }
expect() { local d="$1"; shift; if ! "$@"; then OK=0; NOTES="${NOTES}FAILED: $d. "; fi; }

record() {          # id req priority result notes
    ROWS="${ROWS}| $1 | $2 | $3 | $4 | $5 |
"
    case "$4" in
        PASS) PASS=$((PASS + 1)) ;;
        FAIL) FAIL=$((FAIL + 1))
              case "$3" in High*|*Security*) HIGH_FAILED="$HIGH_FAILED $1" ;; esac ;;
        *)    OTHER=$((OTHER + 1)) ;;
    esac
    printf '%-6s %-8s %s\n' "$1" "$4" "$5"
}
finish() { if [ "$OK" -eq 1 ]; then record "$1" "$2" "$3" PASS "$4"
           else record "$1" "$2" "$3" FAIL "$NOTES"; fi; }

# ---------------------------------------------------------------- scripts
QUIZ='2\n1\n'                      # main menu: Take Quiz, topic: ML
ALLB='B\nB\nB\nB\nB\n'
ENDQ='0\n0\n'                      # after quiz: main menu, then Exit
LOGIN='1\nadmin\nadmin123\n'       # admin login with test credentials
ANS3="${QUIZ}B\nB\nB\nA\nA\n${ENDQ}"   # answers_3of5: 3 correct, 2 wrong

# ================================================================ TC-01
write_fixture 5; begin
run "${QUIZ}${ALLB}${ENDQ}"
expect "quiz starts for the selected topic" has "Topic: ML"
expect "Question 1 is displayed" has "Question 1 of 5"
expect "score starts at 0 (5 correct gives 5/5)" has "Your score: 5 / 5"
finish TC-01 OQ-F-001 High "Quiz started, Question 1 shown, score started at 0"

# ================================================================ TC-02
begin
run "${QUIZ}"
expect "only Question 1 shown before answering" has "Question 1 of 5"
expect "Question 2 not shown before answering" absent "Question 2 of 5"
run "${QUIZ}B\n"
expect "Question 2 shown after answering Q1" has "Question 2 of 5"
expect "Question 3 not shown yet" absent "Question 3 of 5"
finish TC-02 OQ-F-002 High "One question at a time"

# ================================================================ TC-03
begin
run "${QUIZ}${ALLB}${ENDQ}"
for L in A B C D; do
    expect "option $L shown for all 5 questions" eq "$(count "  $L. ")" 5
done
expect "no fifth option" absent "  E. "
finish TC-03 OQ-F-003 High "Every question shows exactly options A-D"

# ================================================================ TC-04
begin
run "${QUIZ}B\n"
expect "B accepted and processed" has "Correct! Your answer is right."
expect "no invalid-option error" absent "Invalid option"
expect "quiz moves on" has "Question 2 of 5"
finish TC-04 OQ-F-004 High "Valid answer B accepted"

# ================================================================ TC-05
begin
run "${QUIZ}E\n5\n@\nAB\n\nA\n"
expect "5 invalid inputs rejected" eq "$(count "Invalid option. Enter A, B, C, or D.")" 5
expect "A then accepted" eq "$(count "Incorrect!")" 1
expect "Question 1 stayed active" before "Invalid option" "Question 2 of 5"
run "${QUIZ}E\n5\n@\nAB\n\nB\nB\nB\nB\nB\n${ENDQ}"
expect "score unchanged by invalid inputs" has "Your score: 5 / 5"
finish TC-05 OQ-F-005 Medium "E, 5, @, AB and empty input rejected; A accepted"

# ================================================================ TC-06
begin
run "${QUIZ}B\nA\n"
expect "correct answer marked correct" eq "$(count "Correct! Your answer is right.")" 1
expect "wrong answer marked incorrect" eq "$(count "Incorrect!")" 1
expect "Q1 correct before Q2 incorrect" before "Correct! Your answer is right." "Incorrect!"
finish TC-06 OQ-F-006 High "Q1 (B) marked correct, Q2 (A) marked incorrect"

# ================================================================ TC-07
begin
run "${QUIZ}A\nA\nA\nA\nA\n${ENDQ}"
expect "all wrong gives 0" has "Your score: 0 / 5"
run "${QUIZ}B\nA\nA\nA\nA\n${ENDQ}"
expect "one correct answer adds exactly 1" has "Your score: 1 / 5"
finish TC-07 OQ-F-007 High "Score increases by exactly one per correct answer"

# ================================================================ TC-08
begin
run "${QUIZ}B\nA\nB\nB\nB\n${ENDQ}"
expect "wrong answer does not change the score" has "Your score: 4 / 5"
finish TC-08 OQ-F-008 High "Wrong answer left the score unchanged (4 of 5)"

# ================================================================ TC-09
begin
run "${QUIZ}E\nA\n"
expect "E rejected" eq "$(count "Invalid option")" 1
expect "quiz stays on Q1 after E" before "Invalid option" "Question 2 of 5"
expect "Question 2 shown after A" has "Question 2 of 5"
expect "no skip ahead" absent "Question 3 of 5"
finish TC-09 OQ-F-009 High "Stays on Q1 after E, moves to Q2 after A"

# ================================================================ TC-10
begin
run "$ANS3"
expect "3 correct" eq "$(count "Correct! Your answer is right.")" 3
expect "2 incorrect" eq "$(count "Incorrect!")" 2
expect "final score is 3" has "Your score: 3 / 5"
finish TC-10 OQ-F-010 High "answers_3of5 gives final score 3"

# ================================================================ TC-11
begin
expect "completion message" has "Quiz completed!"
expect "final score shown" has "Your score: 3 / 5"
finish TC-11 OQ-F-011 High "Result screen shows Your score: 3 / 5"

# ================================================================ TC-12
write_fixture 5; begin
run "${LOGIN}1\n1\nWhich keyword declares a constant in C++?\nconst\nstatic\nvolatile\nmutable\nA\n4\n5\n0\n"
expect "success message" has "Question added successfully."
expect "file has 6 questions" eq "$(file_lines)" 6
expect "saved with topic in TOPIC-pipe format" file_has "ML|Which keyword declares a constant in C++?|"
expect "question appears in the loaded bank" has "[ML] Which keyword declares a constant in C++?"
finish TC-12 OQ-F-012 Medium "Question saved (6 questions) and loaded"

# ================================================================ TC-13
write_fixture 5; begin
run "${LOGIN}2\n2\n1\nModified question TC-13\nNew A\nNew B\nNew C\nNew D\nC\n5\n${QUIZ}C\nC\nC\nC\nC\n${ENDQ}"
expect "success message" has "Question modified successfully."
expect "new text and answer saved" file_has "ML|Modified question TC-13|New A|New B|New C|New D|C"
expect "old Q2 replaced" file_lacks "Fixture question 2|"
expect "updated text used in the quiz" has "Modified question TC-13"
expect "updated answer C used in scoring" has "Your score: 1 / 5"
finish TC-13 OQ-F-013 Medium "Q2 text and answer updated and used in the quiz"

# ================================================================ TC-14
write_fixture 6; begin
run "${LOGIN}3\n6\nY\n5\n${QUIZ}${ALLB}${ENDQ}"
POST="${OUT#*Admin logged out successfully.}"
expect "success message" has "Question deleted successfully."
expect "5 questions remain" eq "$(file_lines)" 5
expect "Q6 removed from file" file_lacks "Fixture question 6|"
expect "Q6 not in the quiz" not_in "Fixture question 6" "$POST"
finish TC-14 OQ-F-014 Medium "Q6 deleted after Y confirmation; 5 remain"

# ================================================================ TC-15
write_fixture 5; begin
run "3\n4\nadd\nmodify\ndelete\n0\n"
expect "every attempt rejected" eq "$(count "Invalid menu choice.")" 5
expect "admin menu never opened" absent "ADMIN MENU"
expect "no question-management option reachable" absent "Add a question"
expect "question data unchanged" unchanged
finish TC-15 OQ-F-015 "High, Security" "Add/modify/delete unreachable without login; data unchanged. Note: app shows Invalid menu choice (STP wording: Access denied)"

# ================================================================ TC-16
begin
run "${QUIZ}B\nB\nB\nA\nA\n1\nA\nA\nA\nA\nA\n${ENDQ}"
expect "first attempt scored 3" has "Your score: 3 / 5"
expect "restart begins at Question 1" eq "$(count "Question 1 of 5")" 2
expect "score reset (second attempt 0, not 3)" has "Your score: 0 / 5"
expect "restart happens after the result" before "Your score: 3 / 5" "Your score: 0 / 5"
finish TC-16 OQ-F-016 Medium "Retry starts from Q1 with score reset to 0"

# ================================================================ TC-17
begin
IN20="${QUIZ}${ALLB}1\n${ALLB}2\n1\n${ALLB}${ENDQ}"   # 20 inputs before exit
T0=$(now_ms); run "$IN20"; T1=$(now_ms); MS=$((T1 - T0))
expect "3 quizzes completed" eq "$(count "Quiz completed!")" 3
expect "all 20 inputs processed within 1 s in total" lt "$MS" 1000
finish TC-17 OQ-NF-001 Medium "20 inputs processed in ${MS} ms total (each well under 1 s)"

# ================================================================ TC-18
begin
for R in 1 2 3; do
    run "$ANS3"
    expect "run $R scores 3" has "Your score: 3 / 5"
    expect "run $R has exactly one final score" eq "$(count "Your score:")" 1
done
finish TC-18 OQ-NF-002 High "Score 3 on all 3 runs; one final score per run"

# ================================================================ TC-19
record TC-19 OQ-NF-003 Medium MANUAL "Needs 3 new users; record results in RTM"

# ================================================================ TC-20
begin
run "${QUIZ}"
expect "no correct answer shown before answering" absent "Correct answer:"
run "${QUIZ}${ALLB}${ENDQ}"
expect "no question-management options in user flow" absent "Add a question"
expect "no admin menu in user flow" absent "ADMIN MENU"
expect "raw question data not printed" absent "Fixture question 1|"
finish TC-20 OQ-NF-004 "High, Security" "No answers before answering; no admin functions in user menu"

# ================================================================ TC-21
record TC-21 OQ-NF-005 Medium PARTIAL "Built and ran on $(uname -s) with $(basename "$CXX"); Windows MinGW and Linux GCC to be checked manually"

# ================================================================ TC-22
write_fixture 5; begin
run "1\nwrong\nwrong\n0\n"
expect "login prompt appears first" has "ADMIN LOGIN"
expect "no admin menu without valid login" absent "ADMIN MENU"
run "${LOGIN}5\n0\n"
expect "login before admin menu" before "ADMIN LOGIN" "ADMIN MENU"
expect "add/modify/delete only after login" before "Admin login successful." "Add a question"
finish TC-22 OQ-SR-001 "High, Security" "Login prompt shown first; management only after login"

# ================================================================ TC-23
begin
run "1\nadmin\nwrongpass\n1\nwronguser\nadmin123\n1\n\n\n1\n   \n   \n${LOGIN}5\n0\n"
expect "wrong password and wrong username rejected" eq "$(count "Access denied. Invalid username or password.")" 2
expect "empty and whitespace fields rejected" eq "$(count "Username and password cannot be empty.")" 2
expect "only the correct login opens the menu" eq "$(count "ADMIN MENU")" 1
finish TC-23 OQ-SR-002 "High, Security" "Wrong password, wrong username and empty fields rejected; correct login works"

# ================================================================ TC-24
write_fixture 5; begin
run "${LOGIN}1\n1\n\nA\nB\nC\nD\nB\n1\n1\nQuestion with 3 options\nA\nB\nC\n\nB\n1\n1\nQuestion with answer E\nA\nB\nC\nD\nE\n5\n0\n"
expect "3 invalid questions rejected" eq "$(count "Question was not added. Please check your input.")" 3
expect "questions.txt unchanged" unchanged
finish TC-24 OQ-SR-003 "Medium, Security" "Empty text, missing 4th option and answer E rejected; file unchanged"

# ================================================================ TC-25
begin
run "admin\nADMIN\n9\n99\n-1\n${QUIZ}1\nadmin\n${ALLB}${ENDQ}"
expect "hidden and out-of-range menu input rejected" eq "$(count "Invalid menu choice.")" 5
expect "admin input rejected at answer prompt" eq "$(count "Invalid option")" 2
expect "admin menu never opened" absent "ADMIN MENU"
expect "no login prompt reached" absent "ADMIN LOGIN"
finish TC-25 OQ-SR-004 "High, Security" "admin, 9, 99, -1 rejected; menu numbers rejected during quiz"

# ================================================================ TC-26
begin
run "1\nadmin\nwrongpass\n${LOGIN}5\n0\n"
expect "wrong password never printed" absent "wrongpass"
expect "correct password never printed" absent "admin123"
if [ "$OK" -eq 1 ]; then
    record TC-26 OQ-SR-005 "Medium, Security" PARTIAL "Passwords never appear in output (automated). Masking with * must be checked by hand in a real terminal"
else
    record TC-26 OQ-SR-005 "Medium, Security" FAIL "$NOTES"
fi

# ---------------------------------------------------------------- summary + log
TOTAL=$((PASS + FAIL + OTHER))
if [ -z "$HIGH_FAILED" ]; then EXIT_MSG="All automated High-priority and security cases passed"
else EXIT_MSG="FAILED High-priority/security cases:$HIGH_FAILED"; fi

cat > "$LOG" <<EOF
# Test Execution Log - Online Quiz System

| Item | Value |
|---|---|
| Issue | QZ-23 (#25) |
| Test plan | Software Test Plan v1.0, section 13.2 |
| Date | $(date '+%Y-%m-%d %H:%M') |
| Commit | $(cd "$ROOT" && git rev-parse --short HEAD 2>/dev/null) ($(cd "$ROOT" && git rev-parse --abbrev-ref HEAD 2>/dev/null)) |
| OS | $(uname -sr) |
| Compiler | $("$CXX" --version | head -1) |
| Build | OK, $WARNINGS warnings (-std=c++17 -Wall -Wextra) |
| Script | tests/system/run_system_tests.sh |

## Results

| TC | Requirement | Priority | Result | Notes |
|---|---|---|---|---|
${ROWS}
## Summary

- Executed: $TOTAL / 26
- Passed: $PASS
- Failed: $FAIL
- Manual or partial: $OTHER
- Exit criteria: $EXIT_MSG

## Test data and deviations

- Each case uses a temporary copy of data/questions.txt (5 or 6 ML fixture questions, every correct answer B), so the real question bank is never modified.
- Questions are shown in random order, so a fixed per-position answer key (STP: Q1 = B, Q2 = C) cannot be scripted; a uniform key B keeps every scripted outcome deterministic.
- Admin tests use the test credentials admin / admin123.
- TC-15 and TC-23: the application wording differs from the STP expected text, but the behaviour (no access, no data change) matches.
- TC-19, TC-21 and the masking part of TC-26 need manual execution.
EOF

echo
echo "Executed $TOTAL/26 | PASS $PASS | FAIL $FAIL | MANUAL/PARTIAL $OTHER"
echo "$EXIT_MSG"
echo "Log written to docs/test_execution_log.md"
[ "$FAIL" -eq 0 ]
