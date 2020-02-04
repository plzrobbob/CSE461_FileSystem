#ifndef TABLE_H
#define TABLE_H

#include "Filesys.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Table : public Filesys
{
public:
	Table(string diskname, int numberofblocks, int blocksize, string flatfile,
		string indexfile);
	int buildTable(string inputFile);
	int search(string value);
private:
	string flatfile;
	string indexfile;
	int numberofrecords;
	int indexSearch(string value);
};
Table::Table(string diskname, int numberofblocks, int blocksize,
	string flatfile, string indexfile) :
	Filesys(diskname, numberofblocks, blocksize)
{
	this->flatfile = flatfile;
	this->indexfile = indexfile;

	if (newfile(this->flatfile) == 0)
	{
		cout << "Error creating flatfile!" << endl;
	}

	if (newfile(this->indexfile) == 0)
	{
		cout << "Error creating indexfile!" << endl;
	}
	//displayRoot();

	//flatfile = _flatfile;
	//indexfile = _indexfile;

	cout << "Table object successfully created!" << endl;
}

int Table::buildTable(string inputfile)
{
	string record;
	int count = 0;
	vector<string> key;
	vector<int> iblock;

	ifstream infile;
	infile.open(inputfile.c_str());

	//getline(infile,record);

	ostringstream outstream;

	while (getline(infile, record))
	{
		//process the record
		string pkey = record.substr(0, 5);
		vector<string> oblock = block(record, getblocksize());
		int blockID = addblock(flatfile, oblock[0]);
		//key.push_back(pkey);
		//iblock.push_back(blockID);
		outstream << pkey << " " << blockID << " ";
		++count; //keep track of records written to outstream
		//cout << "Count: " << count << endl;

		if (count == 4 || infile.eof())
		{
			string outbuffer = outstream.str();
			vector<string> o2block = block(outbuffer, getblocksize());
			//cout << outbuffer << endl;
			if (addblock(indexfile, o2block[0]) <= 0)
			{

				cout << "No more room!" << endl;
				return 0;
			}

			count = 0;
			outstream.str("");
			if (infile.eof())
			{
				return 1;
			}
		}

		//getline(infile, record);
	}
	return 1;
}

int Table::search(string value)
{
	int block = indexSearch(value);
	if (block == 0)
	{
		cout << "No record!" << endl;
		return 0;
	}
	else
	{
		string buffer;
		readblock(flatfile, block, buffer);

		while (buffer[buffer.size() - 1] == '#')
		{
			buffer.pop_back();
		}

		cout << buffer << endl;
		return 1;
	}
	return 0;
}

int Table::indexSearch(string value)
{
	int blockID = getfirstblock(indexfile);
	if (blockID == -1)
	{
		cout << "Index file not found!" << endl;
		return 0;
	}

	while (blockID != 0)
	{
		string buffer, k;
		int b;
		istringstream instream;

		readblock(indexfile, blockID, buffer);

		instream.str(buffer);

		for (int i = 1; i <= 4; ++i)
		{
			instream >> k >> b;
			if (k == value)
			{
				return b; //record found!
			}
		}
		blockID = nextblock(indexfile, blockID); //move to next block
	}
	return 0; //error, file not found
}

#endif
