#!/bin/bash

make

# Function to handle Ctrl+C
function ctrl_c() {
    echo "Ctrl+C pressed. Terminating the server..."
    if [ -n "$server_pid" ]; then
        kill -9 $server_pid
    fi
    exit 1
}

# Check if the server binary exists
if [ ! -x "./server" ]; then
    echo "Error: The 'server' binary does not exist in the current directory."
    exit 1
fi

# Start the server in the background
./server -p 8090 "./my_website" &
server_pid=$

# Set up the Ctrl+C handler
trap ctrl_c INT

# Wait for the server to finish
wait $server_pid

