# Sockets

First make sure to compile project by running `make` at the root directory.

The binary files are in the **/bin** directory.

## Server

Run the server binary from the root directory

```bash
./bin/server [hostname] [port] [server-files-directory]
```

Example:

```bash
./bin/server localhost 8080 views
```

This will serve the server on <localhost:8080>

> You can test out multi-threading by accessing `/sleep` route and quickly on another tab accessing `/` route. The `/sleep` makes the serving thread sleep for **5** seconds.

## Client

TO BE IMPLEMENTED
