# Sockets

First make sure to compile the project by running `make` at the root directory.

The binary files are in the **/bin** directory.

Create a **recv** directory to save the files that the client downloaded.

## Server

Run the server binary from the root directory

```bash
./bin/server [hostname] [port] [server-files-directory]
```

Example:

```bash
./bin/server localhost 8080 views
```

This will serve the server on <localhost:8080> and have all assets served from the `views` directory.

> You can test out multi-threading by accessing `/sleep` route and quickly on another tab accessing `/` route. The `/sleep` makes the serving thread sleep for **5** seconds.

## Client

Run the client binary from the root directory

```bash
./bin/client [URL 1] [URL 2]...
```

Example:

```bash
./bin/client localhost:8080
```

This will make a **GET** request to all the URLs specified and will save the objects received from them inside the `recv` directory. If making a request to the HOST=`localhost` make sure to first run the server binary and also to include the port as the default port set is `80`.
