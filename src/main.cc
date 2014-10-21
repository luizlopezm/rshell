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
		cout << "Commands Parsed: " << argv[i] << endl;		
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
		if (execvp(*argv, argv) == -1)
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
	if(input != "exit")
	{		
		char *token = strtok(const_cast<char*>(input.c_str()), " ");
		vector<char *> input_list;
		while(token != NULL)
		{
			input_list.push_back(token);
			token = strtok (NULL, " ");
		}
		
		bool proper_exc = true;
		bool found_ands = false;
		bool found_par = false;
		vector<char *>cmd_run; 
		for(int i = 0; i < input_list.size();++i)
		{	
			
			string temp = input_list.at(i);
			if(temp == ";")
			{
				proper_exc = execute(cmd_run);
				cmd_run.clear();
			}
			if(temp == "#")
			{
				proper_exc = execute(cmd_run);
				cmd_run.clear();
				break;
			}
			else if(temp == "&&")
			{
				if(proper_exc == true)
				{
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					found_ands = true;
				}
				else
				{
					cout << "First Command Failed, cannot excute current command" << endl;
					break;
				}
			}
       			else if(temp == "||")
			{
				if(!found_par)
				{
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					found_par = true;
				}
				else if(found_par && !proper_exc)
				{
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					found_par = true;
				}

				else
				{
					cout << "First Command Succeeded, cannot excute current command" << endl;
				}
			}
			else
			{
				
				cmd_run.push_back(input_list.at(i));
	
			}
			if(temp.find('#') != string::npos && no_exit && temp != "#" )
			{
				if(temp.find('#') != 0)
				{
					input_list.at(i) = const_cast<char *>(temp.substr(0, temp.find('#')).c_str());
					cmd_run.pop_back();
					cmd_run.push_back(input_list.at(i));
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					break;
			
				}
				else
				{
					cmd_run.pop_back();
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					break;
		
				}
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
					found_ands = false;
			
				}
				else
				{
					cmd_run.pop_back();
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					found_ands = false;
					input_list.at(i) = const_cast<char *>(temp.substr(temp.find(';')+1).c_str());
					cmd_run.push_back(input_list.at(i));
		
				}
			}
			if(temp.find("&&") != string::npos && no_exit && temp != "&&" )
			{
				if(temp.find("&&") != 0)
				{
					input_list.at(i) = const_cast<char *>(temp.substr(0, temp.find("&&")).c_str());
					cmd_run.pop_back();
					found_ands = true;
					cmd_run.push_back(input_list.at(i));
					
					if(proper_exc == true)proper_exc = execute(cmd_run);
					else cout << "Cannot Execute Command, Previous CMD faliled." << endl;

					cmd_run.clear();
			
				}
				else
				{
					cmd_run.pop_back();
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					found_ands = true;
					input_list.at(i) = const_cast<char *>(temp.substr(temp.find("&&")+2).c_str());
					cmd_run.push_back(input_list.at(i));
		
				}
			}
			if(temp.find("||") != string::npos && no_exit && temp != "||" )
			{
				if(temp.find("||") != 0)
				{
					input_list.at(i) = const_cast<char *>(temp.substr(0, temp.find("||")).c_str());
					cmd_run.pop_back();
				
					cmd_run.push_back(input_list.at(i));
					
					if(proper_exc && !found_par)proper_exc = execute(cmd_run);
					else if(!proper_exc && found_par)proper_exc = execute(cmd_run);
					
					else cout << "Cannot Execute Command, Previous CMD Succeeded." << endl;

					cmd_run.clear();
					found_par = true;
			
				}
				else
				{
					cmd_run.pop_back();
					proper_exc = execute(cmd_run);
					cmd_run.clear();
					found_par = true;
					input_list.at(i) = const_cast<char *>(temp.substr(temp.find("||")+2).c_str());
					cmd_run.push_back(input_list.at(i));
		
				}
			}



		}
		if(no_exit && !found_ands && !found_par)proper_exc = execute(cmd_run);
		else if(no_exit && found_ands)
		{
			if(proper_exc)proper_exc = execute(cmd_run);
			else
			{
				cout << "Cannot Execute Command, Previous CMD faliled." << endl;
			}

		}
		else if(no_exit && found_par)
		{
			if(!proper_exc)proper_exc = execute(cmd_run);
			else
			{
				cout << "Cannot Execute Command, Previous CMD Succeeded." << endl;
			}
		}
		cout << endl;
		cmd_run.clear();
		cin.clear();
	}
	}
	return 0;
}
