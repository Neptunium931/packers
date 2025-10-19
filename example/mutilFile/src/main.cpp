#include "fibo.hpp"
#include <iostream>

auto
main() -> int
{
  std::cout << "Fibonacci: 10 " << fibo(10) << "\n";
  std::cout << "Fibonacci: 1 " << fibo(1) << "\n";
  std::cout << "Fibonacci: 2 " << fibo(2) << "\n";
  std::cout << "Fibonacci: 0 " << fibo(0) << "\n";
  return 0;
}
