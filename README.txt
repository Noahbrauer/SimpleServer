README FOR CS 270 SPRING '18 PROJECT 5

INCLUDED FILES:
csapp.h, csapp.o, csapp.c, smalld.cpp, smallGet.cpp, smallSet.cpp, 
smallRun.cpp, smallDigest.cpp, MAKEFILE, README.txt, testcases.txt
The testcases file is how we chose to run and test our program. 
It includes a few variations which work.

COMPILATION/INVOCATION:
On a Unix command line, type "make" to create the executable smalld 
as well as executables for the clients included. 
You also have the choice to make the executables seperately. 
Run the executable with a port and secretKey to establish a connection to the server, i.e.) 
./smalld 6564 1411223 &
The '&' must be included to start the server, if it isnt youll 
recieve a small UNIX error and not be able to enter commands
To compile the clients, it is the same as smalld, except the executable 
name which will be whatever client you are compiling.
Likewise with how to invoke smalld, the only change is the arguments 
to pass to the function. The arguments are outlined below:

./smalld port SecretKey &

./smallGet MachineName port SecretKey variableName value

./smallSet MachineName port SecretKey variableName

./smallDigest MachineName port SecretKey value

./smallRun MachineName port SecretKey value

OVERVIEW:
Smalld is a server that listens for connections with a specific port 
and a secret key. The server can only handle one request per 
connection. If a valid request is received, the server will print out 
the following:

Secret Key = xxxxxxx (whatever the user entered as a secret key)
Request type = set, get, digest, or run
Detail = detail (specific to request type)
Completion = status (success or failure)

FUNCTIONS:

smallGet: the server reports the contents of the variable or failure if 
a variable is not set. The client prints the contents it
receives from the server, or failed.

smallSet: the server sets the vaule passed to the variable passed internally. 
The client prints nothing unless it failed.

smallDigest: returns the result of /bin/echo value | /usr/bin/sha256sum. 
This guarantees the result to be 100 bytes or less. The
client prints the result it gets from the server, or failed.

smallRun: the server runs the program and sends the first 100 bytes of output 
back to the client. The only valid programs are the 
following:

inet (the command: /sbin/ifconfig -a)
hosts (the command: /bin/cat /etc/hosts)
services (the command: /bin/cat /etc/services)

Any other program passed in is rejected, and the client prints out failed.
