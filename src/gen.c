// Author: Aidan Stanford

/* This file contains functions responsible for generating 
 * the Sudoku array
 *
 * We use flattened arrays here
 */

#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// this is just an array we use to keep running totals of rows
// columns and boxes (in that order)
static int *check;
static int conf_dim;

int *sudoku_init(const int dim)
{
	// basically just allocate a pointer to a malloced array
	int *ret = (int *)calloc(dim * dim, sizeof(int));
	check = (int *)calloc(3 * dim, sizeof(int));

	// check pointer
	assert(ret && "ret malloc in sudoku_init failed\n");
	assert(check && "check malloc in sudoku_init failed\n");

	// seed the rng
	srand(time(NULL));

	// set the global variable
	conf_dim = dim;

	return ret;
}

// modify in place
void sudoku_fill(int *s)
{
	const int conf_dim_sq = conf_dim*conf_dim;
	const int conf_dim_sqr = (int)sqrt(conf_dim);
	int temp_ind, temp_sum = 1, row, col, blk;
	bool is_valid = false;

	// check pointer s
	assert(s && "sudoku array not initialized in sudoku_fill\n");

	// this is gonna be stupid bro
	for (int i = 0; i < 9; i++) {
		// get a random array pos until we find a number that works :(
		temp_sum += (i+1);
		for (int j = 0; j < 9; j++) {
			do {
				temp_ind = rand() % conf_dim_sq; // index in original array
				row = temp_ind / conf_dim;
				col = temp_ind % conf_dim;
				// relying on order of operations here
				blk = (col/conf_dim_sqr) + ((row / conf_dim_sqr) * conf_dim_sqr);

				// we do it this way because of how check is organized
				is_valid = (check[row] == temp_sum) ||
						   (check[conf_dim+col] == temp_sum) ||
						   (check[2*conf_dim+blk] == temp_sum);
				// printf("row: %d col: %d blk: %d\n", row, col, blk);
			} while (is_valid);
			s[temp_ind] = i+1;
			check[row] = check[conf_dim+col] = check[2*conf_dim+blk] = temp_sum;
			printf("%d at row %d col %d blk %d\n", i+1, row, col, blk);
		}
	}
}

int main(void)
{
	int *s_arr = sudoku_init(9);
	sudoku_fill(s_arr);

	// print the array out here like this
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (s_arr[(9*i)+j] == 0) {
				printf("  ");
			} else {
			printf("%d ", s_arr[(9*i)+j]);
			}
		}
		printf("\n");
	}

	free(s_arr);
	free(check);

	return 0;
}
