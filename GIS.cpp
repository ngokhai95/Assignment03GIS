#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>
#include <map>
#include <vector>
#include "HashTable.h"
#include "QuadTree.h"

using namespace std;

int cmdStat = 0;
vector<vector<string>> database;
string latX, latY, longX, longY;

void readDatabase(string fileName)
{
	ifstream file(fileName);
	string line;
	int i = 0;
	while (getline(file, line))
	{
		string temp;
		stringstream check(line);
		vector<string> record;
		int j = 0;
		if (i != 0)
		{
			record.push_back(to_string(i));
			while (getline(check, temp, '|'))
			{
				switch (j)
				{
					case 0: //FEATURE_ID
						record.push_back(temp);
						break;
					case 1: //FEATURE_NAME
						record.push_back(temp);
						break;
					case 2: //FEATURE_CLASS
						record.push_back(temp);
						break;
					case 3: //STATE_ALPHA
						record.push_back(temp);
						break;
					case 4: //STATE_NUMERIC
						record.push_back(temp);
						break;
					case 5: //COUNTY_NAME
						record.push_back(temp);
						break;
					case 6: //COUNTY_NUMERIC
						record.push_back(temp);
						break;
					case 7: //PRIMARY_LAT_DMS
						record.push_back(temp);
						break;
					case 8: //PRIM_LONG_DMS
						record.push_back(temp);
						break;
					case 9: //PRIM_LAT_DEC
						record.push_back(temp);
						break;
					case 10: //PRIM_LONG_DEC
						record.push_back(temp);
						break;
					case 11: //SOURCE_LAT_DMS
						record.push_back(temp);
						break;
					case 12: //SOURCE_LONG_DMS
						record.push_back(temp);
						break;
					case 13: //SOURCE_LAT_DEC
						record.push_back(temp);
						break;
					case 14: //SOURCE_LONG_DEC
						record.push_back(temp);
						break;
					case 15: //ELEV_IN_M
						record.push_back(temp);
						break;
					case 16: //ELEV_IN_FT
						record.push_back(temp);
						break;
					case 17: //MAP_NAME
						record.push_back(temp);
						break;
					case 18: //DATE_CREATED
						record.push_back(temp);
						break;
					case 19: //DATE_EDITED
						record.push_back(temp);
						break;
				}
				j++;
			}	
		}		
		database.push_back(record);
		i++;
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
	return seconds;
}

template <typename T>
void world(string databaseFile, queue<T>& data)
{
	int i = 0;
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
			switch (i)
			{
				case 0:
					longX = data.front();
					break;
				case 1:
					longY = data.front();
					break;
				case 2:
					latX = data.front();
					break;
				case 3: 
					latY = data.front();
					break;
			}
			data.pop();
		}
		i++;
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

void hashing()
{
	HashFunction* hash = new SimpleStringHash();
	QuadraticProbing* q = new QuadraticProbing();
	Hashtable hashtable = Hashtable(1024, hash, q);
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].size())
		{
			hashtable.insert(database[i][2] + ":" + database[i][4], stoi(database[i][0]));
		}

	}
	hashtable.display();
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
			cout << "Execute CMD World: " << endl << "World boundaries are set to: " << endl;
			world("yes", data);
			cmdStat = 0;
			break;
		case 2:
			cout << "Execute CMD Import: ";
			cout << "Importing ";
			import("yes", data);
			cout << "Imported! Hashing..." << endl;
			hashing();
			cout << "Finished!" << endl;
			cmdStat = 0;
			break;
		case 3:
			cout << "Execute CMD Debug: ";
			debug("yes", data);
			cmdStat = 0;
			break;
		case 4:
			cout << "Execute CMD What_is_at: ";
			whatisat("yes", data);
			cmdStat = 0;
			break;
		case 5:
			cout << "Execute CMD What_is: ";
			whatis("yes", data);
			cmdStat = 0;
			break;
		case 6:
			cout << "Execute CMD What_is_in: ";
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
	parser("DemoScript02.txt");
	Quad world(Coordinate(-10000000, -10000000), Coordinate(10000000, 10000000));
	Node temp;
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].size())
		{
			temp.pos = Coordinate(DMStoSecond(database[i][9]), DMStoSecond(database[i][8]));
			temp.data = stoi(database[i][0]);
			cout << "Insert (" << temp.pos.x << "," << temp.pos.y << ") and " << temp.data <<endl;
			world.insert(&temp);
			
		}
	}
	//cout << DMStoSecond("1074019W") << " and " << DMStoSecond("380145N") << endl;
	 cout << "Location: " << world.search(Coordinate(DMStoSecond("1074146W"), DMStoSecond("375437N")))->data << "\n";
	//cout << "Location: " << center.search(Coordinate(-387902, 137347))->data << "\n";
	return 0;
}