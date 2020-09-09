#include <iostream>
#include <string>
#include <cerrno> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;


std::mutex m;
int balance;
auto start = chrono::steady_clock::now(); 


/**
*
*this method returns a random number....
*it was given to us by Dr. Tan
*I kept getting zero so I wrote get Ran as a checker to make sure the random number we got was within bounds that we need
*/
int randNum(long nonce){
	const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

	static long state = 1;
	long t = A * (state % Q) - R * (state / Q);
	
	if (t > 0)
		state = t;
	else
		state = t + M;
	return (long)(((double) state/M)* nonce);
}

/**
*
*this number get a numbe between 1 and 0
*it uses time stamps and randNum to get this number
*
*/
double getTrans(int y){
	bool flag = true;//set flag to true
	auto end = chrono::steady_clock::now();
	int time = chrono::duration_cast<chrono::microseconds>(end-start).count();//get time stamp
	double x = randNum(y * time);//get random number from Dr. Tans code
	while(x > 1){//incase x is too big
		int temp = randNum(y);//get a random number
		temp = temp % 2;//mod random number
		if(temp == 0){
			x = x / 3;//divide x by 3
		}
		else{
			x = x / 4;//divide x by 4
		}
		
	}
	return x;//return x
}

/**
*
*this method get a random number and uses a ranNum to make a random number 
*this method make sure that the random number generate from randNum is within our rand of 300 to 1
*it also uses time stamps as well as getTrans to help widen the rand of numbers
*/
int getRand(long num){
	bool flag = true;//boolean used to make sure number in specifide range
	auto end = chrono::steady_clock::now();//used to get time stamp
	int time = chrono::duration_cast<chrono::microseconds>(end-start).count();//grab time stamp to help ensure randomness
	double x = randNum(num * time);	//get a random number
	double temp = 0;//initiate temp
	while(flag){
		temp = getTrans(num * time * x);//get another random number
		x = getTrans(x);//get a number between 1 and 0 based off perv gnereated random number
		if(temp > 0.5){
			x = x * 100;//times number between 1 and 0 by 50
		}
		else{
			x = x * 100;//times number between 1 and 0 by 100
		}
		if(x > 300){
			x = x - 50;//minus 50 if too big
		}
		else if(x <= 0){
			x = x + 50;//add 50 if too small
		}
		else{
			flag = false;//if number checks out exit while loop
		}
	}
	num = num + x;//add x to num
	temp = getTrans(num);//get ranom num between 1 and 0
	if(temp > 0.5){
		num = num + 150;//add 150 to num
	}
	else{
		num = num - 100;//subtract 100 from num
	}
	flag = true;//reste flag
	while(flag){
		if(num > 300){//if num too big
			num = num - 100;//minus 100
		}
		else if(num <= 0){//if num too small
			num = num + 100;//add 100
			num = num + randNum(num);//generate new random number
		}
		else{//number checks out
			flag = false;//exit while loop
		}
	}
	return num;//set num to x
}

/**
*this method prompts the user for input and returns an int
*
*/
int promptUser(std::string &variable){
	int num = 0;//num to be returned
	std::string input = "";//variable to hold user input
	cout<<"Enter "<<variable<<": ";//print prompt to console
	scanf("%i", &num);//grab user input
	return num;//return user input
}

/**
*
*this method preforms the withdraw and then prints what had happened
*/
void withdraw(int ammount, int id){
	int prevBalance = balance;//temp save prev balance
	auto end = chrono::steady_clock::now();//get time stampe
	if(balance - ammount < 0){//this checks if the withdraw would cause a negative number
		cout<<"tID: "+std::to_string(id)+" - balance = "+std::to_string(prevBalance)+" | withdraw $"+std::to_string(ammount)+" (NSF) | balance = "+std::to_string(balance)+ " | "+std::to_string(chrono::duration_cast<chrono::microseconds>(end-start).count())<<endl;
	}
	else{//if there are enough monies in the bank bro
		balance = balance - ammount;//do math
		cout<<"tID: "+std::to_string(id)+" - balance = "+std::to_string(prevBalance)+" | withdraw $"+std::to_string(ammount)+" | balance: "+std::to_string(balance)+" | "+std::to_string(chrono::duration_cast<chrono::microseconds>(end-start).count())<<endl;
	}
}

/**
*
*this method preforms the deposit and then prints what had happened
*/
void deposit(int ammount, int id){
	int prevBalance = balance;//temp save previous balance
	auto end = chrono::steady_clock::now();//get time stamp
	balance = balance + ammount;//do math
	cout<<"tID: "+std::to_string(id)+" - balance = "+std::to_string(prevBalance)+" | deposit $"+std::to_string(ammount)+" | balance: "+std::to_string(balance)+" | "+ std::to_string(chrono::duration_cast<chrono::microseconds>(end-start).count())<<endl;
}

/**
*
*this method runs the threads, neat!
*
*/
void run(int numTrans, int id){
	double transaction = 0;//initiate the transaction number
	while(numTrans > 0){//while loop so that all the threads are exicuted
		int ammount = getRand(numTrans * numTrans);//crate ammount
		transaction = getTrans(ammount);//get a 1 or 0 to determine the transaction
		if(transaction > 0.5){
			m.lock();//lock critical section
			withdraw(ammount, id);//withdraw method call
			m.unlock();//unlock

		}
		else{
			m.lock();//lock critical section
			deposit(ammount, id);//deposit method call
			m.unlock();//unlock

		}
		numTrans--;//decriment numTrans
	}
}
/**
*
*this method creates the threads
*
*/
void makeThreads(int numThreads, int numTrans){
	std::thread threads[numThreads];//create thread array
	for(int i = 0; i < numThreads; i++){//loop to exicute threads
		threads[i] = std::thread(run, numTrans, i);//run the threads bro!
	}
	for(int i = 0; i < numThreads; i++){//loop to exicute threads
		threads[i].join();//run the threads bro!
	}
}


/**
*
*driver method
*
*/
int main(){	
	string variable = "NumThreads";//variable to be passed to promptUser, this way we can use the same code twice
	int numThreads = promptUser(variable);//get numThreads from user
	variable = "NumTrans";//change variable
	int numTrans = promptUser(variable);//get numTrans from user
	balance = getRand(numThreads * numTrans);//gets an random number for start balance
	makeThreads(numThreads, numTrans);//makes the threads
}

