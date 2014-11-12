#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <stack>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
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

int stats(string dir, string file)
{
	struct stat f_stats;
	string direc = dir + "/" + file;
	int status = stat(direc.c_str(), & f_stats);
	
	if(status < 0)
	{
		cerr << "Error(" << errno << ") opening " << file << endl; 
		return errno; 
	}
	struct passwd *pw;
	if((pw = getpwuid(f_stats.st_uid)) == NULL )
	{
		cerr << "Error(" << errno << ") Finding User" << endl; 
		return errno; 
	}
	struct group  *gr;
	if((gr = getgrgid(f_stats.st_gid)) == NULL )
	{
		cerr << "Error(" << errno << ") Finding Group" << endl; 
		return errno; 
	}
	if (S_ISDIR(f_stats.st_mode)) cout << "d";
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
	cout << pw->pw_name << " " << gr->gr_name << " ";
	cout.width(5);
	cout << right << f_stats.st_size << " ";
	struct tm *timeinfo; 
	char buffer [80]; 
	timeinfo = localtime (&f_stats.st_mtime); 
	strftime(buffer, 80, "%b %d %R", timeinfo); 
	cout << buffer << " ";
}

int  print_dirL(string dir, vector<string> a)
{
	sort(a.begin(),a.end(),string_case);
	for(int i = 0; i < a.size(); ++i)
	{
		stats(dir, a.at(i));
		struct stat f_stats;
		string direc = dir + "/" + a.at(i);
		int status = stat(direc.c_str(), & f_stats);
	
		if(status < 0)
		{
			cerr << "Error(" << errno << ") opening " << a.at(i) << endl; 
			return errno; 
		}
		if (S_ISDIR(f_stats.st_mode)) cout << "\e[1m" << "\e[94m"<< a.at(i) << "\e[0m"<< "/" << endl;
		else if (S_IEXEC & f_stats.st_mode) cout << "\e[1m" << "\e[92m"<< a.at(i) << "\e[0m"<< "*" << endl;
		else if (S_ISDIR(f_stats.st_mode) && a.at(i).at(0) == '.') cout << "\e[1m" << "\e[94m"<< a.at(i) << "\e[0m"<< "/" << endl;
		else if (S_IEXEC & f_stats.st_mode && a.at(i).at(0) == '.') cout << "\e[1m" << "\e[92m"<< a.at(i) << "\e[0m"<< "*" << endl;
		else cout << a.at(i) << endl;
	}
	cout << endl;
}

int print_dir(string dir , vector<string> a)
{
	sort(a.begin(),a.end(),string_case);
	int count = 0;
	for(int i = 0; i < a.size(); ++i)
	{
		struct stat f_stats;
		string direc = dir + "/" + a.at(i);
		int status = stat(direc.c_str(), & f_stats);
	
		if(status < 0)
		{
			cerr << "Error(" << errno << ") opening " << a.at(i) << endl; 
			return errno; 
		}
		if (S_ISDIR(f_stats.st_mode)) cout << "\e[1m" << "\e[94m"<< a.at(i) << "\e[0m" << "/" << endl;
		else if (S_IEXEC & f_stats.st_mode) cout << "\e[1m" << "\e[92m"<< a.at(i) << "\e[0m" << "*" << endl;
		else cout << a.at(i) << endl;
	}
	cout << endl;

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
	if(flag2 == 1)print_dirL(dir, all_files);
	else print_dir(dir ,all_files);
}


int list_allR(const string &dir,int flag1, int flag2, int flag3)
{
	vector<string>all_files;
	cout << dir << ":" << endl;
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
	if(flag2 == 1)print_dirL(dir, all_files);
	else print_dir(dir ,all_files);
	closedir(curr_dir);
	DIR *curr_dir2;
	if (!(curr_dir2 = opendir(dir.c_str()))) 
	{ 
		cerr << "Error(" << errno << ") opening " << dir << endl; 
		return errno; 
	} 
	
	if(curr_dir2)
	{
		struct dirent *tmp_dir2;
		while( (tmp_dir2 = readdir(curr_dir2)) != NULL)
		{
			if (tmp_dir2->d_name[0]=='.' && flag1 == 0) 
			continue;
			string file_name2 = tmp_dir2->d_name;
			string temp = tmp_dir2->d_name;
			
			if((tmp_dir2->d_type & DT_DIR))
			{
			 if(temp.compare(".") != 0  && temp.compare("..") !=  0)
			{
	 		 string reccur = dir +  "/" + file_name2;
			 list_allR(reccur, flag1, flag2, flag3);
			}
			}	
		}
	}
	closedir(curr_dir2);
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
	string str5 = "-R";
	string str6 = "-Ra";
	string str7 = "-aR";
	string str8 = "-Rl";
	string str9 = "-lR";
	string str10 = "-Ral";
	string str11 = "-Rla";
	string str12 = "-lRa";
	string str13 = "-aRl";
	string str14 = "-alR";
	string str15 = "-laR";


	vector<string>dir_in;
	
	for(int i = 1; i < argc; ++i)
	{
	
		if(str1.compare(argv[i]) == 0 )flag2 = 1;
		else if(str2.compare(argv[i]) == 0 )flag1 = 1;
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
		else if(str5.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		}
		else if(str6.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag1 = 1;
		}
		else if(str7.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag1 = 1;
		}	
		else if(str8.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		}
		else if(str9.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		}
		else if(str10.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		  flag1 = 1;	
		}
		else if(str11.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		  flag1 = 1;	
		}
		else if(str12.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		  flag1 = 1;	
		}
		else if(str13.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		  flag1 = 1;	
		}	
		else if(str14.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		  flag1 = 1;	
		}	
		else if(str15.compare(argv[i]) == 0)
		{
		  flag3 = 1;
		  flag2 = 1;
		  flag1 = 1;	
		}	
		else dir_in.push_back(argv[i]);
	}
	if(dir_in.size() == 0)dir_in.push_back(".");
	for(int i = 0; i < dir_in.size(); ++i)
	{
	  if(flag3 == 1)
	   {
		list_allR(dir_in.at(i),flag1,flag2,flag3);
	   }
	   else list_all(dir_in.at(i),flag1,flag2,flag3);
	  cout << endl;
	}	
	return 0;
}
