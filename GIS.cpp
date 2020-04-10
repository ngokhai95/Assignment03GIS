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

int cmdExec = 0;
int cmdStat = 0;
bool checkSp = false;
vector<vector<string>> dataPlaceholder;
vector<string> record;
string latX, latY, longX, longY;
string water[18] = {"Arroyo", "Basin", "Bay", "Bend", "Canal", "Channel", "Falls", "Glacier", "Gut", "Harbor", "Lake", "Rapids", "Reservoir", "Sea", "Spring", "Stream", "Swamp", "Well"};
string structure[12] = {"Airport", "Bridge", "Building", "Church", "Dam",  "Hospital", "Levee", "Park", "Post Office", "School", "Tower", "Tunnel"};
string pop[1] = {"Populated Place"};
HashFunction* hashfunc = new SimpleStringHash();
QuadraticProbing* q = new QuadraticProbing();
Hashtable hashtable = Hashtable(1024, hashfunc, q);
Quad* quadTree;
LRUCache pool(15);
ofstream outputLog;
ifstream checkF;
string scriptF, dbF, logF;

string DMStoString(string dms)
{
	string degree, minute, second;
	char direction = ' ';
	string result;
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
	if (direction == 'W')
	{
		result = degree + "d" + minute + "m" + second + "s" + " West";
	}
	else if (direction == 'S')
	{
		result = degree + "d" + minute + "m" + second + "s" + " South";
	}
	else if (direction == 'N')
	{
		result = degree + "d" + minute + "m" + second + "s" + " North";
	}
	else if (direction == 'E')
	{
		result = degree + "d" + minute + "m" + second + "s" + " East";
	}
	
	return result;
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

void hashing()
{
	for (auto record : dataPlaceholder)
	{
		if (record.size())
		{
			hashtable.insert(record[2] + " " + record[4], stoi(record[0]));
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
	if(!checkSp)
		pool.save(stoi(record[0]), record);
}

void writeDatabase(string fileName)
{
	ofstream dataFile(fileName);
	for (auto record : dataPlaceholder)
	{
		for (auto entry : record)
		{
			dataFile << entry << "|";
		}
		dataFile << endl;
	}
}

vector<string> findOffset(int offset, string dataFile)
{
	ifstream file(dataFile);
	string line;
	int i = 0;
	while (getline(file, line))
	{
		if (i != 0)
		{
			string word;
			stringstream check(line);
			while (getline(check, word, '|'))
			{
				if (word == to_string(offset))
				{
					while (getline(check, word, '|'))
					{
						record.push_back(word);
					}
						
				}
			}
		
		}
		i++;
		if (record.size() > 15)
		{
			break;
		}
	}
	caching(record);
	return record;
}

void dataSearch(int type, string lt, string ln, string ltOffset, string lnOffset, string filter, string dataFile)
{
	
	vector<int> offsetList;
	switch (type)
	{
	case 0: //what_is
		if (hashtable.search(lt).size())
		{
			offsetList = hashtable.search(lt);
		}
		for (auto offset : offsetList)
		{
			record.clear();
			dataPlaceholder.push_back(findOffset(offset, dataFile));
		}
		outputLog << endl;
		outputLog << "The following " << offsetList.size() << " feature was found: " << endl;
		outputLog << endl;
		for (int i = 0; i < offsetList.size(); i++)
		{
			if (dataPlaceholder[i].size())
			{
				outputLog << offsetList[i] << ": " << dataPlaceholder[i][5] << " (" << DMStoString(dataPlaceholder[i][8]) << ", " << DMStoString(dataPlaceholder[i][7]) << ")" << endl;
			}
		}
		outputLog << endl;
		dataPlaceholder.clear();
		break;
	case 1: //what_is_at
		if (quadTree->search(Coordinate(DMStoSecond(ln), DMStoSecond(lt))))
		{
			offsetList = quadTree->search(Coordinate(DMStoSecond(ln), DMStoSecond(lt)))->bucketOffset;
		}
		for (auto offset : offsetList)
		{
			record.clear();
			dataPlaceholder.push_back(findOffset(offset, dataFile));
		}
		outputLog << endl;
		outputLog << "The following " << offsetList.size() << " feature was found: " << endl;
		outputLog << endl;
		for (int i = 0; i < offsetList.size(); i++)
		{
			if (dataPlaceholder[i].size())
			{
				outputLog << offsetList[i] << ": " << dataPlaceholder[i][1] << " " << dataPlaceholder[i][5] << " " << dataPlaceholder[i][3] << endl;
			}
		}
		outputLog << endl;
		dataPlaceholder.clear();
		break;
	case 2: //what_is_in
		for (int i = -stoi(lnOffset); i <= stoi(lnOffset); i++)
		{
			for (int j = -stoi(ltOffset); j <= stoi(ltOffset); j++)
			{
				if (quadTree->search(Coordinate(DMStoSecond(ln) + i, DMStoSecond(lt) + j)))
				{
					for (auto offset : quadTree->search(Coordinate(DMStoSecond(ln) + i, DMStoSecond(lt) + j))->bucketOffset)
					{
						offsetList.push_back(offset);
					}
				}
			}
		}
		for (auto offset : offsetList)
		{
			record.clear();
			dataPlaceholder.push_back(findOffset(offset, dataFile));
		}
		outputLog << endl;
		outputLog << "The following " << offsetList.size() << " feature was found: " << endl;
		outputLog << endl;
		for (int i = 0; i < offsetList.size(); i++)
		{
			if (dataPlaceholder[i].size())
			{
				outputLog << offsetList[i] << ": " << dataPlaceholder[i][1] << " " << dataPlaceholder[i][3] << " " << " (" << DMStoString(dataPlaceholder[i][8]) << ", " << DMStoString(dataPlaceholder[i][7]) << ")" << endl;
			}
		}
		dataPlaceholder.clear();
		break;
	case 3: //what_is_in -long
		for (int i = -stoi(lnOffset); i <= stoi(lnOffset); i++)
		{
			for (int j = -stoi(ltOffset); j <= stoi(ltOffset); j++)
			{
				if (quadTree->search(Coordinate(DMStoSecond(ln) + i, DMStoSecond(lt) + j)))
				{
					for (auto offset : quadTree->search(Coordinate(DMStoSecond(ln) + i, DMStoSecond(lt) + j))->bucketOffset)
					{
						offsetList.push_back(offset);
					}
				}
			}
		}
		for (auto offset : offsetList)
		{
			record.clear();
			dataPlaceholder.push_back(findOffset(offset, dataFile));
		}
		outputLog << endl;
		outputLog << "The following " << offsetList.size() << " feature was found: " << endl;
		outputLog << endl;
		for (int i = 0; i < offsetList.size(); i++)
		{
			if (dataPlaceholder[i].size())
			{
				outputLog << "Feature ID: " << dataPlaceholder[i][0] << endl;
				outputLog << "Feature Name: " << dataPlaceholder[i][1] << endl;
				outputLog << "Feature Class: " << dataPlaceholder[i][2] << endl;
				outputLog << "State: " << dataPlaceholder[i][3] << endl;
				outputLog << "County: " << dataPlaceholder[i][5] << endl;
				outputLog << "Longitude: " << DMStoString(dataPlaceholder[i][8]) << endl;
				outputLog << "Latitude: " << DMStoString(dataPlaceholder[i][7]) << endl;
				outputLog << "Elev in Feet: " << dataPlaceholder[i][16] << endl;
				outputLog << "USGS Quad: " << dataPlaceholder[i][17] << endl;
				outputLog << "Date Created: " << dataPlaceholder[i][18] << endl << endl;
			}
		}
		dataPlaceholder.clear();
		break;
	case 4: //what_is_in -filter
		for (int i = -stoi(lnOffset); i <= stoi(lnOffset); i++)
		{
			for (int j = -stoi(ltOffset); j <= stoi(ltOffset); j++)
			{
				if (quadTree->search(Coordinate(DMStoSecond(ln) + i, DMStoSecond(lt) + j)))
				{
					for (auto offset : quadTree->search(Coordinate(DMStoSecond(ln) + i, DMStoSecond(lt) + j))->bucketOffset)
					{
						offsetList.push_back(offset);
					}
				}
			}
		}
		for (auto offset : offsetList)
		{
			record.clear();
			dataPlaceholder.push_back(findOffset(offset, dataFile));
		}
		outputLog << endl << "These following feature was found: " << endl;
		if (filter == "water")
		{
			for (int i = 0; i < offsetList.size(); i++)
			{
				for (auto ftClass : water)
				{
					if (dataPlaceholder[i].size())
					{
						if (dataPlaceholder[i][2] == ftClass)
						{
							outputLog << offsetList[i] << ": " << dataPlaceholder[i][1] << " " << dataPlaceholder[i][3] << " " << " (" << DMStoString(dataPlaceholder[i][8]) << ", " << DMStoString(dataPlaceholder[i][7]) << ")" << endl;
						}
						
					}
				}
			}
		}
		else if (filter == "structure")
		{
			for (int i = 0; i < offsetList.size(); i++)
			{
				for (auto ftClass : structure)
				{
					if (dataPlaceholder[i].size())
					{
						if (dataPlaceholder[i][2] == ftClass)
						{
							outputLog << offsetList[i] << ": " << dataPlaceholder[i][1] << " " << dataPlaceholder[i][3] << " " << " (" << DMStoString(dataPlaceholder[i][8]) << ", " << DMStoString(dataPlaceholder[i][7]) << ")" << endl;
						}
						
					}
				}
			}
		}
		else if (filter == "pop")
		{
			for (int i = 0; i < offsetList.size(); i++)
			{
				for (auto ftClass : pop)
				{
					if (dataPlaceholder[i].size())
					{
						if (dataPlaceholder[i][2] == ftClass)
						{
							outputLog << offsetList[i] << ": " << dataPlaceholder[i][1] << " " << dataPlaceholder[i][3] << " " << " (" << DMStoString(dataPlaceholder[i][8]) << ", " << DMStoString(dataPlaceholder[i][7]) << ")" << endl;
						}
						
					}
				}
			}
		}
		dataPlaceholder.clear();
		break;
	}
}

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

template <typename T>
void worldCmd(string databaseFile, queue<T>& data)
{
	cout << "Getting World coordinate..." << endl;
	int i = 0;
	while (!data.empty()) 
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
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
	outputLog << "\t" << DMStoSecond(latY) << endl;
	outputLog << DMStoSecond(longX) << "\t" << "\t" << DMStoSecond(longY) << endl;
	outputLog << "\t" << DMStoSecond(latX) << endl;
	outputLog << endl;
}

template <typename T>
void importCmd(string databaseFile, queue<T>& data)
{
	cout << "Importing database..." << endl;
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			if (data.front() == "NM_All.txt")
			{
				checkSp = true;
			}
			outputLog << data.front() << endl;
			readDatabase(data.front());
			data.pop();
		}
	}
	outputLog << endl;
}

template <typename T>
void debugCmd(string databaseFile, queue<T>& data, string logFile)
{
	cout << "Debug using ";
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			if (data.front() == "hash")
			{
				cout << "Hash Table..." << endl;
				outputLog << "Hash Table: " << endl;
				hashtable.display(logFile);
			}
			else if (data.front() == "quad")
			{
				cout << "Quad Tree..." << endl;
				outputLog << "Quad Tree: " << endl;
				quadTree->displayTree(quadTree, logFile);
			}
			else if (data.front() == "pool")
			{
				cout << "Cache Pool..." << endl;
				outputLog << "Cache Pool: " << endl;
				pool.display(logFile);
			}
			data.pop();
		}
	}
	outputLog << endl;
}

template <typename T>
void whatisatCmd(string databaseFile, queue<T>& data)
{
	cout << "Searching by Quad Tree..." << endl;
	string ln, lt;
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
				lt = data.front();
			}
			else if (i == 1)
			{
				ln = data.front();
			}
			data.pop();
			i++;
		}
	}
	outputLog <<"(" << DMStoString(ln) << "," << DMStoString(lt) << ")"<< endl;
	dataSearch(1, lt, ln, "", "", "", databaseFile);
}

template <typename T>
void whatisCmd(string databaseFile, queue<T>& data)
{
	cout << "Searching by Hash Table..." << endl;
	string input;
	while (!data.empty())
	{
		if (data.size() == 1)
		{
			data.pop();
		}
		else
		{
			input = input + data.front() + " ";
			data.pop();
		}
	}
	input.pop_back();
	outputLog << input << endl;
	dataSearch(0, input, "", "", "", "", databaseFile);
}

template <typename T>
void whatisinCmd(string databaseFile, queue<T>& data)
{
	cout << "Searching by Quad Tree..." << endl;
	string ln, lt, offsetLn, offsetLt, filter;
	int i = 0;
	bool lng = false;
	bool flt = false;
	if (data.front() == "-long")
	{
		lng = true;
		while (!data.empty())
		{
			if (data.size() == 1)
			{
				data.pop();
			}
			else
			{
				if (i == 1)
				{
					lt = data.front();
				}
				else if (i == 2)
				{
					ln = data.front();
				}
				else if (i == 3)
				{
					offsetLt = data.front();
				}
				else if (i == 4)
				{
					offsetLn = data.front();
				}
				data.pop();
				i++;
			}
		}
	}
	else if (data.front() == "-filter")
	{
		while (!data.empty())
		{
			if (data.size() == 1)
			{
				data.pop();
			}
			else
			{
				flt = true;
				if (i == 1)
				{
					filter = data.front();
				}
				else if (i == 2)
				{
					lt = data.front();
				}
				else if (i == 3)
				{
					ln = data.front();
				}
				else if (i == 4)
				{
					offsetLt = data.front();
				}
				else if (i == 5)
				{
					offsetLn = data.front();
				}
				data.pop();
				i++;
			}
		}
	}
	else
	{
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
					lt = data.front();
				}
				else if (i == 1)
				{
					ln = data.front();
				}
				else if (i == 2)
				{
					offsetLt = data.front();
				}
				else if (i == 3)
				{
					offsetLn = data.front();
				}
				data.pop();
				i++;
			}
		}
	}
	if (lng)
	{
		outputLog << "(" << DMStoString(ln) << " +/- " << offsetLn << ", " << DMStoString(lt) << " +/- " << offsetLt << ")" << endl;
		dataSearch(3, lt, ln, offsetLt, offsetLn, "", databaseFile);
	}
	else if (flt)
	{
		outputLog << "(" << DMStoString(ln) << " +/- " << offsetLn << ", " << DMStoString(lt) << " +/- " << offsetLt << ")" << " with filter: "<< filter << endl;
		dataSearch(4, lt, ln, offsetLt, offsetLn, filter, databaseFile);
	}
	else
	{
		outputLog << "(" << DMStoString(ln) <<" +/- " << offsetLn << ", " << DMStoString(lt) << " +/- " << offsetLt << ")" << endl;
		dataSearch(2, lt, ln, offsetLt, offsetLn, "", databaseFile);
	}
	outputLog << endl;
}

template <typename T>
void execute(int cmd, queue<T>& data, string dataFile, string logFile)
{
	
	if (cmd == 0)
	{
		return;
	}

	switch (cmd)
	{
		case 1:
			outputLog << "Execute CMD World: " << endl << "World boundaries are set to: " << endl;
			worldCmd(dataFile, data);
			cmdStat = 0;
			cmdExec++;
			break;
		case 2:
			outputLog << "--------------------------------------------------------------------------------" << endl;
			outputLog << cmdExec <<".Execute CMD Import: ";
			outputLog << "Importing ";
			importCmd(dataFile, data);
			writeDatabase(dataFile);
			hashing();
			buildQuadTree();
			dataPlaceholder.clear();
			cmdStat = 0;
			cmdExec++;
			break;
		case 3:
			outputLog << "--------------------------------------------------------------------------------" << endl;
			outputLog << cmdExec << ".Execute CMD Debug: ";
			debugCmd(dataFile, data, logFile);
			cmdStat = 0;
			cmdExec++;
			break;
		case 4:
			outputLog << "--------------------------------------------------------------------------------" << endl;
			outputLog << cmdExec << ".Execute CMD What_is_at: ";
			whatisatCmd(dataFile, data);
			cmdStat = 0;
			cmdExec++;
			break;
		case 5:
			outputLog << "--------------------------------------------------------------------------------" << endl;
			outputLog << cmdExec << ".Execute CMD What_is: ";
			whatisCmd(dataFile, data);
			cmdStat = 0;
			cmdExec++;
			break;
		case 6:
			outputLog << "--------------------------------------------------------------------------------" << endl;
			outputLog << cmdExec << ".Execute CMD What_is_in: ";
			whatisinCmd(dataFile, data);
			cmdStat = 0;
			cmdExec++;
			break;
	}
	
}

void parser(string scriptFile, string dataFile, string logFile)
{
	ifstream file(scriptFile);
	string line;
	queue<string> data;
	outputLog.open(logFile, fstream::app);
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
					execute(cmdStat, data, dataFile, logFile);
					cmdStat = 1;
				}
				else if (word == "import")
				{
					execute(cmdStat, data, dataFile, logFile);
					cmdStat = 2;
				}
				else if (word == "debug")
				{
					execute(cmdStat, data, dataFile, logFile);
					cmdStat = 3;
				}
				else if (word == "what_is_at")
				{
					execute(cmdStat, data, dataFile, logFile);
					cmdStat = 4;
				}
				else if (word == "what_is")
				{
					execute(cmdStat, data, dataFile, logFile);
					cmdStat = 5;
				}
				else if (word == "what_is_in")
				{
					execute(cmdStat, data, dataFile, logFile);
					cmdStat = 6;
				}
				else if (word == "quit")
				{
					execute(cmdStat, data, dataFile, logFile);
				}
				else
				{
					data.push(word);
				}
			} 
			while (ss);
		}
	}
	outputLog << "--------------------------------------------------------------------------------" << endl;
	outputLog << cmdExec << ".Execute CMD: Quit!" << endl;
	outputLog.close();
}

void clearFile(string fileName)
{
	outputLog.open(fileName, fstream::trunc);
	outputLog.close();
}
int main()
{
	cout << "Enter the script file name: ";
	cin >> scriptF;
	checkF.open(scriptF);
	while (checkF.fail()) 
	{
		cout << "Could not find the script file. Please re enter the correct file name: ";
		cin >> scriptF;
		checkF.open(scriptF);
	}
	checkF.close();
	cout << "Enter the database file name: ";
	cin >> dbF;
	cout << "Enter the log file name: ";
	cin >> logF;
	clearFile(logF);
	cout << "Executing script, this could take very long for large datafiles" << endl;
	parser(scriptF, dbF, logF);
	cout << "Finish! Check the Log file for results" << endl;
	return 0;
}