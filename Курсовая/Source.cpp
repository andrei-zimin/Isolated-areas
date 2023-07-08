#define _CRT_SECURE_NO_WARNINGS //���������� ��� ���������� ������ ������, ��� ���������, ���� �� ��������������
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <regex> //��������� ��������� �� ������ � ������� �������
#include <windows.h>
#include <fstream>
#include <string>
#define maxlen 256 // ������������ ������ �������
#define PATTERN "^[0-1]( [0-1])*$" //������, � ������� �� ���������� ������
using namespace std;

void start_menu();
void menu_open();
void counter(bool** matrix, int m, int n);

int fileexists(char* filename)
{	//��������� ������� ����� filename
	FILE* f = fopen(filename, "r");
	if (f == NULL)
		return 0;
	else
		return 1;
}

bool** new_matrix(int m, int n)
{	//���������� ������ ������� (1,0) m x n
	bool** matrix = NULL;
	matrix = (bool**)malloc(m * sizeof(bool*));
	for (int i = 0; i < m; i++)
		matrix[i] = (bool*)malloc(n * sizeof(bool));
	return matrix;
}

void print_matrix(bool** matrix, int m, int n)
{	//������� �� ����� ������� matrix � ��������� m x n.
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}
int* to_bools(string line)
{	//���������� ������ � ������ (0,1)
	int buf[maxlen]; //�������� ������, ��������� ���������� ��������� ������ ����������
	int n = 1;
	string d;
	stringstream ss(line); // ���������� ������ � �����
	buf[0] = 0;
	while (getline(ss, d, ' '))
		buf[n++] = atof(d.c_str()); //���������� ��������� � �����
	buf[0] = n - 1;
	int* mas = new int[n];
	for (int i = 0; i < n; i++)
		mas[i] = buf[i]; //������������ � ������ �������� �� ������
	return mas;
}

void read_data(char* filename)
{	//������������ ������� �� ����� filename
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
			if (!regex_match(line, regex(PATTERN))) { //���� ������ �� ������������� �������
				printf("%d: ������������ ������\n", m + 1);
				error = true;
			}
			else {
				int* f = to_bools(line);
				n = f[0];
				if (last_n != -1) {
					if (n != last_n) { // ���� ����� ��������� ������ �� ��������� � ������ ��������� � ������ ���������� ������
						printf("%d: ������������ ����� ��������� ������ (%d)\n", m + 1, n);
						error = true;
					}
				}
				else
					last_n = n;
				if (!error) { //�������� ������ � ��������� �������
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
				bool** matrix = new bool* [m]; // ������ m ������ 
				for (i = 0; i < m; i++)
					matrix[i] = buf[i]; // ��������� �� ������
				print_matrix(matrix, m, n); // ��������� �������
				counter(matrix, m, n);
			}
		}
		else {
			cout << "���� ������.\n";
		}
	}
	else {
		cout << "������ ������ �����\n";
	}
	file.close();
	cout << "\n������� ����� ������� ��� ������ �����";
	_getch();
	menu_open();
}

void seeker(bool** matrix, int m, int n, int i, int j)
{	/*
	������� ��������� ���������� i � j.
	���� ��� ������ �������, �� �� ���������� = 1 � ��� �������� ������, ������, �����, ����� ����� ���������� seeker (���� ���� ������� ����������).
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
{	//������������ ����� ������������� �������� �� ��������:���������� ������������ ������ ������� �������. 
	//���� ������� �������, �� ���������� ������� seeker, ������� ���������� �������� � ���������� ��������
	int i, j, count = 0;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			if (!matrix[i][j]) {
				seeker(matrix, m, n, i, j);
				count++;
			}
	printf("\n\n����� ������������� 0-��������: %d\n", count);
}

void menu_make()
{	//���� ������� �������
	system("cls");
	int m = 0, i, n, last_n;
	bool* buf[maxlen];
	string line;
	cout << "������� (0,1)-������� ���������, �������� �������� ����� ���������\n(ENTER-��������� �����)." << endl << endl;
	while (getline(cin, line)) {
		if (line[0] == '\0')
		{
			break;
		}
		if (!regex_match(line, regex(PATTERN))) {
			cout << "^ ������������ ������. ���������� � ����������?\n1. ��(����������)\n������ ��������. ������ ������ \n";
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
			printf("^ ������������ ����� ��������� ������. ���������� � ����������?\n1. ��(����������)\n������ ��������. ������ ������ \n");
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
		cout << "\n������� ����� ������� ��� ������ �����";
		_getch();
	}
	start_menu();
}

void menu_open()
{	//���� �������� ����� ��� ��������� �������
	system("cls");
	char filename[maxlen];
	printf(
		"������� �������� �����(ENTER-��������� �����): "
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
		printf("���� \"%s\" �� ������\n", filename);
		cout << "\n������� ����� ������� ��� ������ �����";
		_getch();
		menu_open();
	}
}

void start_menu()
{	//������� ����
	system("cls");
	printf(
		"�������� ����� ����\n"
		"1. ��������� ������� �� ��������� �����\n"
		"2. ������ ������� �������\n"
		"0. �����\n"
	);

	char choice = _getch();
	while (choice < '0' || choice>'2') {
		printf("\n������� �������, ������� ��� ���.\n");
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
		printf("��������� ��������� ������");
		exit(0);
	}

	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	start_menu();

}
