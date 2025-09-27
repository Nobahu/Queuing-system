#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <chrono>
#include <numeric>
#include <random>

using namespace std;
using namespace std::chrono;

class MultichannelServer {
private:
	mt19937_64 gen;
	double _requestRate;
	double _serviceRate;
	milliseconds _workTime;

	//Очередь запросов
	queue<milliseconds> _request_queue;

	//Информация о канале
	vector<bool> _isServerBusy;
	vector<milliseconds> _serverWorkingTime;

	//Векторы данных о запросах
	vector<milliseconds> _request_service_times;
	vector<milliseconds> _request_arrival_times;

	//Stats
	int _totalRequests = 0;
	int _servedRequests = 0;
	int _rejectedRequests = 0;

public:

	MultichannelServer(int channelAmount,double lambda, double mu, int seed, milliseconds duration) :
		_requestRate(lambda), 
		_serviceRate(mu),
		_workTime(duration), 
		gen(seed),
		_serverWorkingTime(channelAmount, 0ms),
		_isServerBusy(channelAmount,false)
	{
		_request_service_times.resize(channelAmount, 0ms);
		_request_arrival_times.resize(channelAmount, 0ms);
	}

	milliseconds serviceTimeGenerator();

	void addRequestToServer(milliseconds curr_time);

	void deleteRequestFromServer(milliseconds curr_time);

	void newRequests(int newRequests, milliseconds curr_time);

	void run();

	void printStats();
};