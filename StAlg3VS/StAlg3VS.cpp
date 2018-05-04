#define NOMINMAX
//used libraries
#include "stdafx.h"
#include <fstream>//file stream
#include <iostream>//input, output stream
#include <conio.h>//getch
#include <iomanip>//string comparison
#include <windows.h>//localization
#include <stdlib.h>//random
#include <time.h>//time
#include <string>//strings
#include <algorithm>

//functions prototypes
void outputMainMenu();//menu and info
void aboutProgramme();//about programme
int solutionBruteForce(int *dimensions,int* actions, int *answer,int quantity);
int solutionDynamicProgramming(int *dimensions, int* actions, int *answer, int quantity);
void writeMatrix(int** Table, int i, int j, int &counter);

void permutations(int* dimensions, int* actions, int* answer, int quantity, int &complexity, int pos = 0)
{
	if (pos >= quantity-2)
	{
		int tempcomplexity = 0;
		int *tempdimensions = new int[quantity];
		for (int i = 0; i < quantity; i++)
			tempdimensions[i] = dimensions[i];
		for (int i = 0; i < quantity - 2; i++)
		{
			int prev = actions[i] - 1;
			int next = actions[i] +1;
			while (tempdimensions[prev] == -1)
				prev--;
			while (tempdimensions[next] == -1)
				next++;
			tempcomplexity = tempcomplexity + tempdimensions[prev] * tempdimensions[actions[i]] * tempdimensions[next];
			tempdimensions[actions[i]] = -1;
		}
		delete[]tempdimensions;
		if (complexity<0 || complexity>tempcomplexity)
		{
			complexity = tempcomplexity;
			for (int i = 0; i < quantity-2; i++)
				answer[i] = actions[i];
		}
		return ;
	}
	else
	{
		for (int i = pos; i < quantity-2; ++i)
		{
			std::swap(actions[i], actions[pos]);
			permutations(dimensions, actions, answer, quantity, complexity, pos + 1);
			std::swap(actions[i], actions[pos]);
		}
	}
}

const int TESTS_NUMBER = 33;//number of units tests

void main()//main function
{
	short operationCode = 0, passedTests = 0;
	int quantity = 0;
	int *dimensions = new int[0];
	int *actions = new int[0];
	int *answer = new int[0];
	int complexity=0;
	time_t start, finish;
	double timePassed;
	srand(time(NULL));//initialization of random seed
	SetConsoleOutputCP(1251);//console localization
	SetConsoleCP(1251);
	outputMainMenu();//show menu of available commands
	while (operationCode != 48)//repeat until exit
	{
		operationCode = _getch();//get command
		system("cls");//clear console screen
		switch (operationCode)//command choice
		{
		case 49://matrices quantity set
			std::cout << "SET QUANTITY OF MATRICES\n";
			std::cout << "Enter quantity value:\n";//value input
			std::cin >> quantity;
			delete[] dimensions;//array of matrices dimensions recreation
			dimensions = new int[quantity];
			actions = new int[quantity-2];
			answer = new int[quantity - 2];
			std::cout << "Done!\n";
			break;
		case 50://matrices dimensions set
			std::cout << "SET DIMENSIONS OF MATRICES\n";
			std::cout << "Enter dimensions values:\n";
			for (int i = 0; i < quantity; i++) {//dimensions values input
				std::cin >> dimensions[i];
			}
			std::cout << "Done!\n";
			break;
		case 51://matrices quantity generation
			std::cout << "GENERATE QUANTITY OF MATRICES\n";
			quantity = std::rand() % 20 + 1;//random values from 1 to 20 generator
			delete[] dimensions;//array of matrices dimensions recreation
			dimensions = new int[quantity];
			actions = new int[quantity-2];
			answer = new int[quantity - 2];
			std::cout << "Generated value: " << quantity << std::endl;//result output
			std::cout << "Done!\n";
			break;
		case 52://matrices dimensions generation
			std::cout << "GENERATE DIMENSIONS OF MATRICES\n";
			std::cout << "Generated values: ";
			for (int i = 0; i < quantity; i++) {//dimensions values input
				dimensions[i]= std::rand() % 100 + 1;//random values from 1 to 100 generator
				std::cout << dimensions[i] << " ";
			}
			std::cout << "Done!\n";
			break;
		case 53://brute force solution search
			std::cout << "SEARCH SOLUTION BY BRUTE FORCE\n";
			start = clock();//начало сортировки
			complexity = solutionBruteForce(dimensions,actions,answer, quantity);
			finish = clock();//завершение сортировки
			timePassed = (finish - start) / CLOCKS_PER_SEC;//вычисление времени сортировки
			for (int i = 0;i < quantity-2; i++)
				std::cout << answer[i] << " ";
			std::cout << "\nComplexity: " << complexity ;
			std::cout << "\nTime passed: " << timePassed<<"s\nTicks: ";
			std::cout << finish - start << "\n";
			std::cout << "Done!\n";
			break;
		case 54://dynamic programming solution search
			std::cout << "SEARCH SOLUTION BY DYNAMIC PROGRAMMING\n";
			start = clock();//начало сортировки
			complexity = solutionDynamicProgramming(dimensions, actions, answer, quantity);
			finish = clock();//завершение сортировки
			timePassed = (finish - start) / CLOCKS_PER_SEC;//вычисление времени сортировки
			std::cout << "\nComplexity: " << complexity;
			std::cout << "\nTime passed: " << timePassed << "s\nTicks: ";
			std::cout << finish - start << "\n";
			std::cout << "Done!\n";
			break;
		case 57://show info about programme
			aboutProgramme();
			break;
		case 48://programme shut down
			exit;
			break;
		default://case of pressing button not assigned to commands
			outputMainMenu();//show menu of available commands
		}
	}
}

int solutionBruteForce(int* dimensions, int *actions, int *answer, int quantity) {
	int complexity=-1;
	actions = new int[quantity-2];
	for (int i = 0; i < quantity-2; i++)
		actions[i] = i+1;
	permutations(dimensions,actions , answer, quantity,complexity);


	return complexity;
}

int solutionDynamicProgramming(int* dimensions, int *actions, int *answer, int quantity) {
	int n = quantity-1;

	int** dp = new int*[n+1];
	for (int i = 0; i < n+1; ++i)
		dp[i] = new int[n+1];

	for (int i = 1; i <= n; i++) {
		dp[i][i] = 0;
	}

	int** s = new int*[n + 1];
	for (int i = 0; i < n + 1; ++i)
		s[i] = new int[n + 1];

	for (int i = 1; i <= n; i++) {
		s[i][i] = 0;
	}

	for (int l = 2; l <= n; l++) {
		for (int i = 1; i <= n - l + 1; i++) {
			int j = i + l - 1;
			dp[i][j] = INT_MAX;
			for (int k = i; k <= j - 1; k++) {
				if (dp[i][k] + dp[k + 1][j] + dimensions[i - 1] * dimensions[k] * dimensions[j] < dp[i][j])
					s[i][j] = k;
				dp[i][j] = (std::min)(dp[i][j], dp[i][k] + dp[k + 1][j] + dimensions[i - 1] * dimensions[k] * dimensions[j]);
				
			}
		}
	}
	int counter = 0;
	writeMatrix(s, 1, n,counter);
	return dp[1][n];
}

void writeMatrix(int** Table, int i, int j, int &counter)
{

	if (i == j)
	{
		std::cout << "A";
		std::cout << i;
	}
	else
	{
		std::cout << "(";
		int k = Table[i][j];
		writeMatrix(Table, i, k,counter);
		writeMatrix(Table, k + 1, j,counter);
		std::cout << ")";
	}
}

void outputMainMenu()//show menu of available commands in main menu
{
	std::cout << "MAIN MENU\n";
	std::cout << "Choose operation:\n";
	std::cout << "Press 1 to set quantity of matrices\n";
	std::cout << "Press 2 to set matrices dimensions\n";
	std::cout << "Press 3 to generate quantity of matrices\n";
	std::cout << "Press 4 to generate matrices dimensions\n";
	std::cout << "Press 5 to find the best solution by brute force\n";
	std::cout << "Press 6 to find the best solution using dynamic programming\n";
	std::cout << "Press 9 to get info about the programme\n";
	std::cout << "Press 0 to exit";

}

void aboutProgramme()//show info about the programme
{
	std::cout << "ABOUT PROGRAMME\n\n";
	std::cout << "Algorithms and data structures\n";
	std::cout << "Practical assignment #3\n";
	std::cout << "Variant: Matrix chain multiplication\n\n";
	std::cout << "Version 0.0.0.0(vs) build 20180407200000\n\n";
	std::cout << "Development and testing: Yaskovich Dmitry (ISBO-05-15)\n\n";
	std::cout << "Dimini Inc, 2018";
}