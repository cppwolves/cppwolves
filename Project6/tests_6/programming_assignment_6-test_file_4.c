// ***************************************************
// * CS460: Programming Assignment 6: Test Program 1 *
// ***************************************************

function int return_math (int n)
{
  int value;

  value = 0;
  value = n*8;

  return value;
}
  
procedure main (void)
{
  int n;
  int sum;

  n = 8;
  sum = return_math (n);
  printf ("sum of the squares of the first %d numbers = %d\n", n, sum);
  printf ("Testing\n", n, sum);
  printf ("New");
  printf ("Old");
}
