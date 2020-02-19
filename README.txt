ARP ASSIGNMENT SPECIFICATIONS

The program uses a Launcher to launch all the required processes and requried pipes. It passes arguments to each of the processes, such as the file descriptors of pipes and the arguments needed, such as RT or the IP number. The processes created by the launcher are S, L, P and G. 

The process S receives signals as shell commands. G sends data to P and P reads data from S and G. S sends a 1 if the program needs to read 

To get your IP, hostname -I


#### STRUCTURE OF THE CONFIGURATION FILE ####
my_machine: 192.168.1.233 
previous_machine: something1
next_machine: something2
my_portnumber: 5555
next_portnumber: something3
RF: 3


