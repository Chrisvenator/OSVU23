#!/bin/bash

echo -e "make:"
make

usage="[./server] USAGE: server [-p PORT] [-i INDEX] DOC_ROOT"
valgrind="valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ";
EVERYTHING_CORRECT="true"


# Argument 1: test case number
# Argument 2: command to test
# Argument 3: expected stdout
# Argument 4: expected stderr
# Argument 5: expected exit code
function test_command() {
    echo -e "\nTest case $1:"
    local command="$2"
    local expected_stdout="$3"
    local expected_stderr="$4"
    local expected_exit_status="$5"

    # Capture stdout and stderr separately
    local output_stdout=$(eval "$command" 2>/dev/null)
    local output_stderr=$(eval "$command" 2>&1 >/dev/null)

    eval "$command" 2>/dev/null
    local exit_status="$?"


#    echo "stdout:"
#    echo "$output_stdout"

    if [ "$output_stdout" == "$expected_stdout" ]; then
      echo "correct stdout"
    else
      EVERYTHING_CORRECT="false"
      echo "wrong stdout!"
      echo "Expected: $expected_stdout"
      echo "Got: $output_stdout"
    fi

#    echo "stderr:"
#    echo "$output_stderr"

    if [[ "$output_stderr" == *"$expected_stderr"* ]]; then
      echo "correct stderr"
    else
      EVERYTHING_CORRECT="false"
      echo "wrong stderr!"
      echo "Expected: $expected_stderr"
      echo "Got: $output_stderr"
    fi

    if [ "$exit_status" -eq "$expected_exit_status" ]; then
      echo "correct exit status."
    else
      EVERYTHING_CORRECT="false"
      echo "wrong exit status!"
      echo "Expected: $expected_exit_status"
      echo "Got: $exit_status"
    fi

    if grep -q "All heap blocks were freed -- no leaks are possible" "log.txt"; then
        echo "All heap blocks were freed -- no leaks are possible"
    else
        EVERYTHING_CORRECT="false"
        echo "ERROR: MEMORY LEAK:"
        cat "log.txt"
    fi

    echo "All heap blocks were freed -- no leaks are possible">log.txt
}


test_command "01" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server" "" "$usage" 1
test_command "02" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server -p 123a ./" "" "$usage" 1
test_command "03" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server -p \"\" ./" "" "$usage" 1
test_command "04" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server -p \"\" ./" "" "$usage" 1
test_command "05" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server -p -1 ./" "" "$usage" 1
test_command "06" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server -p 66666 ./" "" "$usage" 1

test_command "07" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server -i /dev/null/null ./" "" "$usage" 1

echo "" > test.txt
chmod 0000 test.txt
test_command "08" "valgrind --tool=memcheck --leak-check=full --log-file=log.txt --child-silent-after-fork=yes --error-exitcode=42 -s ./server -i test.txt ./" "" "$usage" 1
rm -rf test.txt




















rm log.txt

####################Check if everything had been correct:
if [ "$EVERYTHING_CORRECT" == "false" ]; then
  echo -e "\n\n\nThere has been at least one error"
else
  echo -e "\n\n\nThere have been no errors. Good Job!"
fi


echo -e "\nmake clean:"
make clean
