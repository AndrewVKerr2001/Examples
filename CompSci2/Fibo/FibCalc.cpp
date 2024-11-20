#include <iostream>

long stored[200] = {0};

long fibo(int n)
{
	if(stored[n] != 0) return stored[n];
	if(n <= 2)
		return 1;
	long calc = fibo(n-1) + fibo(n-2);
	stored[n] = calc;
	return calc;
}

int main(int argc, char* argv[])
{
	std::cout << "What Fibonacci number would you like?: ";
	int num;
	std::cin >> num;
	std::cout << "Result is: " << fibo(num) << std::endl;
	return 0;
}
