# Excercises

- [1A MyCompress](#1a-mycompress)
- [1B Closest Pair](#1b-closest-pair)
- [3A http-client](#3a-http-client)
- [contributing](#contributing-1)
- [license](#license)

---

# 1A MyCompress

## Overview

MyCompress is a simple compression utility written in C.
It compresses input text by substituting subsequent identical characters
with a single occurrence of the character followed by the number of occurrences.
This program can process input from either standard input (stdin) or specified input files.

## Features

- Compresses text by reducing repeated characters.
- Accepts input from stdin or input files.
- Outputs compressed text to stdout or a specified output file.

## Installation

Clone the repository and compile the program using the provided Makefile:

```bash
make
```

## Usage

To use MyCompress, you can specify an input file and an optional output file. If no input file is specified, it reads
from stdin.

```bash
./mycompress [-o outputFile] [inputFiles]
```

or:

```bash
./mycompress [-o outputFile] < input.txt
```

- `-o outputFile`: Specifies the output file to write the compressed data. If not provided, output will be written to
  stdout.
- `inputFile`: Specifies the input file to read data from. If not provided, input will be read from stdin.

## Example

Compressing a file:

```bash
./mycompress -o output.txt input.txt
```

Reading from stdin and writing to stdout:

```bash
echo "aaabbbbcc" | ./mycompress
```

## Dependencies

- GCC for compiling the C program.
- Standard C libraries.

## Contributing

Contributions to improve MyCompress are welcome. Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch for your feature or fix.
3. Commit your changes with clear commit messages.
4. Push your changes to the branch.
5. Create a pull request.


---

# 1B Closest Pair

## Overview

This project implements an algorithm to find the closest pair of points
in a given set of points. The main logic of the project is contained in **cpair.c**,
which uses various helper functions and structures defined in **cpair.h**, **includes.h**,
and **helperFunctions.c**.

## Features

Efficient calculation of the closest pair of points.
Utilization of various data structures and algorithms for optimal performance.
Modular codebase with clear separation of concerns.

## How to Build

The project can be built using the provided Makefile. Simply run the following command in the project directory:

```bash
make
```

## Usage

After building the project, you can run the program with the following command:

```bash
./cpair < input.txt
```

or you can:

```bash
echo -e "4 90\n4 89\n4 200\n4 -100" | ./cpair
```

Input the points as instructed by the program, and it will output the closest pair of points.

## Files Description

**cpair.c:** Main file containing the program's entry point and core logic.
**cpair.h:** Header file for cpair.c, containing function prototypes and structure definitions.
**includes.h:** Common includes and definitions used across the project.
**helperFunctions.c:** Auxiliary functions supporting the main algorithm in cpair.c.

## Algorithm

The program uses a divide-and-conquer approach to find the closest pair of points, which is more efficient than the
brute force method for large datasets.

---

# 3A http-client

## Overview

The "3A http-client" is a simple yet powerful HTTP client written in C. It is capable of sending HTTP GET requests to a specified URL and handling the responses appropriately. The project is structured with a focus on modularity and reusability, separating the core functionalities into different helper functions.

## Features

- Send HTTP GET requests to specified URLs.
- Handle HTTP responses and output the received data.
- Parse command-line arguments to specify request details like port, file, and directory.
- Efficiently handle URLs, extracting hostname and resource paths.
- Robust error handling and dynamic memory management.

## Installation

To install the http-client, clone the repository and compile the program using the provided Makefile:

```bash
make client
```

## Usage
To use the http-client, run the compiled executable with the necessary arguments:
```bash
./client [-p PORT] [-o FILE | -d DIR] URL
```
### Options:

- **-p** PORT: Specify the port number for the HTTP request.
- **-o** FILE: Direct the output to a specified file.
- **-d** DIR: Specify a directory for the output.
- **URL**: The URL for the HTTP GET request.

### Example Usage:
```bash
./client http://www.example.com/ > tests/test_A01.html
./client -o tests/test_A02.html http://www.example.com/
./client -d tests/ http://www.example.com/
./client -d tests http://www.example.com/
./client -f tests http://www.example.com/
./client -f tests.html http://www.example.com/
```

## Dependencies
- C standard libraries.
- POSIX compliant system for network and file operations.

## Contributing

Contributions to enhance the http-client are encouraged. Please follow the standard procedure for contributions as outlined in the contributing section.

---

# Contributing

Contributions to the project are welcome. Please follow the standard GitHub pull request process to submit your changes.

# License

Just don't copy and hand in this assignment. If you do something else with it, I don't care.

# Author

Chrisvenator