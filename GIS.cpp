#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void parser(string scriptFile);
void import(string databaseFile);
void debug(string databaseFile);
char cmdStat;

int main()
{
	parser("DemoScript01.txt");
	return 0;
}

void parser(string scriptFile)
{
	std::ifstream file(scriptFile);
	std::string line;
	
	while (std::getline(file, line))
	{
		vector<string> words;
		if (line[0] != ';')
		{
			string word;		
			istringstream ss(line);
			do
			{
				ss >> word;
				if (word == "world")
				{

				}
				else if (word == "import")
				{

				}
				else if (word == "debug")
				{

				}
				else if (word == "what_is_at")
				{

				}
				else if (word == "what_is")
				{

				}
				else if (word == "what_is_in")
				{

				}
				else if (word == "quit")
				{

				}
			} 
			while (ss);
		}
		for (auto word : words)
		{
			cout << word << endl;
		}
	}

	
}

void import(string databaseFile)
{

}

void debug(string databaseFile)
{

}