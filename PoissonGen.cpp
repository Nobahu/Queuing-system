#include "SimpleServer.h"

using namespace std;

const int SEED = 245;
const double lambda = 3.0;

int main() {

	setlocale(LC_ALL, "RU");

	double mu = 100.0;
	milliseconds workTime = 1000ms;

	MultichannelServer server(20, lambda, mu, SEED, workTime);
	server.run();
	server.printStats();
}


