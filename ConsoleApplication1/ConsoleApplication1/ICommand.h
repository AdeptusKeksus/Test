#pragma once
#include<string>
#include <vector>
using namespace std;

class ICommand
{

public:
	ICommand(vector<string> query)
	{
		commandQuery = query;
	}

	virtual void Execute() {};

private:
	vector<string>  commandQuery;
};