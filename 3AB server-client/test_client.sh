#!/bin/bash

echo -e "make:"
make

usage="[./client] USAGE: client [-p PORT] [ -o FILE | -d DIR ] URL"
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

    if [ "$output_stderr" == "$expected_stderr" ]; then
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
}


#test_command "01" "./client" "" "$usage" 1
#test_command "02" "./client -p abc > /dev/null" "" "$usage" 1
#test_command "03" "./client -p 80x http://localhost/ > /dev/null" "" "getaddrinfo failed" 1
#test_command "04" "./client -p 80 -p 81 http://localhost/ > /dev/null" "" "$usage" 1
#test_command "05" "./client -a http://localhost/ > /dev/null" "" "$usage" 1

mkdir tests
url=http://www.example.com/
wget $url -q -O tests/example.html

test_command "A01.1" "./client $url > tests/test_A01.html" "" "" "0"
test_command "A01.2" "diff tests/example.html tests/test_A01.html" "" "" "0"

test_command "A02.1" "./client -o tests/test_A02.html $url" "" "" "0"
test_command "A02.2" "diff tests/example.html tests/test_A02.html" "" "" "0"

test_command "A03.1" "./client -d tests/ $url" "" "" "0"
test_command "A03.2" "diff tests/example.html tests/index.html" "" "" "0"

rm tests -r

test_command "A03.1" "./client -d tests/ $url" "" "" "0"
wget $url -q -O tests/example.html
test_command "A03.2" "diff tests/example.html tests/index.html" "" "" "0"

rm tests -r

test_command "A03.1" "./client -d tests $url" "" "" "0"
wget $url -q -O tests/example.html
test_command "A03.2" "diff tests/example.html tests/index.html" "" "" "0"




















rm tests -r

####################Check if everything had been correct:
if [ "$EVERYTHING_CORRECT" == "false" ]; then
  echo -e "\n\n\nThere has been at least one error"
else
  echo -e "\n\n\nThere have been no errors. Good Job!"
fi


echo -e "\nmake clean:"
make clean
