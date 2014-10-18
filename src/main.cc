#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

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
		cin >> input;
		if(input == "exit")
		no_exit = false;
	}
	return 0;
}
