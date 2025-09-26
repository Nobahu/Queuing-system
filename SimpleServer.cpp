#include "SimpleServer.h"

int poissonGenerator(double lambda, mt19937_64& gen) {
	uniform_real_distribution<double> dist(0.0, 1.0);
	double sum = 0;
	int m = 0;

	while (true) {
		double alpha = dist(gen);
		sum += log(alpha);
		if (sum < -lambda) {
			return m;
		}
		m++;
	}
}

double MultichannelServer::serviceTimeGenerator() {
	uniform_real_distribution<double> dist(0.0, 1.0);
	return -log(dist(gen)) / _serviceRate;
}

void MultichannelServer::newRequests(int newRequests,double curr_time) {
	for (int i = 0; i < newRequests; i++) {
		_totalRequests++;
		_request_queue.push(curr_time);
		cout << "Поступил запрос. Время: " << curr_time << "############################" << '\n';
	}
}

void MultichannelServer::addRequestToServer(double curr_time) {
	for (int i = 0; i < _isServerBusy.size(); i++) {
		if (!_isServerBusy[i] && !_request_queue.empty()) {
			_isServerBusy[i] = true;
			double arrivalTime = _request_queue.front();
			_request_queue.pop();

			double service_time = serviceTimeGenerator();
			_serverWorkingTime[i] = curr_time + service_time;

			_request_service_times[i] = service_time;
			_request_arrival_times[i] = arrivalTime;

			double wait_time = curr_time - arrivalTime;
			cout << "=== НАЧАЛО ОБРАБОТКИ ===================================" << endl;
			cout << "Сервер: " << i << endl;
			cout << "Время поступления: " << arrivalTime << endl;
			cout << "Время начала: " << curr_time << endl;
			cout << "Время ожидания: " << wait_time << " сек" << endl;
			cout << "Время обработки: " << service_time << " сек" << endl;
			cout << "Завершится в: " << _serverWorkingTime[i] << endl;
			cout << "========================================================" << endl;
		}
	}
}

void MultichannelServer::deleteRequestFromServer(double curr_time) {
	for (int i = 0; i < _serverWorkingTime.size(); i++) {
		if (_isServerBusy[i] && _serverWorkingTime[i] <= curr_time) {
			_isServerBusy[i] = false;
			cout << "Сервер " << i << " завершил обработку запроса" << '\n';
			_servedRequests++;
		}
	}
}

void MultichannelServer::run() {
	double current_time = 0;
	double time_step = 1;

	while (current_time < _workTime) {
		deleteRequestFromServer(current_time);

		int newRequest = poissonGenerator(_requestRate * time_step, gen);
		newRequests(newRequest, current_time);

		addRequestToServer(current_time);

		current_time += time_step;
	}
	_rejectedRequests = _request_queue.size();

	for (int i = 0; i < _isServerBusy.size(); i++) {
		if (_isServerBusy[i]) {
			_rejectedRequests++;
			cout << "Сервер " << i << " не успел обработать запрос (время вышло)\n";
		}
	}
}

void MultichannelServer::printStats() {
	cout << "Общее количество запросов: " << _totalRequests << '\n';
	cout << "Количество обработанных запросов: " << _servedRequests << '\n';
	cout << "Количество отклоненных запросов: " << _rejectedRequests << '\n';
}