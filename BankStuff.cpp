/**
* this method checks if the user input is not 0-9
*
*/
bool checkInput(const std::string &str){
    return std::all_of(str.begin(), str.end(), ::isdigit); //I found this code on stack overflow and do not take credit for writing it
}
/**
*this method prompts the user for input and returns an int
*
*/
int promptUser(String variable){
	int num = 0;//num to be returned
	String input = ""//variable to hold user input
	cout<<"Enter "<<variable<<": ";//print prompt to console
	scanf("%s", &input);//grab user input
	if(checkInput(input){//check if vaild
		num = atoi(input);//cast to int
	}
	else{//not vaild input
		cout<<"Please input a number."<<endl;//remond user to enter a number
		num = promptUser(variable);//recusion yo
	}
	return num;//return user input
}

/**
*
*driver method
*
*/
int main(){
	String variable = "NumThreads";//variable to be passed to promptUser, this way we can use the same code twice
	int numThreads = promptUser(variable);//get numThreads from user
	cout<<"You entered: "<<numThreads<<endl;//print entered value
	variable = "NumTrans";//change variable
	int numTrans = promptUser(variable);//get numTrans from user
	cout<<"You entered: "<<numTrans<<endl;//print entered value
}