#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>
#include <string>


using namespace std;

bool execute( vector<char *> command)
{
	bool good_exc = true;
	char *argv[command.size()+ 1];
	for(int i = 0; i < command.size(); ++i)	
	{
		argv[i] = command.at(i);	
	
	}								
	argv[command.size()] = '\0';
	int  pid = fork();
	int status;
	if(pid < 0)
	{
		perror("ERROR: forking child process failed\n");
		exit(1);
	}
	else if (pid == 0)
	{
		if (execvp(*argv, argv) < 0)
		{
			perror("ERROR: CMD execution failed.\n");
			good_exc = false;
		}
	}
	else
	{
		if(-1 == wait(0)) perror ("There was an error with wait().");
	}
	return good_exc;	
}

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
		{
			no_exit = false;
			break;
		}
		
		char *token = strtok(const_cast<char*>(input.c_str()), " ");
		vector<char *> input_list;
		while(token != NULL)
		{
			input_list.push_back(token);
			token = strtok (NULL, " ");
		}
		
		bool proper_exc = true;
		vector<char *>cmd_run; 
		for(int i = 0; i < input_list.size();++i)
		{	
			
			string temp = input_list.at(i);
			if(temp == ";")
			{
				proper_exc = execute(cmd_run);
				cmd_run.clear();
			}
			else
			{
				
				cmd_run.push_back(input_list.at(i));
	
			}
			if(temp.find(';') != string::npos && no_exit && temp != ";" )
			{
				if(temp.find(';') != 0)
				{
					input_list.at(i) = const_cast<char *>(temp.substr(0, temp.find(';')).c_str());
					cmd_run.pop_back();
					cmd_run.push_back(input_list.at(i));
					proper_exc = execute(cmd_run);
					cmd_run.clear();
			
				}
				else
				{
					cmd_run.pop_back();
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					input_list.at(i) = const_cast<char *>(temp.substr(temp.find(';')+1).c_str());
					cmd_run.push_back(input_list.at(i));
		
				}
			}
		}
		if(no_exit)proper_exc = execute(cmd_run);
		cout << endl;
	}
	return 0;
}
