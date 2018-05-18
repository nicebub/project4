/*
 * a sample program #1
 */

int scanf( char *fmt, ... );
int printf( char *fmt, ... );

int
main( void )
{
  int i, j;

  scanf( "%d", &i );                /* get i */
  j = 9 + i * 8;                    /* evaluate j */
  printf( "Result is %d\n", j );    /* print j */
  return(0);
}
