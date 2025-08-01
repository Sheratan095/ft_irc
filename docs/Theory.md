
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
POLLOU: watch for writing (socket is ready to write)
POLLERR: watch for errors
POLLHUP: whtch for hang-up (peer closed connection)
For a server socket, that means: “There is an incoming connection waiting to accept()


The poll() function provides applications with a mechanism for multiplexing input/output over a set of file descriptors. For each member of the array pointed to by fds, poll() shall examine the given file descriptor for the event(s) specified in events. The number of pollfd structures in the fds array is specified by nfds. The poll() function shall identify those file descriptors on which an application can read or write data, or on which certain events have occurred.
Upon successful completion, poll() shall return a non-negative value. A positive value indicates the total number of file descriptors that have been selected (that is, file descriptors for which the revents member is non-zero). A value of 0 indicates that the call timed out and no file descriptors have been selected. Upon failure, poll() shall return -1 and set errno to indicate the error.



----------------------------------------------------------------------------------------------------------------------------------

DEPRECATED
Configure socket timeout to prevent accept() from blocking indefinitely
This allows the server to periodically check SERVER_RUNNING flag
struct timeval	timeout;
timeout.tv_sec = 1;		// 1 second timeout
timeout.tv_usec = 0;	// 0 microseconds could be removed
setsockopt(_socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

