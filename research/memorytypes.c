#include <stdio.h>
#include <inttypes.h>

size_t get_int_sz(int num) {
	return sizeof(num);
}

int check_int_sz(int num) {
	/**
	 * This method checks all possible C-represented integer sizes this
	 * value could possibly be.
	 */
	size_t intsz;
	intsz = get_int_sz(num);
	printf("%zu\n int64:", sizeof(int64_t));
	printf("%zu\n", intsz);
	return 0;
}

int main(void) {
	int64_t n = 16; 
	check_int_sz(n);
	printf("%+"PRId64"\n", n);
}
