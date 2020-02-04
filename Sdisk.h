/*
Cameron Maclean
CSUSB
November 11th, 2019
lab3/4
*/

#ifndef SDISK_H
#define SDISK_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Sdisk
{
public:
	Sdisk(string _diskname, int _numberofblocks, int _blocksize);
	int getblock(int blocknumber, string& buffer);
	int putblock(int blocknumber, string buffer);
	int getnumberofblocks(); //accessor function
	int getblocksize(); //accessor function
private:
	string diskname;
	int numberofblocks;
	int blocksize;
};
Sdisk::Sdisk(string _diskname, int _numberofblocks, int _blocksize)
{
	diskname = _diskname;
	numberofblocks = _numberofblocks;
	blocksize = _blocksize;

	ifstream infile;

	infile.open(diskname.c_str());

	if (!infile.good())
	{
		cout << "Disk file not found, creating..." << endl;

		ofstream outfile;
		outfile.open(diskname.c_str());

		for (int i = 0; i < numberofblocks * blocksize; ++i)
		{
			outfile << "#";
		}
	}
	else
	{
		cout << "Disk file opened." << endl;
	}
}

int Sdisk::getblock(int blocknumber, string& buffer)
{
	// Find file to read from
	fstream iofile;
	iofile.open(this->diskname.c_str(), ios::in);   // Ready for input

	// Check if file failed to open to read
	if (iofile.fail())
	{
		// For failure
		return 0;
	}

	// Start at blocknumber k starts at k * blocksize
	iofile.seekg(blocknumber * this->blocksize);
	buffer = ""; // char c; 

	// Iterate through entire block to initialize buffer
	for (int i = 0; i < this->blocksize; i++)
	{
		buffer += iofile.get();
	}

	// Close the file after reading it.
	iofile.close();

	// For success
	return 1;

}

int Sdisk::putblock(int blocknumber, string buffer)
{
	fstream iofile;

	iofile.open(diskname.c_str(), ios::in | ios::out);

	if (!iofile.good())
	{
		return 0;
	}

	iofile.seekp(blocknumber*blocksize);

	iofile.write(buffer.c_str(), buffer.length());

	return 1;
}

int Sdisk::getnumberofblocks()
{
	return numberofblocks;
}

int Sdisk::getblocksize()
{
	return blocksize;
}


#endif
