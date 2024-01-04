#!/bin/bash

# Compile the server and client code if they are not already compiled
make

# Open new terminal for server
gnome-terminal -- bash -c "./server; exec bash"

# Open new terminal for client
gnome-terminal -- bash -c "./client; exec bash"
