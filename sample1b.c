/*
* a sample program #1b *(floating point arithmetic)
*/

int scanf(char *fmt, ...);
int printf(char *fmt, ...);

int main (void){
	float x, z;
	int y;

	scanf("%f",&x);
	z = 9 + x * 8;
	y = 9 + x * 8;
	printf("Result (int) is %d\n",y);
	printf("Result (float) is %f\n",z);
	return 0;
}

