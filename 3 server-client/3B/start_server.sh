#!/bin/bash

# Function to handle Ctrl+C
function ctrl_c() {
    echo "Ctrl+C pressed. Terminating the server..."
    kill -9 $server_pid
    exit 1
}

# Start the server in the background
./server /my_website &
server_pid=$

# Set up the Ctrl+C handler
trap ctrl_c INT

# Wait for the server to finish
wait $server_pid
