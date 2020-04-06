//Hash Table Implementation from Lab 05 and 06

#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <random>

using namespace std;

/// Returns the bit index of the most significant bit.
/// If the input is zero, it returns zero
int mostSignificantBit(int x) {
    if (x == 0)
        return 0;
    int mostSignificantBit = 31;
    while ((x & (1 << mostSignificantBit)) == 0)
        mostSignificantBit--;
    return mostSignificantBit;
}

/// Good prime numbers to use as Hashtable sizes
int GoodPrimeNumbers[] = { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
    24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
    12582917, 25165843, 50331653, 100663319, 201326611, 402653189,
    805306457, 1610612741
};

class HashFunction 
{
public:
    virtual unsigned int operator()(string key) const = 0;
    virtual ~HashFunction() = default;
};

class SimpleStringHash : public HashFunction 
{
public:
    // Computes a simple hash value for a string
    unsigned int operator()(string s) const 
	{
        unsigned int hash = 0;
        for (int i = 0; i < s.size(); i++) 
		{
            hash += s[i];
        }
        return hash;
    }
};

class QuadraticProbing 
{
public:
    unsigned int operator()(int i) const 
    {
        return (i * i + i) / 2;
    }
};

class Hashtable {
    enum BucketStatus { EMPTY, OCCUPIED, DELETED };

    /// The total number of collisions since the creation of the hashfunction.
    /// This includes the collisions encountered during insertions, searches,
    /// and deletions
    int numCollisions;

    /// Capacity of the hashtable
    int c;
    double size;

    /// All the buckets in the hashtable
    vector<pair<string,int>> buckets;

    /// Status of all the buckets
    vector<BucketStatus> status;

    HashFunction* hash;

    QuadraticProbing* q;

    /// Adjusts the capacity of the hashtable and rehashes
    void expandAndRehash()
    {
        cout << "More than 70% storage, Expanding and Rehashing..." << endl;
        //create 2 temp vector to hold current buckets and status data
		vector<pair<string, int>> tempBucket = buckets;
        vector<BucketStatus> tempStatus = status;
        //resize of bucket and status by the next prime number
        int msb = mostSignificantBit(c);
        c = GoodPrimeNumbers[msb <= 4 ? 0 : msb - 4];
        buckets.resize(c);
        status.resize(c);
        size = 0;
        //initialize the expanded status vector to empty
        for (int i = 0; i < status.size(); i++)
        {
            status[i] = EMPTY;
        }
        //rehashing by calling insert on OCCUPIED status item
        for (int i = 0; i < tempBucket.size(); i++)
        {
            if (tempStatus[i] == OCCUPIED)
            {
                insert(tempBucket[i].first, tempBucket[i].second);
            }
        }
    }

public:
    Hashtable(int n, HashFunction* _h, QuadraticProbing* _q) : numCollisions(0), hash(_h), q(_q) 
    {
        //int c = n * 15 / 10; // Initialize to 150% of the maximum size. This is a bad choice; change it!
        // TODO initialize the buckets and status to the given capacity

        //initialize lists size and elements.
        c = n;
        buckets.resize(c);
        status.resize(c);
        for (int i = 0; i < c; i++)
        {
            status[i] = EMPTY;
        }

    }

    ~Hashtable() {
    }

    /// Tries to insert the given key into the hashtable.
    /// Returns true if the element was inserted and false if not.
    /// The insertion will fail if the element already exists in the input.
    void insert(string key, int value)
    {
        if ((size / buckets.size()) > 0.7)
        {
            expandAndRehash();
        }
        int hkey = (*hash)(key) % c;
        srand(hkey);
        //while hash key status is occupied , create a different hash key value
        while (status[hkey] == OCCUPIED)
        {
            hkey = (rand() + (*q)(hkey)) % c;
        }
        //when found empty space, assign key value to hash key and assign its status to occupied
        status[hkey] = OCCUPIED;
        buckets[hkey].first = key;
		buckets[hkey].second = value;
        size++;
    }

    bool search(string key)
    {
        int hkey = (*hash)(key) % c;
        srand(hkey);
        //while hash key status is not empty, if the hash key status is occupied and its bucket value is the same as the key then return true
        while (status[hkey] != EMPTY)
        {
            if (status[hkey] == OCCUPIED && buckets[hkey].first == key)
            {
                return true;
            }
            //create a different hash key value
            hkey = (rand() + (*q)(hkey)) % c;
        }
        return false;
    }

    bool erase(string key)
    {
        if ((size / buckets.size()) > 0.7)
        {
            expandAndRehash();
        }
        int hkey = (*hash)(key) % c;
        srand(hkey);
        //while the status of the has key is not empty, if its bucket value is the key, set its value to NULL and set its status to deleted.
        while (status[hkey] != EMPTY)
        {
            if (buckets[hkey].first == key)
            {
                buckets[hkey].first = "";
                status[hkey] = DELETED;
                return true;
            }
            //create a different hash key value
            hkey = (rand() + (*q)(hkey)) % c;
        }
        return false;
    }

    int getNumCollisions() const 
    {
        return numCollisions;
    }

    void display()
    {
        for (int i = 0; i < buckets.size(); i++)
        {
            if (status[i] == OCCUPIED)
            {
                cout << i << ": " << buckets[i].first << ", ["<< buckets[i].second << "]" << endl;
            }
        }
    }
};