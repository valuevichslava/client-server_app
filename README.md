# Multitasking App

This is a client-server app that communicates with two servers to perform various tasks. The app uses sockets to connect to the servers, sends commands, and reads their responses. The app is written in C and uses Podman which emulates Docker for creating isolated environments.

## Installation and Setup <sup><sub>(tested on Linux)</sub></sup>

1. Install Make, Docker if they are not already installed on your system.

2. Clone the project repository:

```bash
git clone https://github.com/valuevichslava/client-server_app.git
cd client_server_app
```
3. Start the project:

```bash
make build-all
```

```bash
make run-server1
```

```bash
make run-server2
```

```bash
make run-client
```

## Technologies Used

- C - The programming language used for the project.
- Docker - A platform used in the project for creating, deploying, and managing containers, allowing the application to run in an isolated environment.
