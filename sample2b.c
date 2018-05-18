/*
* sample program 2b
*/

int scanf(char *fmt, ...);
int printf(char*fmt, ...);

void count(int n);

int sum;

int main(void){
	int i;
	scanf("%d",&i);
	count(i);
	printf("%d\n",sum);
	return 0;
}

void count(int n){
	int i;
	i = 1;
	sum = 0;
	while(i<=n){
		sum = sum+i;
		i=i+1;
	}
}
