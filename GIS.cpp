#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>

using namespace std;

int cmdStat = 0;

void DMStoString(string dms)
{
	string degree, minute, second;
	char direction = ' ';
	if (dms.size() == 7)
	{
		degree = dms.substr(0, 2);
		minute = dms.substr(2, 2);
		second = dms.substr(4, 2);
		direction = dms[6];
	}
	else if (dms.size() == 8)
	{
		degree = dms.substr(0, 3);
		minute = dms.substr(3, 2);
		second = dms.substr(5, 2);
		direction = dms[7];
	}
	cout << degree << " Degree " << minute << " Minute " << second << " Second " << "at " << direction << endl;
}

int DMStoSecond(string dms)
{
	int seconds = 0;
	int degree, minute, second;
	char direction;
	if (dms.size() == 7)
	{
		degree = stoi(dms.substr(0, 2));
		minute = stoi(dms.substr(2, 2));
		second = stoi(dms.substr(4, 2));
		direction = dms[6];
		if (direction == 'W' || direction == 'S')
		{
			seconds = -(degree * 3600 + minute * 60 + second);
		}
		else if (direction == 'E' || direction == 'N')
		{
			seconds = degree * 3600 + minute * 60 + second;
		}
	}
	else if (dms.size() == 8)
	{
		degree = stoi(dms.substr(0, 3));
		minute = stoi(dms.substr(3, 2));
		second = stoi(dms.substr(5, 2));
		direction = dms[7];
		if (direction == 'W' || direction == 'S')
		{
			seconds = -(degree * 3600 + minute * 60 + second);
		}
		else if (direction == 'E' || direction == 'N')
		{
			seconds = degree * 3600 + minute * 60 + second;
		}
	}
	cout << "Seconds: " << seconds << endl;
	return seconds;
}

template <typename T>
void world(string databaseFile, queue<T>& data)
{
	while (!data.empty()) 
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			DMStoString(data.front());
			DMStoSecond(data.front());
			data.pop();
		}
	}
	cout << endl;
}

template <typename T>
void import(string databaseFile, queue<T>& data)
{
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			cout << data.front() << " ";
			data.pop();
		}
	}
	cout << endl;
}

template <typename T>
void debug(string databaseFile, queue<T>& data)
{
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			cout << data.front() << " ";
			data.pop();
		}
	}
	cout << endl;
}

template <typename T>
void whatisat(string databaseFile, queue<T>& data)
{
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			cout << data.front() << " ";
			data.pop();
		}
	}
	cout << endl;
}

template <typename T>
void whatis(string databaseFile, queue<T>& data)
{
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			cout << data.front() << " ";
			data.pop();
		}
	}
	cout << endl;
}

template <typename T>
void whatisin(string databaseFile, queue<T>& data)
{
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			cout << data.front() << " ";
			data.pop();
		}
	}
	cout << endl;
}

template <typename T>
void execute(int cmd, queue<T>& data)
{
	if (cmd == 0)
	{
		return;
	}

	switch (cmd)
	{
		case 1:
			cout << "Execute CMD: World " << endl << "World boundaries are set to: " << endl;
			world("yes", data);
			cmdStat = 0;
			break;
		case 2:
			cout << "Execute CMD: Import File Name: ";
			import("yes", data);
			cout << "Importing..." << endl;
			cout << "Import Success!" << endl;
			cmdStat = 0;
			break;
		case 3:
			cout << "Execute CMD: Debug ";
			debug("yes", data);
			cmdStat = 0;
			break;
		case 4:
			cout << "Execute CMD: What_is_at ";
			whatisat("yes", data);
			cmdStat = 0;
			break;
		case 5:
			cout << "Execute CMD: What_is ";
			whatis("yes", data);
			cmdStat = 0;
			break;
		case 6:
			cout << "Execute CMD: What_is_in ";
			whatisin("yes", data);
			cmdStat = 0;
			break;
	}
}

void parser(string scriptFile)
{
	std::ifstream file(scriptFile);
	std::string line;
	queue<string> data;

	while (std::getline(file, line))
	{
		
		if (line[0] != ';')
		{
			string word;		
			istringstream ss(line);
			do
			{
				ss >> word;
				if (word == "world")
				{
					execute(cmdStat, data);
					cmdStat = 1;
				}
				else if (word == "import")
				{
					execute(cmdStat, data);
					cmdStat = 2;
				}
				else if (word == "debug")
				{
					execute(cmdStat, data);
					cmdStat = 3;
				}
				else if (word == "what_is_at")
				{
					execute(cmdStat, data);
					cmdStat = 4;
				}
				else if (word == "what_is")
				{
					execute(cmdStat, data);
					cmdStat = 5;
				}
				else if (word == "what_is_in")
				{
					execute(cmdStat, data);
					cmdStat = 6;
				}
				else if (word == "quit")
				{
					execute(cmdStat, data);
					cout << "Finish parsing script" << endl;
					system("pause");
				}
				else
				{
					data.push(word);
				}
			} 
			while (ss);
		}
	}

	
}


int main()
{
	parser("DemoScript03.txt");
	return 0;
}