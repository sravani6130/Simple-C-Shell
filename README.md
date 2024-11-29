# How to run the code
just run the make file
```<home/Downloads> $ make```
# Specification 1 : Display Requirement
On every line when waiting for user input, a shell prompt of the following form must appear along with it. Do NOT hard code the username/system name here. The current directory address should be shown in the prompt as well.
```<Username@SystemName:~>```<br>
For example, if system name is “SYS”, username is “JohnDoe”, and the user is currently in the home directory, then prompt looks like this <br>
```<JohnDoe@SYS:~>```<br>
The directory from which shell is invoked becomes the home directory for the shell and represented with “~”. All paths inside this directory should be shown relative to it. Absolute path of a directory/file must be shown when outside the home directory.
When user changes the working directory, the corresponding change in path of directory must be reflected in the next prompt. For example, on going to the parent directory of the home directory of shell, following form of prompt is expected :<br>
```<JohnDoe@SYS:/home/johndoe/sem3>```<br>
# Specification 2 : Input Requirements
Your shell should support a ‘;’ or ‘&’ separated list of commands. You can use ‘strtok’ to tokenize the input.
Your shell should account for random spaces and tabs when taking input.
The “;” command can be used to give multiple commands at the same time. This works similar to how “;” works in Bash.
‘&’ operator runs the command preceding it in the background after printing the process id of the newly created process.<br>
```c
./a.out

<JohnDoe@SYS:~> vim &

[1] 35006

<JohnDoe@SYS:~> sleep 5 & e cho "Lorem ipsum"

[2] 35036

Lorem ipsum

# sleep runs in the background while echo runs in the foreground

<JohnDoe@SYS:~> hop test ; pwd

sleep with pid 35036 exited normally # after 5 seconds

~/test

<JohnDoe@SYS:~/test>

```
If any command is erroneous, then error should be printed.

```c
<JohnDoe@SYS:~> sleeeep 6

ERROR : 'sleeeep' is not a  valid command

```
# Specification 3 : hop
‘hop’ command changes the directory that the shell is currently in. It should also print the full path of working directory after changing. The directory path/name can be provided as argument to this command.

You are also expected to implement the “.”, “..”, “~”, and “-” flags in hop. ~ represents the home directory of shell (refer to specification 1).

You should support both absolute and relative paths, along with paths from home directory.

If more than one argument is present, execute hop sequentially with all of them being the argument one by one (from left to right).

If no argument is present, then hop into the home directory.
```c
<JohnDoe@SYS:~> hop test

/home/johndoe/test

<JohnDoe@SYS:~/test> hop assignment

/home/johndoe/test/assignment

<JohnDoe@SYS:~/test/assignment> hop ~

/home/johndoe

<JohnDoe@SYS:~> hop -

/home/johndoe/test/assignment

<JohnDoe@SYS:~/test/assignment> hop .. tutorial

/home/johndoe/test

/home/johndoe/test/tutorial

<JohnDoe@SYS:~/test/tutorial> hop ~/project

/home/johndoe/project

<JohnDoe@SYS:~/project>
```
# Specification 4 : reveal 
‘reveal’ command lists all the files and directories in the specified directories in lexicographic order (default reveal does not show hidden files). You should support the -a and -l flags.

-l : displays extra information

-a : displays all files, including hidden files

Similar to hop, you are expected to support “.”, “..”, “~”, and “-” symbols.

Support both relative and absolute paths.

If no argument is given, you should reveal at the current working directory.

Multiple arguments will not be given as input.

The input will always be in the format :
```c
reveal <flags> <path/name>
```
Handle the following cases also in case of flags :

reveal -a <path/name>

reveal -l <path/name>

reveal -a -l <path/name>

reveal -l -a <path/name>

reveal -la <path/name>

reveal -al <path/name>

Handle the cases where there are multiple l & a like:-

reveal -lala <path/name>

# Specification 5 : log commands
Implement a ‘log’ command which is similar to the actual history command in bash. The default number of commands it should store and output is 15 (max). You must overwrite the oldest commands if more than the set number of commands are entered. You should track the commands across all sessions and not just one. The commands should be printed from oldest to recent moving top to down (Refer example below).

Note :

DO NOT store a command in log if it is the exactly same as the previously entered command.

Store the arguments along with the command

Commands separated by ; or & are supposed to be stored in a single line.

Erroneous commands can also be stored (this is not a strict requirement, and you can choose to not store them). Please mention whatever you choose in README accordingly.

Do NOT store the log command in log. In cases where multiple commands separated by ; or & contain a log command, you are expected to not store the entire command string. For eg. in case of hop .. ; echo "OSN" ; log, the log will not be updated at all since log is present in the command string.

log purge

Clears all the log currently stored. Do not store this command in the log.

log execute <index>

Execute the command at position in log (ordered from most recent to oldest). **You are not required to store the command corresponding to this index in the log**, but you are free to do so if you want.

```c
<JohnDoe@SYS:~> reveal test

# output

<JohnDoe@SYS:~> sleep 5

# output

<JohnDoe@SYS:~> sleep 5

# output

<JohnDoe@SYS:~/test> echo "Lorem ipsum"

# output

<JohnDoe@SYS:~> log

reveal test

sleep 5

echo "Lorem ipsum"

<JohnDoe@SYS:~> log execute 1

# output of echo "Lorem ipsum"

<JohnDoe@SYS:~> log

reveal test

sleep 5

echo "Lorem ipsum"

<JohnDoe@SYS:~> log execute 3

# output of reveal test

<JohnDoe@SYS:~> log

reveal test

sleep 5

echo "Lorem ipsum"

<JohnDoe@SYS:~> log purge

<JohnDoe@SYS:~> log

<JohnDoe@SYS:~>     //outputs nothing since log has been purged

<JohnDoe@SYS:~> sleep 5 ; reveal

# output

<JohnDoe@SYS:~> log 

sleep 5 ; reveal

<JohnDoe@SYS:~> sleep 5 ; revealll

# error mesage

<JohnDoe@SYS:~> sleep 5 ; echo "Lorem Ipsum" ; log

<JohnDoe@SYS:~> log

sleep 5 ; reveal 

sleep 5 ; revealll   
```
# Specification 6 : System commands 
Your shell must be able to execute the other system commands present in Bash as well like emacs, gedit etc. This should be possible in both foreground and background processes.

## Foreground Process

Executing a command in foreground means the shell will wait for that process to complete and regain control afterwards. Control of terminal is handed over to this process for the time being while it is running.

Time taken by the foreground process and the name of the process should be printed in the next prompt if process takes > 2 seconds to run. Round the time down to integer before printing in prompt.

```c
<JohnDoe@SYS:~> sleep 5

# sleeps for 5 seconds

<JohnDoe@SYS:~ sleep : 5s>
```
## Background Process

Any command invoked with “&” is treated as a background command. This implies that your shell will spawn that process but doesn’t hand the control of terminal to it. Shell will keep taking other user commands. Whenever a new background process is started, print the PID of the newly created background process on your shell also.

```c
<JohnDoe@SYS:~> sleep 10 &

13027

<JohnDoe@SYS:~> sleep 20 & # After 10 seconds

Sleep exited normally (13027)

13054

<JohnDoe@SYS:~> echo "Lorem Ipsum" # After 20 seconds

Sleep exited normally (13054)

Lorem Ipsum
```
# Specification 7 : proclore 
proclore is used to obtain information regarding a process. If an argument is missing, print the information of your shell.

Information required to print :

pid

Process Status (R/R+/S/S+/Z)

Process group

Virtual Memory

Executable path of process
```c
<JohnDoe@SYS:~> proclore

pid : 210

process status : R+

Process Group : 210

Virtual memory : 167142

executable path : ~/a.out

  

<JohnDoe@SYS:~> proclore 301

pid : 301

process Status : R

Process Group : 243

Virtual memory : 177013

executable Path : /usr/bin/gcc
```
Process states :

R/R+ : Running

S/S+ : Sleeping in an interruptible wait

Z : Zombie

The “+” signifies whether it is a foreground or background process.
# Specification 8 : seek 
‘seek’ command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.

Note that by files, the text here refers to non-directory files.

Flags :

-d : Only look for directories (ignore files even if name matches)

-f : Only look for files (ignore directories even if name matches)

-e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it. Otherwise, the flag has no effect. This flag should work with -d and -f flags.

If -e flag is enabled but the directory does not have access permission (execute) or file does not have read permission, then output “Missing permissions for task!”

Argument 1 :

The target that the user is looking for. A name without whitespace characters will be given here. You have to look for a file/folder with the exact name, or one which contains this target as a prefix in its name.

Argument 2 (optional) :

The path to target directory where the search will be performed (this path can have symbols like . and ~ as explained in the reveal command). If this argument is missing, target directory is the current working directory. The target directory’s tree must be searched (and not just the directory).

```c
<JohnDoe@SYS:~> seek newfolder

./newfolder

./doe/newfolder

./doe/newfolder/newfolder.txt

<JohnDoe@SYS:~> seek newfolder ./doe

./newfolder # This is relative to ./doe

./newfolder/newfolder.txt

<JohnDoe@SYS:~> seek -d newfolder ./doe

./newfolder

<JohnDoe@SYS:~> seek -f newfolder ./doe

./newfolder/newfolder.txt

<JohnDoe@SYS:~> seek newfolder ./john

No match found!

<JohnDoe@SYS:~> seek -d -f newfolder ./doe

Invalid flags!

<JohnDoe@SYS:~> seek -e -f newfolder ./doe

./newfolder/newfolder.txt

This is a new folder! # Content of newfolder.txt

<JohnDoe@SYS:~> seek -e -d newfolder ./doe

./newfolder/

<JohnDoe@SYS:~/doe/newfolder>
```
Print “No match found!” in case no matching files/directories is found. Note that -d and -f flag can’t be used at the same time and must return error message “Invalid flags!”.

A call to this command will always be in the format :

### ```seek <flags> <search> <target_directory>``` ###

# Specification 9 : I/O Redirection
I/O Redirection is when you change the default input/output (which is the terminal) to another file. This file can be used to read input into a program or to capture the output of a program.

Your shell should support >, <,  » (< should work with both > and »).

> : Outputs to the filename following “>”.

>> : Similar to “>” but appends instead of overwriting if the file already exists.

< : Reads input from the filename following “<”.

Your shell should handle these cases appropriately:

An error message “No such input file found!” should be displayed if the input file does not exist.

The output file should be created (with permissions 0644) if it does not already exist in both > and ».

In case the output file already exists, it should be overwritten in case of > and appended to in case of  ».

You are NOT required to handle multiple inputs and outputs.

```c
<JohnDoe@SYS:~> echo "Hello world" > newfile.txt

<JohnDoe@SYS:~> cat newfile.txt

Hello world

<JohnDoe@SYS:~> wc < a.txt

1 2 12 # There can be extra spaces

<JohnDoe@SYS:~> echo "Lorem ipsum" > newfile.txt

<JohnDoe@SYS:~> cat newfile.txt

Lorem ipsum

<JohnDoe@SYS:~> echo "dolor sit amet" >> newfile.txt; cat newfile.txt

Lorem ipsum

dolor sit amet

<JohnDoe@SYS:~> wc < newfile.txt > a.txt

<JohnDoe@SYS:~> cat a.txt

2 5 27 # There can be extra spaces
```
# Specification 10 : Pipes 
Pipes are used to pass information between commands. It takes the output from command on left and passes it as standard input to the command on right. Your shell should support any number of pipes.

Note :

Return error “Invalid use of pipe”, if there is nothing to the left or to the right of a pipe (”|”).

Run all the commands sequentially from left to right if pipes are present.

```c
<JohnDoe@SYS:~> echo "Lorem Ipsum" | wc

1 2 12 # extra spaces can be present
```
# Specification 11 : Redirection along with pipes 
This specification requires you to be able to run I/O redirection along with pipes. It should support any number of pipes (but not multiple inputs and outputs from I/O redirection). In short, you are required to make sure that Specification 10 and Specification 11 work when given as input together.
```c
<JohnDoe@SYS:~> cat a.txt

Lorem Ipsum

<JohnDoe@SYS:~> cat < a.txt | wc | cat > b.txt

<JohnDoe@SYS:~> cat b.txt

1 2 12  # note that extra spaces can be present
```
# Specification 12 : activities
This specification requires you to print a list of all the processes currently running that were spawned by your shell in lexicographic order. This list should contain the following information about all processes :

Command Name

pid

state : running or stopped
```c
<JohnDoe@SYS:~> activities

221 : emacs new.txt - Running

430 : vim - Stopped

620 : gedit - Stopped
```
Format of an entry should be :

``` [pid] : [command name] - [State]```
# Specification 13 : Signals
``` ping <pid> <signal_number>``` <br>
ping command is used to send signals to processes. Take the pid of a process and send a signal to it which corresponds to the signal number (which is provided as an argument). Print error “No such process found”, if process with given pid does not exist. You should take signal number’s modulo with 32 before checking which signal it belongs to (assuming x86/ARM machine). Check man page for signal for an exhaustive list of all signals present.
```c
<JohnDoe@SYS:~> activities

221 : emacs new.txt - Running

430 : vim - Running

620 : gedit - Stopped

<JohnDoe@SYS:~> ping 221 9 					# 9 is for SIGKILL

Sent signal 9 to process with pid 221

<JohnDoe@SYS:~> activities

430 : vim - Running

620 : gedit - Stopped

<JohnDoe@SYS:~> ping 430 47

Sent signal 15 to process with pid 430 		# 15 is for SIGTERM

<JohnDoe@SYS:~> activities

430 : vim - Stopped

620 : gedit - Stopped
```
Following 3 commands are direct keyboard input where Ctrl is Control key on keyboard (or it’s equivalent).

Ctrl - C

Interrupt any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.

Ctrl - D

Log out of your shell (after killing all processes) while having no effect on the actual terminal.

Ctrl - Z

Push the (if any) running foreground process to the background and change it’s state from “Running” to “Stopped”. It has no effect on the shell if no foreground process is running.
# Specification 14 : fg and bg 
``` fg <pid>```
Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal. Print “No such process found”, if no process with given pid exists.
```c
<JohnDoe@SYS:~> activities

620 : gedit - Stopped

<JohnDoe@SYS:~> fg 620

# brings gedit [620] to foreground and change it's state to Running
```
``` bg <pid>```
Changes the state of a stopped background process to running (in the background). If a process with given pid does not exist, print “No such process found” to the terminal.
```c
<JohnDoe@SYS:~> activities

620 : gedit - Stopped

<JohnDoe@SYS:~> bg 620

# Changes [620] gedit's state to Running (in the background)
```
# Specification 15 : Neonate
Command : ```neonate -n [time_arg]```
The command prints the Process-ID of the most recently created process on the system (you are not allowed to use system programs), this pid will be printed every [time_arg] seconds until the key ‘x’ is pressed.
```c
<JohnDoe@SYS:~> neonate -n 4

# A line containing the pid should be printed

# every 4 seconds until the user

# presses the key: 'x'.

11810

11811

11811

11812

11813 # key 'x' is pressed at this moment terminating the printing
```

# Specification 16 : iMan 
iMan fetches man pages from the internet using sockets and outputs it to the terminal (stdout). In this case, you are required to use the website : http://man.he.net/ to get the man pages.

You do not need to handle “man page not found” error separately and you can print the html tags as well if there are any.

Do not print the header received during the GET request. (Check example given below for exact output).

```iMan <command_name>```

```<command_name>``` is the name of the man page that you want to fetch.

This should fetch the man page for the given command from http://man.he.net/, if the page does not exist, you can continue to print the response from the page.

```c
<JohnDoe@SYS:~> iMan sleep

# expected output here :
sleep




sleep

SLEEP(1)                         User Commands                        SLEEP(1)

NAME
       sleep - delay for a specified amount of time

SYNOPSIS
       sleep NUMBER[SUFFIX]...
       sleep OPTION

DESCRIPTION
       Pause for NUMBER seconds.  SUFFIX may be 's' for seconds (the default),
       'm' for minutes, 'h' for hours or 'd' for days.  Unlike most  implemen-
       tations  that require NUMBER be an integer, here NUMBER may be an arbi-
       trary floating point number.  Given two or more  arguments,  pause  for
       the amount of time specified by the sum of their values.

       --help display this help and exit

       --version
              output version information and exit

AUTHOR
       Written by Jim Meyering and Paul Eggert.

REPORTING BUGS
       GNU coreutils online help: &lt;https://www.gnu.org/software/coreutils/&gt;
       Report sleep translation bugs to &lt;https://translationproject.org/team/&gt;

COPYRIGHT
       Copyright  (C) 2018 Free Software Foundation, Inc.  License GPLv3+: GNU
       GPL version 3 or later &lt;https://gnu.org/licenses/gpl.html&gt;.
       This is free software: you are free  to  change  and  redistribute  it.
       There is NO WARRANTY, to the extent permitted by law.

SEE ALSO
       sleep(3)

       Full documentation at: &lt;https://www.gnu.org/software/coreutils/sleep&gt;
       or available locally via: info '(coreutils) sleep invocation'

GNU coreutils 8.30              September 2019                        SLEEP(1)

Man Pages Copyright Respective Owners.  Site Copyright (C) 1994 - 2024
Hurricane Electric.
All Rights Reserved.


o or more  arguments,  pause  for
       the amount of time specified by the sum of their values.

       --help display this help and exit

       --version
              output version information and exit

AUTHOR
       Written by Jim Meyering and Paul Eggert.

REPORTING BUGS
       GNU coreutils online help: &lt;ht

// expected output ends

<JohnDoe@SYS:~> iMan invalid_command


# expected output here : 


invalid_command




invalid_command
No matches for "invalid_command"
Search Again

Man Pages Copyright Respective Owners.  Site Copyright (C) 1994 - 2024
Hurricane Electric.
All Rights Reserved.


ERROR: No matches for "invalid_command" command

// expected output ends

```
If there are more than 1 argument, iMan considers only the first argument and ignores the rest. For example :

```"iMan sleep" is the same as "iMan sleep extra"```