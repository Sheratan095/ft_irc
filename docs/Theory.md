
struct pollfd
{
    int   fd;       => file descriptor you want to watch
    short events;   => which events you care about
    short revents;  => what actually happened (poll() fills this in)
};

Which descriptor (fd) to watch.
What you want to watch for (events).
Where to store the result (revents).

For a listening socket, you are interested in:
POLLIN: means there is something to read.
For a server socket, that means: “There is an incoming connection waiting to accept()