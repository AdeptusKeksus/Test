#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <string>
#include "ICommand.h"
using namespace std;

class SmoothCommand: public ICommand
{
	int CountCPUCores = 4;
	vector<string>  commandQuery;
	vector<vector<float>> threadValueVector;
	vector<float> PrimaryArray;
	string filePath;
	int window;

	int Max(int value1, int value2)
	{
		return (value1 > value2) ? value1 : value2;
	}

	int Min(int value1, int value2)
	{
		return (value1 < value2) ? value1 : value2;
	}

	int LastArgument(int i)
	{
		return (i == CountCPUCores - 1) ? PrimaryArray.size() : (i + 1) * (PrimaryArray.size() / CountCPUCores);
	}

	void ThreadExecute(int fisrtIndex, int index)
	{
		for (int i = fisrtIndex; i < LastArgument(index); i++)
		{
			float tempSum = 0;
			int firstIndex = Max(0, (i - window));
			int lastIndex = Min(PrimaryArray.size(), (i + window + 1));

			for (int j = firstIndex; j < lastIndex; j++)
				tempSum += PrimaryArray[j];

			tempSum /= (lastIndex - firstIndex);
			threadValueVector[index].push_back(tempSum);
		}

	}

	void ExceptionMessage(string message)
	 {
		 cout << message << ". Нажмите на любую клавишу" << endl;
		 cin.get();
		 exit(0);
	 }
	
	string CreateNewFileName(string extraWord)
	 {
		 int lastSlashIndex = filePath.find_last_of("\\") + 1;
		 string firstPartPath = filePath.substr(0, lastSlashIndex);

		 int lastPointIndex = filePath.find_last_of(".");
		 string shortFileName = filePath.substr(lastSlashIndex, lastPointIndex - lastSlashIndex);

		 string lastPartPath = filePath.substr(lastPointIndex, filePath.size() - lastPointIndex);

		 return  firstPartPath + shortFileName + extraWord + lastPartPath;
	 }
	
	vector<float> GetData()
	{
		vector<float> tempVector;
		string line;


		try
		{
			ifstream in(filePath.c_str());
			if(!in.is_open())
				ExceptionMessage("Файл не найден");
			while (getline(in, line))
				tempVector.push_back(stof(line.c_str()));
			in.close();
		}
		catch (...) 
		{
			ExceptionMessage("Ошибка данных файла");
		}

		return tempVector;
	}

	void SetData(vector<float> AvarageValuesList)
	 {
		 string newFileName = CreateNewFileName("_recounted");

		 ofstream out;          
		 out.open(newFileName); 
		 if (out.is_open())
			 for (int i = 0; i < AvarageValuesList.size(); i++)
				 out << AvarageValuesList[i] << endl;

		 cout << "Рассчёт произведен успешно. Данные записаны в файл: " << newFileName << endl;
	 }
	 
public:

	SmoothCommand(vector<string> query)
		: ICommand(query)
	{
		try
		{
			filePath = query[0];
			window = stoi(query[1]);
		}
		catch (invalid_argument& e)
		{
			ExceptionMessage("Неверно введена команда");
		}		
		threadValueVector = vector<vector<float>>(CountCPUCores);

		if (window < 0)
			ExceptionMessage("Окно имеет отрицатильное значение");
		
	}
	
	 void Execute()
	{	
		 vector<thread> threadList(CountCPUCores);
		 vector<float> AvarageValuesList;
		 PrimaryArray = GetData();

		 for (int i = 0; i < CountCPUCores;i++)			
				threadList[i] = thread([=] {ThreadExecute(i * (PrimaryArray.size() / CountCPUCores), i); });			
		 for (int i = 0; i < CountCPUCores;i++)
			 threadList[i].join();	
		threadList.clear();

		for (int i = 0; i < CountCPUCores;i++)
			AvarageValuesList.insert(AvarageValuesList.end(), threadValueVector[i].begin(), threadValueVector[i].end());

		SetData(AvarageValuesList);
	}
};

