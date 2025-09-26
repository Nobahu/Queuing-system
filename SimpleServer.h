#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <numeric>
#include <random>

using namespace std;

class MultichannelServer {
private:
	mt19937_64 gen;
	double _requestRate;
	double _serviceRate;
	double _workTime;

	//Очередь запросов
	queue<int> _request_queue;

	//Информация о канале
	vector<bool> _isServerBusy;
	vector<double> _serverWorkingTime;

	//Векторы данных о запросах
	vector<double> _request_service_times;
	vector<double> _request_arrival_times;

	//Stats
	int _totalRequests = 0;
	int _servedRequests = 0;
	int _rejectedRequests = 0;

public:

	MultichannelServer(int channelAmount,double lambda, double mu, int seed, double workTime) : 
		_requestRate(lambda), 
		_serviceRate(mu),
		_workTime(workTime), 
		gen(seed),
		_serverWorkingTime(channelAmount, 0.0),
		_isServerBusy(channelAmount,false)
	{
		_request_service_times.resize(channelAmount, 0.0);
		_request_arrival_times.resize(channelAmount, 0.0);
	}

	double serviceTimeGenerator();

	void addRequestToServer(double curr_time);

	void deleteRequestFromServer(double curr_time);

	void newRequests(int newRequests, double curr_time);

	void run();

	void printStats();
};