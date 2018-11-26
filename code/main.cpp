//============================================================================
// Name		: main.cpp
// Author	 : Xarvie
// Version	 : 1.2
// Copyright : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include "common.h"
#include "parser.h"
#include <cstring>
#include <algorithm>
#include <errno.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <io.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define SPL "\\"
#define SPLC '\\'
#else
#define SPL "/"
#define SPLC '/'
#endif
using namespace std;

void read_file(const char* file, void **buff, size_t *length)
{
	size_t readLen = 0;
	size_t pos = 0;
	FILE* fp = fopen(file, "rb");
	if (fp == NULL)
		cout << errno << endl;
	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	rewind(fp);

	void* tmpbuf = malloc(fileSize + 1);
	*length = fileSize;
	((char*)tmpbuf)[fileSize] = 0;
	do
	{
		readLen = fread(tmpbuf, 1, fileSize, fp);
		if (readLen > 0)
			pos += readLen;
	} while (pos < fileSize);
	fclose(fp);
	*buff = tmpbuf;
}

int execParse(const char * fileDir)
{
	{

		fileStr = NULL;
		if (fileBuff)
			::free(fileBuff);
		fileBuff = NULL;
		fileStrLen = 0;

		Scanner::pos = 0;
		Scanner::oldCh = ' ';
		Scanner::lineNumber = 0;

		Scanner::linePos = 0;
		Scanner::oldLinePos = 0;
		Scanner::goBack = 0;
		Scanner::sState = NORMAL;

		const char* file = fileDir;
		void *buff = NULL;
		size_t filesize = 0;
		read_file(file, &buff, &filesize);
		fileStr = (char*)buff;
		unsigned char * fileByte = (unsigned char *)buff;
		fileBuff = buff;
		unsigned char bomHEX[] = { 0xEF,0xBB,0xBF };
		size_t i = 0;
		const size_t bomLen = sizeof(bomHEX) / sizeof(*bomHEX);
		for (; i<bomLen && fileByte[i] == bomHEX[i]; i++);
		if (i == bomLen)
			fileStr += bomLen;
		fileStrLen = strlen(fileStr);
	}

	int ret;
	char wch = Scanner::whatCh();
	if (wch != '[' && wch != '{')
		ERRORSTR("只能是数组和对象");
	if ((ret = Parser::parse_value()) == PASSED)
	{

		if (Scanner::getCh() != 0)
		{
			ERRORSTR("解析完root还有其他奇怪的东西");
		}
		//cout << "ok" << fileDir<< endl;
	}
	if (fileBuff)
	{
		::free(fileBuff);
		fileBuff = NULL;
	}
	return ret;
}

extern void ergodicFolder(string folderPath, void(*dirFunc)(string dir, void *param) = NULL, void *fordelParam = NULL, void(*fileFunc)(string file, void *param) = NULL, void *fileParam = NULL);

void s1(string fileName)
{
	cout << "dir:" << fileName << endl;
}

void fileExecParse(string fileName)
{
	const size_t len = fileName.length();
	const char* jstr = "nosj.";
	for (size_t i = 0; i < std::min<size_t>(len, size_t(5)); i++)
	{
		if (fileName[len - i - 1] != jstr[i])
			return;
	}

	if (execParse(fileName.c_str()) != PASSED)
	{
		std::cout << (fileName.c_str()) << "\n\n" << endl;
		Parser::errorNum++;
	}
}

#include <set>

void pushFiles(string fileName, void *param)
{
	if (param)
	{
		std::set<string> * pathSet = (std::set<string>*)param;
		pathSet->insert(fileName);
	}
}

void formatDir(int ac, char*arg[], std::set<string>& pathSet)
{
#ifdef _WIN32
	char * tmpPwd = getcwd(NULL, 256);
	string pwd(tmpPwd);
	::free(tmpPwd);

	for (int i = 0; i<ac; i++)
	{
		DWORD dwAttr = GetFileAttributes(arg[i]);
		if (dwAttr == INVALID_FILE_ATTRIBUTES)
			continue;
		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
		{
			string s = arg[i];
			size_t len = strlen(arg[i]);
			if (len>1 && arg[i][len - 1] == SPLC)
				arg[i][len - 1] = 0;
			ergodicFolder(arg[i], NULL, NULL, pushFiles, &pathSet);
		}
		else
		{
			pathSet.insert(arg[i]);
		}
	}
	if (ac < 2)
	{
		ergodicFolder(pwd, NULL, NULL, pushFiles, &pathSet);
	}
#else
	char * tmpPwd = getcwd(NULL, 100);
	string pwd(tmpPwd);
	free(tmpPwd);
	struct stat st;
	for (int i = 0; i<ac; i++)
	{
		if (0 == lstat(arg[i], &st))
		{
			if (S_ISDIR(st.st_mode))
			{
				string s = arg[i];

				size_t len = strlen(arg[i]);
				if (len>1 && arg[i][len - 1] == SPLC)
					arg[i][len - 1] = 0;
				cout << arg[i] << "  " << len << endl;

				ergodicFolder(arg[i], NULL, NULL, pushFiles, &pathSet);

			}
			else
			{
				pathSet.insert(arg[i]);
			}
		}
	}

	if (ac < 2)
	{
		ergodicFolder(pwd, NULL, NULL, pushFiles, &pathSet);
	}
#endif
}

void ergodicFolder(string folderPath, void(*dirFunc)(string dir, void *param), void *folderParam, void(*fileFunc)(string file, void *param), void *fileParam)
{
#ifdef _WIN32

	_finddata_t fileData;
	string strfind = folderPath + "\\*";

	intptr_t handle = _findfirst(strfind.c_str(), &fileData);
	if (handle == -1L)
	{
		cerr << "文件打开失败" << errno << endl;
		exit(-1);
	}
	do
	{
		if (fileData.attrib & _A_SUBDIR)
		{
			if ((strcmp(fileData.name, ".") != 0) && (strcmp(fileData.name, "..") != 0))
			{
				string newPath = folderPath + "\\" + fileData.name;
				if (dirFunc) dirFunc(newPath, folderParam);
				ergodicFolder(newPath, dirFunc, folderParam, fileFunc, fileParam);
			}
		}
		else
		{
			string filename = (folderPath + "\\" + fileData.name);
			if (fileFunc) fileFunc(filename, fileParam);
		}
	} while (!_findnext(handle, &fileData));
	_findclose(handle);

#else
	DIR *dp;
	struct dirent *entry;
	struct stat statBuf;
	if ((dp = opendir(folderPath.c_str())) == NULL)
	{
		char * tmpPwd = getcwd(NULL, 100);
		string pwd(tmpPwd);
		free(tmpPwd);
		fprintf(stderr, "cannot open directory: %s\n%s\n", folderPath.c_str(), pwd.c_str());
		return;
	}

	while (NULL != (entry = readdir(dp)))
	{
		string tmpdir = folderPath + SPL + entry->d_name;
		if (lstat(tmpdir.c_str(), &statBuf) == 0)
		{
			if (S_ISDIR(statBuf.st_mode))
			{
				if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
					continue;
				if (dirFunc)
					dirFunc(tmpdir, folderParam);
				ergodicFolder(tmpdir, dirFunc, folderParam, fileFunc, fileParam);
			}
			else
			{
				string filename = folderPath + SPL + entry->d_name;
				if (fileFunc)
					fileFunc(filename, fileParam);
			}
		}
	}

	closedir(dp);
#endif
}

int main(int ac, char*av[])
{
	std::set<string>filePathSet;
	formatDir(ac, av, filePathSet);
	string fileOrFolderPath;
	for (std::set<string>::iterator it = filePathSet.begin(); it != filePathSet.end(); it++)
	{
		fileExecParse(*it);
	}
	cout << "eNum:" << Parser::errorNum << endl;
	#ifdef _WIN32
	system("pause");
	#else
	if (Parser::errorNum)
		cout << "Error! Error! Error! Error! Error! Error! Error!\nError! Error! Error! Error! Error! Error! Error!\nError! Error! Error! Error! Error! Error! Error!\nError! Error! Error! Error! Error! Error! Error!\nError! Error! Error! Error! Error! Error! Error!\n" << endl;
	#endif
	
	return 0;
}