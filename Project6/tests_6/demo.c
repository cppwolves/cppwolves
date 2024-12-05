// ***************************************************
// * CS460: Programming Assignment 6: Test Program 1 *
// ***************************************************

function int return_math (int n)
{
  int value;

  value = 0;
  value = n*n;

  return value;
}
  
procedure main (void)
{
  int n;
  int square;

  n = 8;
  square = return_math (n);
  printf ("\n%d squared is %d\n\n", n, square);
  printf ("Testing First\n");
  printf ("Second\n");
  printf ("Third\n\n");
}
