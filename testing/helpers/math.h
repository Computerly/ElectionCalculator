#ifndef MATH_H
#define MATH_H

/*static int factorial(int n){
	if(n < 0){ return -1; } // Error
	
	int fact = 1;
	for (int i = 2; i <= n; ++i){ // i starts at 2 for efficiency 
		fact *= i;
	}
	return fact;
}*/

static int opt_int_pow(int n)
{
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

int pow10[10] = {
    1, 10, 100, 1000, 10000, 
    100000, 1000000, 10000000, 100000000, 1000000000
};

static int &quick_pow10(int n)
{
    return pow10[n];
}

#endif
