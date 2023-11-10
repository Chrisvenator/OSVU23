#!/bin/bash

echo -e "make:"
make

###################### Command 1
echo -e "\ntestcase 01:"
./mycompress -x 2> /dev/null
output=$(echo $?)
expected_output="1"
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 2
echo -e "testcase 02:"
./mycompress -o file -o file 2> /dev/null
output=$(echo $?)
expected_output="1"
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 2
echo -e "testcase 03:"
./mycompress -o 2> /dev/null
output=$(echo $?)
expected_output="1"
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 4
echo -e "testcase 04:"
output=$(echo -e "aaaaaaaaa" | ./mycompress 2> stderr)
expected_output="a9"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 10 characters
Written: 3 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm stderr 


###################### Command 5
echo -e "testcase 05:"
output=$(echo -e "aaaaaaaaabbbccccc\ndddDDD\nE" | ./mycompress 2> stderr)
expected_output="a9b3c5
d3D3
E1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 27 characters
Written: 15 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm stderr 


###################### Command 6
echo -e "testcase 06:"
output=$(echo -e "aaaaaaaaaaaaaaaaaaaa\nBBB\n\nCCC" | ./mycompress 2> stderr)
expected_output="a20
B3

C3"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 30 characters
Written: 11 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm stderr 


###################### Command 7
echo -e "testcase 07:"
output=$(echo -e "abcdefghijkl123" | ./mycompress 2> stderr)
expected_output="a1b1c1d1e1f1g1h1i1j1k1l1112131"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 16 characters
Written: 31 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm stderr 


###################### Command 8
echo -e "testcase 08:"
echo -e "TEST\nxxxxxxxx" > infile.txt
./mycompress -o outfile.txt infile.txt 2> stderr
output=$(cat outfile.txt)
expected_output="T1E1S1T1
x8"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 14 characters
Written: 12 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm stderr 
rm infile.txt
rm outfile.txt


###################### Command 9
echo -e "testcase 09:"
echo 'Is .,:-!=?% %n%% compressed???' > infile1
echo -e "no, not very" > infile2
output=$(./mycompress infile1 infile2 2> stderr)
expected_output="I1s1 1.1,1:1-1!1=1?1%1 1%1n1%2 1c1o1m1p1r1e1s2e1d1?3
n1o1,1 1n1o1t1 1v1e1r1y1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 44 characters
Written: 78 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm stderr 
rm infile1
rm infile2


###################### Command 10
echo -e "testcase 10:"
echo "some old content" > outfile
echo -e "Lorem ipsuuuuuuuum, ....." | ./mycompress -o outfile 2> stderr
output=$(cat outfile)
expected_output="L1o1r1e1m1 1i1p1s1u8m1,1 1.5"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 26 characters
Written: 29 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm outfile


###################### Command 11
echo -e "testcase 11:"
( echo -n "X"; printf -- "-%.0s" {1..8000}; echo "x" ) > longline
./mycompress longline > longcompressed 2> stderr
output=$(cat longcompressed)
expected_output="X1-8000x1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

# Now check the stderr file
expected_stderr="Read: 8003 characters
Written: 10 characters" 
actual_stderr=$(<stderr)

# Check if stderr is correct
if [ "$actual_stderr" == "$expected_stderr" ]; then
  echo "stderr is correct."
else
  echo "stderr is wrong."
  echo "Expected: $expected_stderr"
  echo "Got: $actual_stderr"
fi

# Clean up (if necessary)
rm stderr 
rm longline
rm longcompressed

###################### Command 12
echo -e "testcase 11:"
rm -rf nonExistingTestfile 
./mycompress nonExistingTestfile 2> /dev/null
output=$(echo $?)
expected_output="1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 13
echo -e "testcase 11:"
touch existingTestfile
chmod 0000 existingTestfile 
echo "test" > existingTestfile
echo "test" | ./mycompress -o existingTestfile 2> /dev/null

output=$(echo $?)

expected_output="1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "message is (probably) correct."
else
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

chmod 7777 existingTestfile
rm existingTestfile
































echo -e "\nmake clean:"
make clean
