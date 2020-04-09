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
#include "LRUCache.h"

using namespace std;

int cmdStat = 0;
vector<vector<string>> dataPlaceholder;
vector<string> record;
string latX, latY, longX, longY;
HashFunction* hashfunc = new SimpleStringHash();
QuadraticProbing* q = new QuadraticProbing();
Hashtable hashtable = Hashtable(1024, hashfunc, q);
Quad* quadTree;
LRUCache buffer(5);

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
		dataPlaceholder.push_back(record);
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
	if (dms == "Unknown")
	{
		return 0;
	}
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
			cout << data.front() << ", this might take a while..." << endl;
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
	string featureName, stateAB;
	int i = 0;
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			if (i == 0)
			{
				featureName = data.front();
			}
			else if (i == 1)
			{
				stateAB = data.front();
			}
			data.pop();
			i++;
		}
	}
	cout << featureName << " " << stateAB << endl;
	dataSearch(0, featureName, stateAB, 0, 0, databaseFile);
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
	for (auto record : dataPlaceholder)
	{
		if (record.size())
		{
			hashtable.insert(record[2] + ":" + record[4], stoi(record[0]));
		}
	}
}

void buildQuadTree()
{
	quadTree = new Quad(Coordinate(DMStoSecond(longX), DMStoSecond(latX)), Coordinate(DMStoSecond(longY), DMStoSecond(latY)));

	for (auto record : dataPlaceholder)
	{
		if (record.size())
		{
			Node* temp = new Node(Coordinate(DMStoSecond(record[9]), DMStoSecond(record[8])), stoi(record[0]));
			quadTree->insert(temp);
		}
	}
}

void caching(vector<string> record)
{
	buffer.save(stoi(record[0]), record);
}

void writeDatabase(string fileName)
{
	ofstream dataFile(fileName);
	for (auto record : dataPlaceholder)
	{
		for (auto entry : record)
		{
			dataFile << entry << " ";
		}
		dataFile << endl;
	}
}

void findOffset(int offset, string dataFile)
{
	ifstream file(dataFile);
	string line;
	while (getline(file, line))
	{
		string word;
		istringstream ss(line);
		do
		{
			if (word == to_string(offset))
			{
				record.push_back(word);
				
			}
		} 
		while (ss);
	}
	caching(record);
	for (auto entry : record)
	{
		cout << entry << " ";
	}
	cout << endl;
	record.clear();
}

void dataSearch(int type, string lt, string ln, string ltOffset, string lnOffset, string dataFile)
{
	
	int offSet = 0;
	switch (type)
	{
		case 0: //what_is
			offSet = hashtable.search(lt + ln);
			findOffset(offSet, dataFile);
			break;
		case 1: //what_is_at
			break;
		case 2: //what_is_in
			break;
		case 3: //what_is_in -long
			break;
		case 4: //what_is_in -filter
			break;
	}
}

template <typename T>
void execute(int cmd, queue<T>& data, string dataFile)
{
	if (cmd == 0)
	{
		return;
	}

	switch (cmd)
	{
		case 1:
			cout << "Execute CMD World: " << endl << "World boundaries are set to: " << endl;
			world(dataFile, data);
			cmdStat = 0;
			break;
		case 2:
			cout << "Execute CMD Import: ";
			cout << "Importing ";
			import(dataFile, data);
			hashing();
			buildQuadTree();
			caching();
			cmdStat = 0;
			break;
		case 3:
			cout << "Execute CMD Debug: ";
			debug(dataFile, data);
			cmdStat = 0;
			break;
		case 4:
			cout << "Execute CMD What_is_at: ";
			whatisat(dataFile, data);
			cmdStat = 0;
			break;
		case 5:
			cout << "Execute CMD What_is: ";
			whatis(dataFile, data);
			cmdStat = 0;
			break;
		case 6:
			cout << "Execute CMD What_is_in: ";
			whatisin(dataFile, data);
			cmdStat = 0;
			break;
	}
}

void parser(string scriptFile, string dataFile, string logFile)
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
				dataPlaceholder.clear();
				if (word == "world")
				{
					execute(cmdStat, data, dataFile);
					cmdStat = 1;
				}
				else if (word == "import")
				{
					execute(cmdStat, data, dataFile);
					writeDatabase(dataFile);
					cout << "Finished!" << endl;
					cmdStat = 2;
				}
				else if (word == "debug")
				{
					execute(cmdStat, data, dataFile);
					cmdStat = 3;
				}
				else if (word == "what_is_at")
				{
					execute(cmdStat, data, dataFile);
					cmdStat = 4;
				}
				else if (word == "what_is")
				{
					execute(cmdStat, data, dataFile);
					cmdStat = 5;
				}
				else if (word == "what_is_in")
				{
					execute(cmdStat, data, dataFile);
					cmdStat = 6;
				}
				else if (word == "quit")
				{
					execute(cmdStat, data, dataFile);
					
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
	parser("DemoScript02.txt", "db02.txt", "Log02.txt");
	return 0;
}