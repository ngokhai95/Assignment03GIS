#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

int cmdStat = 0;
vector<map<string, string>> database;

void readDatabase(string fileName)
{
	ifstream file(fileName);
	string line;
	int i = 0;
	while (getline(file, line))
	{
		string temp;
		stringstream check(line);
		string key;
		map<string, string> record;
		int j = 0;
		if (i != 0)
		{
			while (getline(check, temp, '|'))
			{
				switch (j)
				{
					case 0:
						key = "01.FEATURE_ID";
						record.insert(pair<string, string>(key, temp));
						break;
					case 1:
						key = "02.FEATURE_NAME";
						record.insert(pair<string, string>(key, temp));
						break;
					case 2:
						key = "03.FEATURE_CLASS";
						record.insert(pair<string, string>(key, temp));
						break;
					case 3:
						key = "04.STATE_ALPHA";
						record.insert(pair<string, string>(key, temp));
						break;
					case 4:
						key = "05.STATE_NUMERIC";
						record.insert(pair<string, string>(key, temp));
						break;
					case 5:
						key = "06.COUNTY_NAME";
						record.insert(pair<string, string>(key, temp));						
						break;
					case 6:
						key = "07.COUNTY_NUMERIC";
						record.insert(pair<string, string>(key, temp));
						break;
					case 7:
						key = "08.PRIMARY_LAT_DMS";
						record.insert(pair<string, string>(key, temp));
						break;
					case 8:
						key = "09.PRIM_LONG_DMS";
						record.insert(pair<string, string>(key, temp));
						break;
					case 9:
						key = "10.PRIM_LAT_DEC";
						record.insert(pair<string, string>(key, temp));
						break;
					case 10:
						key = "11.PRIM_LONG_DEC";
						record.insert(pair<string, string>(key, temp));
						break;
					case 11:
						key = "12.SOURCE_LAT_DMS";
						record.insert(pair<string, string>(key, temp));
						break;
					case 12:
						key = "13.SOURCE_LONG_DMS";
						record.insert(pair<string, string>(key, temp));
						break;
					case 13:
						key = "14.SOURCE_LAT_DEC";
						record.insert(pair<string, string>(key, temp));
						break;
					case 14:
						key = "15.SOURCE_LONG_DEC";
						record.insert(pair<string, string>(key, temp));
						break;
					case 15:
						key = "16.ELEV_IN_M";
						record.insert(pair<string, string>(key, temp));
						break;
					case 16:
						key = "17.ELEV_IN_FT";
						record.insert(pair<string, string>(key, temp));
						break;
					case 17:
						key = "18.MAP_NAME";
						record.insert(pair<string, string>(key, temp));
						break;
					case 18:
						key = "19.DATE_CREATED";
						record.insert(pair<string, string>(key, temp));
						break;
					case 19:
						key = "20.DATE_EDITED";
						record.insert(pair<string, string>(key, temp));
						break;
				}
				j++;
			}
		}
		database.push_back(record);
		i++;
	}
	for (auto data : database.back())
	{
		cout << data.first << ": " << data.second << endl;
	}
}

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
			cout << data.front() << " " << endl;
			readDatabase(data.front());
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
	ifstream file(scriptFile);
	string line;
	queue<string> data;

	while (getline(file, line))
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
	parser("DemoScript04.txt");
	return 0;
}