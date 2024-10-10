#include <stdio.h>
#include <vector>
#include <algorithm>
#define MAX 1000
using namespace std;

int largestPrime(long long n);

int main(){

  long long n = 600851475143;
  printf("%d\n", largestPrime(n));

  return 0;
}

int largestPrime(long long n)
{
  vector<long long> factors;
  long long divisor = 2;

  while(n > 1)
  {
    while(n % divisor == 0)
    {
      factors.push_back(divisor);
      n /= divisor;
    }
    divisor++;
    if((divisor * divisor) > n)
    {
      if(n > 1) factors.push_back(n);
      break;
    }
  }
  long long max = factors[0];
  for(int i = 1 ; i < factors.size() ; i++) if(factors[i] > max) max = factors[i];
  return max;
}
