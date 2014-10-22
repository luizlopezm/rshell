rshell
======

Licensing Information: READ LICENSE
---

Project Source can be download from http://github.com/luizlopezm/rshell.git
----

Author & Contributor List
---------
Luiz Lopez Nunez

Mike Izbibki

All other know bugs and fixes can be sent to llope039@ucr.edu

File List
---------
```
.:
Makefile

LICENSE

README.md

./src

./test
```

```
/src:

main.cc
```

```
./test:

exec.script
```

How To Run Program
----------

This program was designed and implemented on linux enviroment as such 
there is no garuantee of running properly on an OS X machine and less 
likely on a windows based PC.

IN ORDER TO RUN PROGRAM RUN FOLLOWING COMMANDS IN TERMINAL:

```
$git clone http://github.com/luizlopezm/rshell.git
$cd rshell
$git checkout hw0
$make
$/bin/rshell
```

Program Overview
----------
This program replicates the command execution of the BASH terminal.It 
also to to run multiple commands connected with specific connectors and 
process the specific logic associated it that specific connector.As well
as having the ability to show comments by using `#` followed by text. Program
can be exited simply by typing `exit` and program will exit back to orginal
terminal.

EXAMPLE COMMAND EXECTIONS:

```
$ls -a
$echo hello
$pwd
$ls -a ; echo hello ; pwd
$ls -a && echo hello
$ls -a || echo hello
$ls -a #This is just a comment.
```

Bugs
----------
1. Shell won't always exit properly.

2. Using different connectors in same command line won't always execute properly.
