#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
#include <functional>
#include <io.h>
#include <direct.h>
#include "utils.h"

#define IMAXSIZE 10

unsigned int threadFunc(unsigned int id, const char* inputFileName, const char* outputpath) {
	int fsize = getFileSize(inputFileName);
	int readCnt = fsize / IMAXSIZE;
	int readLeft = fsize % IMAXSIZE;
	if (_access(outputpath, 0) == -1)	//如果文件夹不存在
		//则创建
		if (! _mkdir(outputpath)) {
			return -1;
		}
	std::string outFileName = outputpath + std::string("\\") + GetPathOrURLShortName(inputFileName);
	FILE* fpin = fopen(inputFileName, "rb");
	FILE* fpout = fopen(outFileName.c_str(), "wb");
	for (int i = 0; i < readCnt; i++) {
		BYTE buffer[IMAXSIZE];
		memset(buffer, '\0', IMAXSIZE);
		fread(buffer, 1, IMAXSIZE, fpin);
		fwrite(buffer, 1, IMAXSIZE, fpout);
	}
	if (readLeft > 0) {
		BYTE buffer[IMAXSIZE];
		memset(buffer, '\0', IMAXSIZE);
		fread(buffer, 1, readLeft, fpin);
		fwrite(buffer, 1, readLeft, fpout);
	}
	fclose(fpin);
	fclose(fpout);
	return id;
}

unsigned int threadFunc6(unsigned int id, char* inputFileName, char* outputpath) {
	std::cout << "子线程" << id << "接收的inputfile:" << inputFileName << std::endl;
	std::cout << "子线程" << id << "接收的outputdir:" << outputpath << std::endl;

	return id;
}