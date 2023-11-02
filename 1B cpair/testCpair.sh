#!/bin/bash

echo -e "make:"
make

###################### Command 1
echo -e "\ntestcase 01:"
output=$(echo -e "4 4\n-1 1\n1 -1\n-4 -4" | ./cpair)
expected_output="-1.000 1.000
1.000 -1.000"
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
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
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 2
echo -e "testcase 03:"
output=$(echo -e "91.0 2\n-1.0 7.1" | ./cpair)
expected_output="-1.000 7.100
91.000 2.000"
if [ "$output" == "$expected_output" ]; then
  echo "message is correct."
else
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
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 8
echo -e "testcase 8:"
echo -e "foo" | ./cpair
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


###################### Command 9
echo -e "testcase 9:"
echo -e "1.0 2\n4 5 6.0" | ./cpair
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


###################### Command 10
echo -e "testcase 10:"
output=$(cat $?)
expected_output="1"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "Output is correct."
else
  echo "Output is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi


###################### Command 11
echo -e "testcase 11: (currently disabled)"
#( for i in `seq 1 10`; do ax=`echo "a$i" | md5sum | cut -b 1-3`; ay=`echo "b$i" | md5sum | cut -b \ 1-3`; bx=`echo "scale=1;$((16#$ax)) / 10" | bc`; by=`echo "scale=1;$((16#$ay)) / 10" | bc`; echo \ $bx $by; done; ) > testfile10
#output=$(./cpair < testfile10)
#expected_output="51.400 357.500
#70.600 384.600"
#
## Check if the output is correct
#if [ "$output" == "$expected_output" ]; then
#  echo "Output is correct."
#else
#  echo "Output is wrong."
#  echo "Expected: $expected_output"
#  echo "Got: $output"
#fi

rm testfile10


###################### Command 12
echo -e "testcase 12: (currently disabled)"
#( for i in `seq 1 250`; do ax=`echo "A$i" | md5sum | cut -b 1-3`; ay=`echo "B$i" | md5sum | cut -b \ 1-3`; bx=`echo "scale=1;$((16#$ax)) / 10" | bc`; by=`echo "scale=1;$((16#$ay)) / 10" | bc`; echo \ $bx $by; done; ) > testfile250
#output=$(./cpair < testfile250)
#expected_output="25.900 16.600
#26.300 16.000"
#
#checksum=$(sha1sum testfile250)
#if [ "$checksum" != "316ae611e3cd3ea6f56996b7718d1db71ed250fd testfile250" ]; then
#    echo "Checksum is incorrect. The following statement is not valid:"
#fi
#
#
## Check if the output is correct
#if [ "$output" == "$expected_output" ]; then
#  echo "Output is correct."
#else
#  echo "Output is wrong."
#  echo "Expected: $expected_output"
#  echo "Got: $output"
#fi
#rm testfile250

###################### Command 13
echo -e "testcase 13:"
output=$(echo -e "-120 1\n-100 1\n-1 1\n1 1\n100 1\n121 1" | ./cpair)

expected_output="-1.000 1.000
1.000 1.000"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "message is (probably) correct."
else
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 14
echo -e "Additional 1:"
output=$(echo -e "123.45 123.45\n123.45 234.56\n123.45 456.78" | ./cpair)

expected_output="123.450 123.450
123.450 234.560"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "message is (probably) correct."
else
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi

###################### Command 13
echo -e "Additional 2:"
output=$(echo -e "123.45 123.45\n123.45 124.45\n123.45 234.56\n123.45 345.67\n123.45 1123.45\n123.45 \ 2234.56\n123.45 3345.67\n123.45 4123.45\n123.45 5234.56\n123.45 6345.67" | ./cpair)
expected_output="123.450 123.450
123.450 124.450"

# Check if the output is correct
if [ "$output" == "$expected_output" ]; then
  echo "message is (probably) correct."
else
  echo "message is wrong."
  echo "Expected: $expected_output"
  echo "Got: $output"
fi
































echo -e "\nmake clean:"
make clean
