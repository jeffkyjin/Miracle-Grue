/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

**/
#include "abstractable.h"


using namespace mgl;
using namespace std;

#define EZLOGGER_OUTPUT_FILENAME "ezlogger.txt"
#include "ezlogger/ezlogger_headers.hpp"


int FileSystemAbstractor::guarenteeDirectoryExists(const char* pathname, mode_t mode )
{
#ifdef WIN32
	EZLOGGERVLSTREAM(axter::log_often) << "not supported on windows QT"
#else
		int status = 0;

		struct stat st;
		if(stat(pathname,&st) != 0){
			mode_t process_mask = umask(0);
			int result_code = mkdir(pathname, mode);
			umask(process_mask);
			if(result_code != 0)
				status = -1 ; //creation fail
		}
		else if (!S_ISDIR(st.st_mode))
			status = -1;
		return status;
#endif

}

string FileSystemAbstractor::pathJoin(string path, string filename) const
{
#ifdef WIN32
	return path  + "\" + filename;"
#else
	return path  + "/" + filename;
#endif
}

char FileSystemAbstractor::getPathSeparatorCharacter() const
{
	return '/'; // Linux & Mac, works on Windows most times
}

string FileSystemAbstractor::ExtractDirectory(const char *directoryPath) const
{
	const string path(directoryPath);
	return path.substr(0, path.find_last_of(getPathSeparatorCharacter()) + 1);
}

string FileSystemAbstractor::ExtractFilename(const char* filename) const
{
	std::string path(filename);
	return path.substr(path.find_last_of(getPathSeparatorCharacter()) + 1);
}

string FileSystemAbstractor::ChangeExtension(const char* filename, const char* extension) const
{
	const string path(filename);
	const string ext(extension);
	std::string filenameStr = ExtractFilename(path.c_str());
	return ExtractDirectory(path.c_str())
			+ filenameStr.substr(0, filenameStr.find_last_of('.')) + ext;
}

string FileSystemAbstractor::removeExtension(const char *filename) const
{
	const string path(filename);
	string filenameStr = ExtractFilename(path.c_str());
	return ExtractDirectory(path.c_str())
			+ filenameStr.substr(0, filenameStr.find_last_of('.'));
}

ProgressBar::ProgressBar(unsigned int count)
:total(0), delta(0), progress(0), ticks(0)
{
	reset(count);
	EZLOGGERVLSTREAM(axter::log_often) << ":";
}


void ProgressBar::reset(unsigned int count)
{
	ticks=0;
	total = count;
	progress = 0;
	delta = count /10;
}


void ProgressBar::tick()
{
	total --;
	ticks ++;
	if (ticks >= delta)
	{
		ticks = 0;
		progress ++;
		EZLOGGERVLSTREAM(axter::log_often) << " [" << progress * 10<< "%] ";

	}
	if (total ==0)
	{
		// cout << "" << endl;
		string now = myPc.clock.now();
		EZLOGGERVLSTREAM(axter::log_often) << now;
	}
}






