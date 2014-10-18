#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <vector>
using namespace std;



int main()
{
	string input;
	bool no_exit = true;
	while(no_exit)
	{
		char *name = getlogin();
		char host[50];
		gethostname(host,50);
		cout << name <<"@"<< host <<"$";
		getline(cin, input);
		if(input == "exit")
		no_exit = false;
		
		char *token = strtok(const_cast<char*>(input.c_str()), " ");
		vector<char *> input_list;
		while(token != NULL)
		{
			input_list.push_back(token);
			token = strtok (NULL, " ");
		}
		cout << "##### PRINTING INPUT ARG#####" << endl;
		//remember to remove for loop 
		for(int i = 0; i < input_list.size();++i)
		{
			cout << input_list.at(i) << endl;
		}
	}
	return 0;
}
