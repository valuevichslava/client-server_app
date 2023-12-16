# Start from a base image
FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    libc-dev

# Set the working directory
WORKDIR /app

# Copy the server code into the container
COPY . .

# Compile the server code
RUN gcc -o nserver1 nserver1.c

# Set the command to run the server
CMD ["./nserver1"]
