#include <stdio.h>
#include <stdlib.h>

#include "1cc.h"

// vector test
void vec_expect(int line, int expected, int actual) {
	if (expected == actual) {
		return;
	}
	fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
	exit(1);

}

void vec_runtest() {
	Vector *vec = new_vector();
	vec_expect(__LINE__, 0, vec->len);

	for (int i = 0; i < 100; i++) vec_push(vec, (void *)i);

	vec_expect(__LINE__, 100, vec->len);
	vec_expect(__LINE__, 0, (long)vec->data[0]);
	vec_expect(__LINE__, 50, (long)vec->data[50]);
	vec_expect(__LINE__, 99, (long)vec->data[99]);
	printf("OK\n");
}
