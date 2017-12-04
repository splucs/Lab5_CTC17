#include <bits/stdc++.h>
using namespace std;
#define N 15
#define MAX_EPOCH 1000000000
#define INF 1000000000
#define DEBUG false
#define MAXIT 50

struct Particle {
	int data[N];
	int fitness;
	Particle() : fitness(0) {
		for(int i = 0; i < N; i++) data[i] = 0;
	}
	void UpdateFitness() {
		fitness = (N*(N-1))/2;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < i; j++) {
				if (data[i] == data[j] || i+data[i] == j+data[j] || i-data[i] == j-data[j]) {
					fitness--;
				}
			}
		}
	}
};

//Get the trial vector for particle x
Particle GetTrialVector(const double CR, const Particle & x) {
	Particle u = x;
	for(int i = 0; i < N; i++) {
		int j = rand()%N;
		if ((rand()%INF)*1.0/INF < CR) swap(u.data[i], u.data[j]);
	}
	u.UpdateFitness();
	return u;
}

void PrintField(const Particle & x) {
	printf("Fitness: %d\n", x.fitness);
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if (x.data[i] != j) printf(".");
			else printf("Q");
		}
		printf("\n");
	}
}

//Run algorithm
Particle DE(const int S, const double CR) {

	if (DEBUG) printf("Running DE with S = %d, CR = %.3f\n", S, CR);

	//Particles
	Particle x[S], u[S], g;	

	//Initialize
	for(int i = 0; i < S; i++) {
		for(int j = 0; j < N; j++) {
			x[i].data[j] = j;
		}
		random_shuffle(x[i].data, x[i].data+N);
		x[i].UpdateFitness();
	}
	int epoch = 1;

	//Run
	for(; epoch <= MAX_EPOCH && g.fitness < (N*(N-1))/2; epoch++) {
		
		//Generate donor vectors
		for(int i = 0; i < S; i++) {
			u[i] = GetTrialVector(CR, x[i]);
		}

		//Update and get most fit particle
		g = x[0];
		for(int i = 0; i < S; i++) {
			if (u[i].fitness > x[i].fitness) x[i] = u[i];
			if (g.fitness < x[i].fitness) g = x[i];
		}
	}

	//Print result
	if (DEBUG) printf("epoch = %d\n", epoch);
	if (DEBUG) PrintField(g);
	if (g.fitness < (N*(N-1))/2) printf("fail\n");

	//Return global best result
	return g;
}

int main() {
	srand(time(NULL));
	DE(20, 0.2);
	double alltime = 0;
	
	for(int S = 0; S<=80; S+=20) {
		for(double CR = 0.0; CR <= 1.0; CR += 0.1) {
			if (S == 0 && fabs(CR) < 1e-9) {
				printf("S \\ CR ");
			}
			else if (S == 0) {
				printf("& %.1f ", CR);
			}
			else if (fabs(CR) < 1e-9) {
				printf("%d ", S);
			}
			else {
				clock_t begin = clock();
				for(int it = 0; it < MAXIT; it++) {
					DE(S, CR);
				}
				clock_t end = clock();
				double t = ((end-begin)*1000.0/CLOCKS_PER_SEC)/MAXIT;
				printf("& %.0f ", t);
				alltime += t;
			}
		}
		printf("\\\\\n");
	}

	printf("avg time = %.3f\n", alltime / 225);
	return 0;
}