# Semaphores_C-
Semaphore assignment in C++

AUTHOR: 
	Nikolo Sperberg and Sterling Rohlinger

VERSION:
	1.052

CONTENTS: 
	BankStuff.cpp
	
DISCRIPTION: 
	This program prompts the user for a number of threads and transactions.
	It then creates those threads and runs the specified ammount of transactions.
	It utilizes semaphores to make sure that the widraws and deposits work right.
	
COMPILE AND RUN:
	C++ -
	To run this program you first must compile the .cpp file.  
	Use command prompt and type the following: g++ -std=c++11 -pthread BankStuff.cpp -o BankStuff
	This will compile the code.
	To run the code use the command prompt again. Type: ./BankStuff
