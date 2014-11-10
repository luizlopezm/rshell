#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <stack>
#include <errno.h> 
#include <stdio.h> 
#include <time.h>
#include <vector>
#include <algorithm>
#include <unistd.h>


using namespace std;

int FLAG_A = 0;
int FLAG_L = 1;
int FLAG_R = 2;


struct case_char: public std::binary_function< char,char,bool >
{
  	bool operator () (char x, char y) const
        {
            return toupper( static_cast< unsigned char >(x)) < 
                   toupper( static_cast< unsigned char >(y));
        }
};

bool string_case(const string &a,const string &b)
{
	string temp;
	string temp2;
	if(a.at(0) == '.')
	{
	 temp = a.substr(1);
	}
	else temp = a;
	if(b.at(0) == '.')
	{
	 temp2 = b.substr(1);
	}
	else temp2 = b;

	const string temp3 = temp;
	const string temp4 = temp2;
	return lexicographical_compare( temp3.begin(),temp3.end(), temp4.begin(),temp4.end(), case_char() );
}

int stats(string file)
{
	struct stat f_stats;
	int status = stat(file.c_str(), & f_stats);
	
	if(status < 0);
	{
		cout << "FAILED"<< status + 1 << endl;
	//	cerr << "Error(" << errno << ") opening " << file << endl; 
	//	return errno; 
	}
	if (f_stats.st_mode & S_ISDIR) cout << "d";
	else cout <<"-";
	if (f_stats.st_mode & S_IRUSR) cout << "r"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IWUSR) cout << "w"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IXUSR) cout << "x"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IRGRP) cout << "r"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IWGRP) cout << "w"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IXGRP) cout << "x"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IROTH) cout << "r"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IWOTH) cout << "w"; 
	else cout << "-"; 
	if (f_stats.st_mode & S_IXOTH) cout << "x"; 
	else cout << "-"; 
	cout << " ";
	cout << f_stats.st_nlink << " ";
	cout.width(10);
	cout << right << f_stats.st_size << " ";
	struct tm *timeinfo; 
	char buffer [80]; 
	timeinfo = localtime (&f_stats.st_mtime); 
	strftime(buffer, 80, "%b %d %R", timeinfo); 
	cout << buffer << " ";
}

void print_dirL(vector<string> a)
{
	vector<string>dot_stripped;
	sort(a.begin(),a.end(),string_case);
	for(int i = 0; i < a.size(); ++i)
	{
		stats(a.at(i));
		cout << a.at(i) << endl;
	}
	return;
}

void print_dir(vector<string> a)
{
	sort(a.begin(),a.end(),string_case);
	for(int i = 0; i < a.size(); ++i)
	{
		cout << a.at(i) << endl;
	}
	return;
}

int list_all(const string &dir,int flag1, int flag2, int flag3)
{
	vector<string>all_files;
	DIR *curr_dir;
	if (!(curr_dir = opendir(dir.c_str()))) 
	{ 
		cerr << "Error(" << errno << ") opening " << dir << endl; 
		return errno; 
	} 
	
	if(curr_dir)
	{
		struct dirent *tmp_dir;
		while( (tmp_dir = readdir(curr_dir)) != NULL)
		{
			if (tmp_dir->d_name[0]=='.' && flag1 == 0) 
			continue;
			string file_name = tmp_dir->d_name;
			all_files.push_back(file_name);						
		}
	}
	closedir(curr_dir);
	if(flag2 == 1)print_dirL(all_files);
	if(flag1 == 0 && flag2 == 0 && flag3 == 0)print_dir(all_files);
	return 0;
}


int main(int argc, char *argv[])
{
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	string str1 = "-l";
	string str2 = "-a";
	string str3 = "-la";
	string str4 = "-al";

	vector<string>dir_in;
	
	for(int i = 1; i < argc; ++i)
	{
	
		if(str1.compare(argv[i]) == 0 )flag2 = 1;
		else if(str2.compare(argv[i]) == 0 )flag1 = 0;
		else if(str3.compare(argv[i]) == 0 )
		{
		  flag1 = 1;
		  flag2 = 1;
		}
		else if(str4.compare(argv[i]) == 0 )
		{
		  flag1 = 1;
		  flag2 = 1;
		}
		else dir_in.push_back(argv[i]);
	}
	for(int i = 0; i < dir_in.size(); ++i)
	{
	  list_all(dir_in.at(i),flag1,flag2,flag3);
	}	
	return 0;
}	
