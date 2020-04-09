#pragma once
#include <list>
#include <map>
#include <vector>
using namespace std;

class LRUCache {

	list<int> key;

	map<int, pair <list<int>::iterator, vector<string>>> pool;
	int bufferSize;

public:
	LRUCache(int);
	void save(int, vector<string>);
	void display();
};

LRUCache::LRUCache(int n)
{
	bufferSize = n;
}

void LRUCache::save(int offset, vector<string> record)
{ 
	if (pool.find(offset) == pool.end()) {

		if (key.size() == bufferSize) {

			int last = key.back();

			key.pop_back();

			pool.erase(last);
		}
	}

	else
	{
		key.erase(pool[offset].first);
	}
	key.push_front(offset);
	pool[offset].first = key.begin();
	pool[offset].second = record;
}

void LRUCache::display()
{
	for (auto record = pool.rbegin(); record != pool.rend(); record++)
	{
		cout << record->first << ": ";
		for (int i = 1; i < record->second.second.size(); i++)
		{
			cout << record->second.second[i] << " ";
		}
		cout << endl;
	}
}