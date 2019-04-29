#include "prime.h"

int is_prime(const int x) {
	if (x < 2) return 0;
	if (x < 4) return 1;
	if (x % 2 == 0) return 0;
	int i = 3;
	while (i * i <= x) {
		if (x % i == 0) return 0;
		i = i + 2;
	}
	return 1;
}

int next_prime(int x) {
	while (!is_prime(x)) x++;
	return x;
}
