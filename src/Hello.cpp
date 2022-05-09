#include<iostream>
#include<Hello.h>
#include<lol/lol.h>

int main() {

	int x = 0;
	int y = 20;

	std::cout << "global value :" << global_value << std::endl;
	std::cout << "global value :" << heeber_jeebers << std::endl;
	std::cout << "Hello Apoorv ! You are the coolest !" << std::endl;

	x = y;
	int z = y;

	return 0;
}