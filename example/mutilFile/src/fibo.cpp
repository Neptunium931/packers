#include "fibo.hpp"

auto
fibo(int n) -> long long
{
  return n < 2 ? n : fibo(n - 1) + fibo(n - 2);
}
