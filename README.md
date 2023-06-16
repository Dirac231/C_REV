### What is this?
A rudimentary reverse shell client/server project in C, mainly created while learning about sockets and network interfacing. \
 \
Could be useful if you want to train various obfuscation and evasion techniques or add complex functionalities to the project.

### How to use
To open the server, on a linux machine first compile and run ```server.c``` after modifying the connection parameters, like this:
```
gcc server.c -o server && ./server
```
Then you can cross-compile the file ```backdoor.c``` to produce an executable that will open a reverse shell to your server:
```
i686-w64-mingw32-gcc -o shell.exe backdoor.c
```
