#define _CRT_SECURE_NO_WARNINGS //необходимо для исключения выдачи ошибки, как случалось, если не использовалась
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <regex> //проверяет совпадает ли строка с целевой строкой
#include <windows.h>
#include <fstream>
#include <string>
#define maxlen 256 // максимальный размер буферов
#define PATTERN "^[0-1]( [0-1])*$" //шаблон, с которым мы сравниваем строку
using namespace std;

void start_menu();
void menu_open();
void counter(bool** matrix, int m, int n);

int fileexists(char* filename)
{	//Проверяет наличие файла filename
	FILE* f = fopen(filename, "r");
	if (f == NULL)
		return 0;
	else
		return 1;
}

bool** new_matrix(int m, int n)
{	//Возвращает пустую матрицу (1,0) m x n
	bool** matrix = NULL;
	matrix = (bool**)malloc(m * sizeof(bool*));
	for (int i = 0; i < m; i++)
		matrix[i] = (bool*)malloc(n * sizeof(bool));
	return matrix;
}

void print_matrix(bool** matrix, int m, int n)
{	//Выводит на экран матрицу matrix с размерами m x n.
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}
int* to_bools(string line)
{	//Превращает строку в массив (0,1)
	int buf[maxlen]; //буферный массив, поскольку количества элементов строки неизвестно
	int n = 1;
	string d;
	stringstream ss(line); // превращает строку в поток
	buf[0] = 0;
	while (getline(ss, d, ' '))
		buf[n++] = atof(d.c_str()); //превращает подстроку в число
	buf[0] = n - 1;
	int* mas = new int[n];
	for (int i = 0; i < n; i++)
		mas[i] = buf[i]; //переписываем в массив элементы из буфера
	return mas;
}

void read_data(char* filename)
{	//Обрабатывает матрицу из файла filename
	bool* buf[maxlen];
	int m = 0, n = 0, i, last_n = -1;
	ifstream file(filename);
	bool error = false;
	printf("\"%s\"\n\n", filename);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			if (line[0] == '\0')
				break;
			if (!regex_match(line, regex(PATTERN))) { //Если строка не соответствует шаблону
				printf("%d: Некорректная строка\n", m + 1);
				error = true;
			}
			else {
				int* f = to_bools(line);
				n = f[0];
				if (last_n != -1) {
					if (n != last_n) { // Если число элементов строки не совпадает с числом элементов в первой корректной строке
						printf("%d: Некорректное число элементов строки (%d)\n", m + 1, n);
						error = true;
					}
				}
				else
					last_n = n;
				if (!error) { //передаем массив в буфферную матрицу
					bool* bool_line = new bool[n];
					for (i = 0; i < n; i++)
						bool_line[i] = f[i + 1];
					buf[m] = bool_line;
				}
			}
			m++;
		}
		if (m) {
			if (!error) {
				bool** matrix = new bool* [m]; // массив m матриц 
				for (i = 0; i < m; i++)
					matrix[i] = buf[i]; // заполняем из буфера
				print_matrix(matrix, m, n); // печаатаем матрицу
				counter(matrix, m, n);
			}
		}
		else {
			cout << "Файл пустой.\n";
		}
	}
	else {
		cout << "Ошибка чтения файла\n";
	}
	file.close();
	cout << "\nНажмите любую клавишу для выхода назад";
	_getch();
	menu_open();
}

void seeker(bool** matrix, int m, int n, int i, int j)
{	/*
	Функция принимает координаты i и j.
	Если это нулвой элемент, то он становится = 1 и для элемента сверху, справа, снизу, слева также вызывается seeker (если этот элемент существует).
	*/
	if (!matrix[i][j]) {
		matrix[i][j] = 1;
		if (i > 0 && !matrix[i - 1][j])
			seeker(matrix, m, n, i - 1, j);
		else if (i + 1 < m && !matrix[i + 1][j])
			seeker(matrix, m, n, i + 1, j);
		if (j > 0 && !matrix[i][j - 1])
			seeker(matrix, m, n, i, j - 1);
		else if (j + 1 < n && !matrix[i][j + 1])
			seeker(matrix, m, n, i, j + 1);
	}
}

void counter(bool** matrix, int m, int n)
{	//Подсчитывает число изолированных областей по принципу:циклически перебирается каждый элемент матрицы. 
	//Если элемент нулевой, то вызывается функция seeker, которой передается матрицаи и координаты элемента
	int i, j, count = 0;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			if (!matrix[i][j]) {
				seeker(matrix, m, n, i, j);
				count++;
			}
	printf("\n\nЧисло изолированных 0-областей: %d\n", count);
}

void menu_make()
{	//Ввод матрицы вручную
	system("cls");
	int m = 0, i, n, last_n;
	bool* buf[maxlen];
	string line;
	cout << "Введите (0,1)-матрицу построчно, разделяя элементы строк пробелами\n(ENTER-вернуться назад)." << endl << endl;
	while (getline(cin, line)) {
		if (line[0] == '\0')
		{
			break;
		}
		if (!regex_match(line, regex(PATTERN))) {
			cout << "^ Некорректная строка. Продолжить с предыдущей?\n1. Да(продолжить)\nДругие клавииши. Начать заново \n";
			char choice = _getch();
			if (choice == '1')
				continue;
			else {
				menu_make();
				break;
			}
		}
		int* f = to_bools(line);
		n = f[0];
		if (m != 0 && n != last_n) {
			printf("^ Некорректное число элементов строки. Продолжить с предыдущей?\n1. Да(продолжить)\nДругие клавииши. Начать заново \n");
			char choice = _getch();
			if (choice == '1')
				continue;
			else {
				menu_make();
				break;
			}
		}
		else
			last_n = n;
		bool* bool_line = new bool[n];
		for (i = 0; i < n; i++)
			bool_line[i] = f[i + 1];
		buf[m++] = bool_line;
	}
	if (m) {
		bool** matrix = new bool* [m];
		for (i = 0; i < m; i++)
			matrix[i] = buf[i];
		counter(matrix, m, n);
		cout << "\nНажмите любую клавишу для выхода назад";
		_getch();
	}
	start_menu();
}

void menu_open()
{	//Ввод названия файла для прочтения матрицы
	system("cls");
	char filename[maxlen];
	printf(
		"Введите название файла(ENTER-вернуться назад): "
	);
	fgets(filename, maxlen, stdin);
	filename[strlen(filename) - 1] = 0;
	system("cls");
	if (*filename == 0)
		start_menu();
	else if (fileexists(filename)) {
		read_data(filename);
	}
	else {
		printf("Файл \"%s\" не найден\n", filename);
		cout << "\nНажмите любую клавишу для выхода назад";
		_getch();
		menu_open();
	}
}

void start_menu()
{	//Главное меню
	system("cls");
	printf(
		"Выберите пункт меню\n"
		"1. Загрузить матрицу из исходного файла\n"
		"2. Ввести матрицу вручную\n"
		"0. Выход\n"
	);

	char choice = _getch();
	while (choice < '0' || choice>'2') {
		printf("\nНеверно введено, введите ещё раз.\n");
		cout << choice << endl;
		choice = _getch();

	}
	choice = choice - '0';
	switch (choice)
	{
	case 1: {
		menu_open();
		break;
	}
	case 2: {
		menu_make();
		break;
	}
	case 0: {
		printf("Программа завершила работу");
		exit(0);
	}

	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	start_menu();

}
