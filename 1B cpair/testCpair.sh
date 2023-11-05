#!/bin/bash

echo -e "make:"
make

EVERYTHING_CORRECT="true"

###################### Command 1
echo -e "\ntestcase 01:"
output=$(echo -e "4 4\n-1 1\n1 -1\n-4 -4" | ./cpair)
expected_output="-1.000 1.000
1.000 -1.000"
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  EVERYTHING_CORRECT="false"
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 2
echo -e "testcase 02:"
output=$(echo -e "13.37 42" | ./cpair)
expected_output=""
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  EVERYTHING_CORRECT="false"
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 3
echo -e "testcase 03:"
output=$(echo -e "91.0 2\n-1.0 7.1" | ./cpair)
expected_output="-1.000 7.100
91.000 2.000"
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  EVERYTHING_CORRECT="false"
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 4
echo -e "testcase 04:"
output=$(echo -e "1.0 -4\n1.0 -44.1" | ./cpair)
expected_output="1.000 -44.100
1.000 -4.000"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 5
echo -e "testcase 05:"
output=$(echo -e "4 90\n4 89\n4 200\n4 -100" | ./cpair)
expected_output="4.000 89.000
4.000 90.000"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi


###################### Command 6
echo -e "testcase 06:"
output=$(echo -e "9.0 7\n8.9 7\n200 7\n-100 7" | ./cpair)
expected_output="8.900 7.000
9.000 7.000"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 7
echo -e "testcase 07:"
output=$(echo -e "3 4\n3 14\n3 4\n3 4.1\n" | ./cpair)
expected_output="3.000 4.000
3.000 4.000"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 8
echo -e "testcase 8:"
echo -e "foo" | ./cpair 2> /dev/null
output=$(echo $?)
expected_output="1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi


###################### Command 9
echo -e "testcase 9:"
echo -e "1.0 2\n4 5 6.0" | ./cpair 2> /dev/null
output=$(echo $?)
expected_output="1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi


###################### Command 10-1
echo -e "testcase 10-1:"
echo "" | ./cpair 2> /dev/null
output=$(echo $?)
expected_output="1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi


###################### Command 10-2
echo -e "testcase 10-2:"

( for i in $(seq 1 10); do
    ax=$(echo "a$i" | md5sum | cut -b 1-3)
    ay=$(echo "b$i" | md5sum | cut -b 1-3)
    bx=$(echo "scale=1;$((16#$ax)) / 10" | bc)
    by=$(echo "scale=1;$((16#$ay)) / 10" | bc)
    echo "$bx $by"
  done
) > testfile10

# Define the expected output from 'head testfile10'
expected_output="189.1 13.5
34.5 151.7
385.7 309.9
75.3 265.6
307.2 346.1
51.4 357.5
70.6 384.6
375.6 164.7
180.2 73.3
77.9 82.4"

# Get the actual output from 'head testfile10'
actual_output=$(cat testfile10)

# Check if the actual output matches the expected output
if [[ "$actual_output" != "$expected_output" ]]; then
  echo "The head of the file does NOT match the expected output. Everything also from 10-2 is INVALID!"
fi

output=$(./cpair < testfile10)
expected_output="51.400 357.500
70.600 384.600"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

rm testfile10


###################### Command 11
echo -e "testcase 11:"

for i in $(seq 1 250); do
  ax=$(echo "A$i" | md5sum | cut -b 1-3)
  ay=$(echo "B$i" | md5sum | cut -b 1-3)
  bx=$(echo "scale=1;$((16#$ax)) / 10" | bc)
  by=$(echo "scale=1;$((16#$ay)) / 10" | bc)
  echo "$bx $by"
done > testfile250

# Define the expected output from 'head testfile250'
expected_output="407.0 173.9
326.9 372.7
92.8 55.0
351.7 48.6
322.9 154.5
293.7 287.3
352.8 28.5
342.7 379.3
253.9 28.2
155.4 349.6"

# Get the actual output from 'head testfile250'
actual_output=$(head testfile250)

# Check if the actual output matches the expected output
if [[ "$actual_output" != "$expected_output" ]]; then
  echo "The head of the file does NOT match the expected output. Everything also from 11 is INVALID!"
fi

# Execute the command 'sha1sum testfile250' and check the SHA1 hash
expected_sha1sum="316ae611e3cd3ea6f56996b7718d1db71ed250fd  testfile250"
actual_sha1sum=$(sha1sum testfile250)

# Check if the actual SHA1 hash matches the expected hash
if [[ "$actual_sha1sum" != "$expected_sha1sum" ]]; then
  echo "Checksum doesn't match! Consider the following statement as invalid:"
fi

output=$(./cpair < testfile250)
expected_output="25.900 16.600
26.300 16.000"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  EVERYTHING_CORRECT="false"
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

rm testfile250


###################### Command 12
echo -e "testcase 12:"
output=$(echo -e "-120 1\n-100 1\n-1 1\n1 1\n100 1\n121 1" | ./cpair)

expected_output="-1.000 1.000
1.000 1.000"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  EVERYTHING_CORRECT="false"
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 13
echo -e "Additional 1:"
output=$(echo -e "123.45 123.45\n123.45 234.56\n123.45 456.78" | ./cpair)

expected_output="123.450 123.450
123.450 234.560"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  EVERYTHING_CORRECT="false"
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 14
echo -e "Additional 2:"
output=$(echo -e "123.45 123.45\n123.45 124.45\n123.45 234.56\n123.45 345.67\n123.45 1123.45\n123.45 2234.56\n123.45 3345.67\n123.45 4123.45\n123.45 5234.56\n123.45 6345.67" | ./cpair)
expected_output="123.450 123.450
123.450 124.450"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
  EVERYTHING_CORRECT="false"
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi




####################Check if everything had been correct:
if [ "$EVERYTHING_CORRECT" == "false" ]; then
  echo -e "\n\n\nThere has been at least one error"
else
  echo -e "\n\n\nThere has been no errors. Good Job!"
fi





























echo -e "\nmake clean:"
make clean
