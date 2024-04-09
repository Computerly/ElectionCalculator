#ifndef MATH_H
#define MATH_H

/**
 * @brief Misc helper math functions
 * 
 */


static int opt_int_pow(int n){
	int r = 1;
	const int x = 10;
	while (n){
		if (n & 1){
		   r *= x;
		   n--;
		}
		else {
			r *= x * x;
			n -= 2;
		}
	}
	return r; 
}
/**
 * @brief Power lut
 * 
 */
int pow10[10] = {
	1, 10, 100, 1000, 10000, 
	100000, 1000000, 10000000, 100000000, 1000000000
};

/**
 * @brief Looks up power in pow10 array
 * 
 */
static int &quick_pow10(int n){ return pow10[n]; }

#endif
