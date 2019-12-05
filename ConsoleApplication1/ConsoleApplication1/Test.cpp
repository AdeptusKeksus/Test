#include <iostream>
#include "ICommand.h"
#include "CommandMapping.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	string query;
	//query = "/smooth D:\\tets01.txt 2";
	cout << "Введите команду (например, /smooth D:\\test.txt 1500)" << endl;
	getline(cin, query);

	CommandMapping comannd =  CommandMapping(query);
	comannd.objectClass->Execute();
	
	cin.get();
	return 0;
}
