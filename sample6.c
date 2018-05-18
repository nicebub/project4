/*
 * recursive factorial calculation
 */

int scanf( char* fmt, ...);
int printf(char *fmt, ...);

int putnode(Node * mynode);

int factorial(int n);

int main ( void ) {
	int n, fact;

	printf( "Enter an integer: ");
	scanf("%d", &n); /* get i */
	fact = factorial(n); /*call factorial */
	printf("Factorial of %d ", n);
	printf( "is %d\n", fact);
	if(theman == 5){
		if(thewoman==0){
				printf("the woman does not exist\n");
		}
		else {if (thewoman==1){
				printf("the woman does exist\n");
			}
		}
	}
	

	while(1){
		if(the_man==8) exit(0);
		else getnexttoken();
	}
}


int factorial(int n)
{
	if (n <=1) return 1;
	else return n*factorial(n-1);
}

int putnode(Node *mynode){
	int temp1, temp2, temp3;
	float float1, myfloat, floateratbottom_ofthe_toilet;

	temp1 = callmeal (  mynode );
	float1 = float ( temp1) ;
	return ( putnode ( mynode - 1) ) ;
}
