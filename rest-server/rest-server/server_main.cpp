#include <iostream>
#include "MyListener.h"


int main()
{
	//setlocale(LC_ALL, "Russian");
	MyListener listener(L"http://localhost:8080/restdemo");  
	listener.start();

	getchar();
	return 0;
}