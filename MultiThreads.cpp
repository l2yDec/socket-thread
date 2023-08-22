#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <thread>
#include <functional>
#include "utils.h"
#include "threadFunc.h"
#include "threadpool/ThreadPool2.h"
using namespace std;
#define THREADCNT 1024

int main_poll()
{
    ThreadPool2 tpool(6); //线程池
    string inipath = "my.ini";
    string inputpath = "F:\\VS Project\\MultiThreads\\data\\";
    string outputdir = "out";
    BYTE sErrMsg[2048] = { 0x00 };

    char ctmp[256];
    string serverIP, clientIP;
    int serverPort;
    ::GetPrivateProfileString("socket", "sIP", "null", ctmp, sizeof(ctmp), inipath.c_str());
    serverIP = ctmp;
    ::GetPrivateProfileString("socket", "sPort", "null", ctmp, sizeof(ctmp), inipath.c_str());
    serverPort = atoi(ctmp);
    ::GetPrivateProfileString("socket", "cIP", "null", ctmp, sizeof(ctmp), inipath.c_str());
    clientIP = ctmp;

    vector<string> fileList = TraverseDirectory(inputpath.c_str());
    for (int i = 0; i < fileList.size(); i++) {
        tpool.enqueue(std::bind(&threadFunc, i, fileList[i].c_str(), outputdir.c_str()));//使用线程池
    }
    return 0;
}

int main()
{
    string inipath = "my.ini";
    string inputpath = "F:\\VS Project\\MultiThreads\\data\\";
    string outputdir = "out_notpoll";

    char ctmp[256];
    string serverIP, clientIP;
    int serverPort;
    ::GetPrivateProfileString("socket", "sIP", "null", ctmp, sizeof(ctmp), inipath.c_str());
    serverIP = ctmp;
    ::GetPrivateProfileString("socket", "sPort", "null", ctmp, sizeof(ctmp), inipath.c_str());
    serverPort = atoi(ctmp);
    ::GetPrivateProfileString("socket", "cIP", "null", ctmp, sizeof(ctmp), inipath.c_str());
    clientIP = ctmp;

    vector<string> fileList = TraverseDirectory(inputpath.c_str());
    vector<std::thread> threads;
    for (int i = 0; i < fileList.size(); i++) {
        threads.push_back(std::thread(std::bind(&threadFunc, i, fileList[i].c_str(), outputdir.c_str())));//不使用线程池
    }
    for (auto& i : threads) {
        i.join();
    }
    return 0;
}

int main_test() {
    ThreadPool2 pool(6); // 创建包含4个线程的线程池
    const char* cconfigpath = "const cconfig";
    char inputfile[100] = { '\0' };
    memcpy(inputfile, string("F:\\VS Project\\MultiThreads\\data\\").c_str(), string("F:\\VS Project\\MultiThreads\\data\\").length());
    char* outdir = (char*)(malloc(1000 * sizeof(char)));
    memcpy(outdir, string("out").c_str(), string("out").length());
    string inputpath = "F:\\VS Project\\MultiThreads\\data\\";
    string outputdir = "out";

    for (int i = 0; i < THREADCNT; i++) {
        pool.enqueue(std::bind(threadFunc6, i, inputfile, outdir));
    }
    return 0;
}