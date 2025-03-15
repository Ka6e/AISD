#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;


/*Compiler version 19.41.34120 for x86
* Visual Studio 2022
* �������� ���� ��-23
 20. ������ ����������  �����  ��������  �������  ���������.
����������   ����������   �����   �����, �������  �  ��������
������������� ������  ��  ��������  �������.��������  ������
�������  � �������� ����� ��������� '*', �� ����������� �����
�����(8).*/



int stringLenght(const string& str)
{
	int length = 0;

	for (auto& i : str)
	{
		length++;

	}

	return length;
}

wchar_t toLower(wchar_t& ch)
{
	if (ch >= L'A' && ch <= L'Z')
	{
		return ch + (L'a' - L'A');
	}
	if (ch >= L'�' && ch <= L'�')
	{
		return ch + (L'�' - L'�');
	}
	if (ch == L'�')
	{
		return L'�';
	}
	return ch;
}

int compareString(const string& str1, const string& str2)
{
	int length1 = stringLenght(str1);
	int length2 = stringLenght(str2);
	int minLength = min(length1, length2);

	for (size_t i = 0; i < minLength; i++)
	{
		char ch1 = str1[i];
		char ch2 = str2[i];

		if (ch1 == '�') ch1 = '�';
		if (ch2 == '�') ch2 = '�';

		if (ch1 < ch2) return -1;
		if (ch1 > ch2) return 1;
	}

	if (length1 < length2) return -1;
	if (length2 > length1) return 1;

	return 0;
}

void replaceWord(fstream& file, streampos pos, size_t length)
{
	file.seekp(pos);
	for (size_t i = 0; i < length; i++)
	{
		file.put('*');
	}
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "ru");

	string inputFileName = "studentList.txt";
	if (argc == 2)
	{
		inputFileName = argv[1];
	}
	else
	{
		cout << "������� ��� �����: ";
		cin >> inputFileName;
	}

	fstream file(inputFileName, ios::in | ios::out | ios::binary);
	if (!file.is_open())
	{
		cerr << "������ �������� �����!" << endl;
		return 1;
	}

	string currentSurname, firstSurname;
	streampos firstSurnamePos = -1;
	streampos currentPos = 0;

	wchar_t ch;

	while (!file.eof())
	{
		ch = file.get();
		if (iswalpha(ch))
		{
			currentSurname += toLower(ch);
		}
		else
		{
			if (stringLenght(currentSurname) > 0)
			{
				if (stringLenght(firstSurname) == 0 || compareString(currentSurname, firstSurname) < 0)
				{
					firstSurname = currentSurname;
					firstSurnamePos = currentPos;
				}

				currentSurname = "";
			}

			currentPos = file.tellg();
		}
	}

	if (firstSurnamePos != -1)
	{
		file.clear();
		replaceWord(file, firstSurnamePos, stringLenght(firstSurname));
	}
	else
	{
		cout << "������� �� �������" << endl;
	}

	file.close();
	return 0;
}



