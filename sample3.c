/*
 * recursive factorial calculation
 */

int scanf( char* fmt, ...);
int printf(char *fmt, ...);

int factorial(int n);

int main ( void ) {
	int n, fact;

	printf( "Enter an integer: ");
	scanf("%d", &n); /* get i */
	fact = factorial(n); /*call factorial */
	printf("Factorial of %d ", n);
	printf( "is %d\n", fact);
	return 0;
}

int factorial(int n)
{
	if (n <=1) return 1;
	else return n*factorial(n-1);
}

