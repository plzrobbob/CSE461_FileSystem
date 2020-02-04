#ifndef SHELL_H
#define SHELL_H

#include "Filesys.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


using namespace std;

class Shell : public Filesys
{
public:
	Shell(string filename, int blocksize, int numberofblocks);
	int dir();// lists all files
	int add(string file, string buffer);// add a new file using input from thei
										// keyboard
	int del(string file);// deletes the file
	int type(string file);//lists the contents of file
	int copy(string file1, string file2);//copies file1 to file2
};

Shell::Shell(string filename, int blocksize, int numberofblocks) :Filesys(filename, numberofblocks, blocksize)
{
	cout << "Shell created!" << endl;
}

int Shell::dir()
{
	vector<string> flist = ls();

	for (int i = 0; i < flist.size(); i++)
	{
		cout << flist[i] << endl;
	}

	return 0;
}

int Shell::add(string file, string buffer)
{
	if (!newfile(file))     //newfile() returns 1 if successful, 0 if error
	{
		cout << "Cannot add file" << endl;
		return 0;
	}
	if (buffer != "")
	{
		vector<string> blocks = block(buffer, getblocksize());
		for (int i = 0; i < blocks.size(); ++i)
		{
			if (!addblock(file, blocks[i]))
			{
				cout << "Error! Cannot add data!" << endl;
				return 0;
			}
		}
	}
	return 1;
}

int Shell::del(string file)
{
	int code = getfirstblock(file);

	if (code == -1)
	{
		cout << "No such file exists!" << endl;
		return 0; //error
	}

	int iblock = code;
	while (iblock != 0)
	{
		int next = nextblock(file, iblock);
		delblock(file, iblock);
		iblock = next;
	}

	return rmfile(file); //rmfile() returns 1 if successful, 0 if error
}

//TODO: Rework this file so #'s at end of last block are deleted before
//outputting
int Shell::type(string file)
{
	string buffer;

	int code = getfirstblock(file);
	if (code == -1)
	{
		cout << "No such file to display!" << endl;
		return 0;
	}
	else if (code == 0)
	{
		cout << "Empty file! Cannot display!" << endl;
		return 0;
	}

	int iblock = code;
	while (iblock != 0)
	{
		string b;
		int next = nextblock(file, iblock);
		readblock(file, iblock, b);
		buffer += b;
		iblock = next;
	}

	while (buffer[buffer.size() - 1] == '#')
	{
		buffer.pop_back();
	}

	cout << buffer << endl; //output the buffer
	return 1;
}

int Shell::copy(string file1, string file2)
{
	int code = getfirstblock(file1);
	if (code == -1)
	{
		cout << "Error!" << endl;
		return 0; //error
	}

	int code2 = getfirstblock(file2);
	if (code2 != -1)
	{
		cout << "File 2 already exists!" << endl;
		return 0; //error
	}

	int code3 = newfile(file2); //add new file
	if (code3 == 0)
	{
		cout << "No space!" << endl;
		return 0; //error
	}

	int iblock = code;
	while (iblock != 0)
	{
		string b;

		readblock(file1, iblock, b);

		int code4 = addblock(file2, b);
		if (code4 == -1)
		{
			cout << "No more space!" << endl;
			del(file2);
			return 0;
		}

		iblock = nextblock(file1, iblock);
	}
	return 1;
}

#endif
