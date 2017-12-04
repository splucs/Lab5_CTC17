#include <bits/stdc++.h>
using namespace std;
#define N 15
#define MAX_EPOCH 1000000000
#define INF 1000000000
#define DEBUG false
#define MAXIT 10

struct Particle {
	int data[N];
	double velocity;
	int fitness;
	Particle() : velocity(0.0), fitness(0) {
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

//Update P and G
void UpdatePG(int S, Particle x[], Particle p[], Particle & g) {
	for(int i = 0; i < S; i++) {
		if (p[i].fitness < x[i].fitness) {
			p[i] = x[i];
		}
		if (g.fitness < p[i].fitness) {
			g = p[i];
		}
	}
}

//Apply speed
void ApplyVelocity(Particle & x) {
	for(int it = 0; it < x.velocity; it++) {
		int i = rand()%N;
		int j = rand()%N;
		swap(x.data[i], x.data[j]);
	}
	x.UpdateFitness();
}

//Aproximates the distance between two particles
inline int Distance(const Particle & p1, const Particle & p2) {
	int dist = 0;
	for(int j = 0; j < N; j++) {
		if (p1.data[j] != p2.data[j]) dist++;
	}
	return dist;
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
Particle PSO(const int S, const double omega, const double C1, const double C2, const double vMax) {

	if (DEBUG) printf("Running PSO with S = %d, omega = %.3f, C1 = %.3F, C2 = %.3f, vMax = %.8f\n", S, omega, C1, C2, vMax);

	//Particles
	Particle x[S], p[S], g;	

	//Initialize
	for(int i = 0; i < S; i++) {
		for(int j = 0; j < N; j++) {
			x[i].data[j] = j;
		}
		x[i].velocity = (rand()%INF)*vMax/INF;
		random_shuffle(x[i].data, x[i].data+N);
		x[i].UpdateFitness();
	}
	UpdatePG(S, x, p, g);
	double phi1 = 1.0;
	double phi2 = 1.0;
	int epoch = 1;

	//Run
	for(; epoch <= MAX_EPOCH && g.fitness < (N*(N-1))/2; epoch++) {
		
		//Update velocity and data
		for(int i = 0; i < S; i++) {
			x[i].velocity = omega*x[i].velocity + C1*phi1*Distance(p[i], x[i]) + C2*phi2*Distance(g, x[i]);
			ApplyVelocity(x[i]);
		}

		//Now update particle best and global best
		UpdatePG(S, x, p, g);
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
	PSO(20, 0.8, 1.494, 1.494, 8);
	double alltime = 0;
	
	for(int S = 20; S<=80; S+=30) {
		for(double omega = 0.4; omega <= 0.8; omega += 0.2) {
			printf("S = %d, omega = %.1f\n", S, omega);
			for(int i = 0; i <= 5; i++) {
				for(int j = 0; j <= 5; j++) {
					if (i == 0 && j == 0) {
						printf("C2 \\ C1 ");
					}
					else if (i == 0) {
						printf("& %.1f ", j*0.2);
					}
					else if (j == 0) {
						printf("%.1f ", i*0.2);
					}
					else {
						clock_t begin = clock();
						for(int it = 0; it < MAXIT; it++) {
							PSO(S, omega, j*0.2, i*0.2, 8);
						}
						clock_t end = clock();
						double t = ((end-begin)*1000.0/CLOCKS_PER_SEC)/MAXIT;
						printf("& %.0f ", t);
						alltime += t;
					}
				}
				printf("\\\\\n");
			}
		}
	}

	printf("avg time = %.3f\n", alltime / 225);
	return 0;
}