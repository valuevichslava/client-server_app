# Start from a base image
FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    libc-dev


ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# Set the working directory
WORKDIR /app

# Copy the client code into the container
COPY . .

# Compile the client code
RUN gcc -o nclient nclient.c

# Set the command to run the client
CMD ["./nclient"]
