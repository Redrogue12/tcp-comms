# tcp-comms
Simple TCP server/client in C

## server.c

- Create a TCP socket.
- Bind it to a port (8080).
- Start listening for client connections.
- Accept a connection when a client connects.
- Receive a message from the client.
- Send a response back.
- Close the connection.

## client.c

- Create a TCP socket.
- Define the server address (IP and port 8080).
- Connect to the server.
- Send a message to the server.
- Receive a response from the server.
- Print the server’s response.
- Close the connection.

## Formatting

clang-format