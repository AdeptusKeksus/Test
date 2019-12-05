#pragma once
#include <iostream>
#include <map> 
#include <functional>
#include "ICommand.h"
#include "SmoothCommand.h"
#include <vector>

class CommandMapping
{

public:
	ICommand* objectClass;
	CommandMapping(string query)
	{
		splitString(query);
		getRequestName();
		getClass();
	}

private:
	vector<string> queryWordList;
	string commandName;

	map<string, function<ICommand* ()> > factory =
	{
		{"/smooth",[&]() { return new SmoothCommand(queryWordList);}}
	};

	void splitString(string query)
	{
		int i = 0;
		char space = ' ';
		string tempString = "";

		while (i < query.size() + 1)
		{
			if ((query[i] == space) || (i == query.size()))
			{
				queryWordList.push_back(tempString);
				tempString = "";
			}
			else
				tempString += query[i];
			i++;
		}
	}

	void getRequestName()
	{
		commandName = queryWordList[0];

		queryWordList.erase(queryWordList.begin());
		vector<string>(queryWordList).swap(queryWordList);	
	}

	void getClass()
	{
		try
		{
			objectClass = factory[commandName]();
		}
		catch (bad_function_call& e)
		{
			cout << "Неверное имя функции" << endl;
			cin.get();
			exit(0);
		}	
	}
};

