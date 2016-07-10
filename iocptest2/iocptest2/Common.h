#pragma once
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <thread>
#include<DirectXMath.h>
#include <mutex>
#include <random>
#include <chrono>
#include <queue>
#include <atomic>
#include <string>
#include <fstream>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")

using namespace std;
using namespace chrono;
using namespace DirectX;
#define CLOCK &CGameTimer::GetInstance()

