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

milliseconds MultichannelServer::serviceTimeGenerator() {
	uniform_real_distribution<double> dist(0.0, 1.0);
	double result = -log(dist(gen)) / _serviceRate;
	return duration_cast<milliseconds>(duration<double>(result));
}

void MultichannelServer::newRequests(int newRequests, milliseconds curr_time) {
	if (newRequests <= 0) return;

	for (int i = 0; i < newRequests; i++) {
		_totalRequests++;
		_request_queue.push(curr_time);
		cout << "Поступил запрос. Время: " << curr_time.count() << "ms" << " ############################" << '\n';
	}
}

void MultichannelServer::addRequestToServer(milliseconds curr_time) {
	for (int i = 0; i < _isServerBusy.size(); i++) {
		if (!_isServerBusy[i] && !_request_queue.empty()) {
			_isServerBusy[i] = true;
			milliseconds arrivalTime = _request_queue.front();
			_request_queue.pop();

			milliseconds service_time = serviceTimeGenerator();
			_serverWorkingTime[i] = curr_time + service_time;

			_request_service_times[i] = service_time;
			_request_arrival_times[i] = arrivalTime;

			milliseconds wait_time = curr_time - arrivalTime;
			cout << "=== НАЧАЛО ОБРАБОТКИ ===================================" << '\n';
			cout << "Сервер: " << i << '\n';
			cout << "Время поступления: " << arrivalTime.count() << '\n';
			cout << "Время начала: " << curr_time.count() << '\n';
			cout << "Время ожидания: " << wait_time.count() << " ms" << '\n';
			cout << "Время обработки: " << service_time.count() << " ms" << '\n';
			cout << "Завершится в: " << _serverWorkingTime[i].count() << '\n';
			cout << "========================================================" << '\n';
			cout << '\n';
		}
	}
}

void MultichannelServer::deleteRequestFromServer(milliseconds curr_time) {
	for (int i = 0; i < _serverWorkingTime.size(); i++) {
		if (_isServerBusy[i] && _serverWorkingTime[i] <= curr_time) {
			_isServerBusy[i] = false;
			cout << "Сервер " << i << " завершил обработку запроса" << '\n';
			_servedRequests++;
		}
	}
}

void MultichannelServer::run() {
	auto current_time = 0ms;
	auto time_step = 5ms;
	while (current_time < _workTime) {
		deleteRequestFromServer(current_time);

		int newRequest = poissonGenerator(_requestRate, gen);
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
	cout << '\n';
	cout << "#############################################" << '\n';
	cout << "Общее количество запросов: " << _totalRequests << '\n';
	cout << "Количество обработанных запросов: " << _servedRequests << '\n';
	cout << "Количество отклоненных запросов: " << _rejectedRequests << '\n';
	if (_totalRequests > 0) {
		cout << "Эффективность сервера: " << (double)_servedRequests / _totalRequests << '\n';
	}
	cout << "#############################################" << '\n';
}