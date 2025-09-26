#include "SimpleServer.h"

using namespace std;

const int SEED = 1;
const double lambda = 3.0;

int main() {

	setlocale(LC_ALL, "RU");

	double mu = 4.0;
	int simTime = 4;

	MultichannelServer server(1, lambda, mu, SEED,simTime);
	server.run();
	server.printStats();
}


