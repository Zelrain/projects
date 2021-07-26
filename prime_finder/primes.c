#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
	int bound, i, j, is_prime, root;
	double tmp_root;

	if(argc != 2 
	|| (bound = atoi(argv[1])) < 1){
		fprintf(stderr,"%s expects a single natural number less than 2,147,483,647.\n", argv[0]);
		exit(1);
	}

	for(i = 2; i <= bound; i++){
		is_prime = 1;
		tmp_root = sqrt(i);
		if((double)(root = (int)tmp_root) == tmp_root) continue;
		for(j = 2; j <= root; j++){
			if(i % j == 0){
				is_prime = 0;
				break;
			}
		}
		if(is_prime) printf("%d\n", i);
	}
	return 0;
}
