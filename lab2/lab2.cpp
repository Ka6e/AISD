#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>


using namespace std;

//Compiler Version 19.41.34120 for x86
//Visual Studio 2022
//Васильев Иван ПС-23
//В   некотором   компиляторе   ПАСКАЛя  текст  программы
//включает примечания, выделенные  фигурными  скобками  '{', '}'
//либо  парами  символов  '(*'  и  '*)'.Примечания  могут быть
//вложенными друг в друга.Если примечание открыто знаком  '{',
//то оно должно быть закрыто знаком '}'.Аналогично примечание,
//начинающееся с символов '(*'  должно  заканчиваться  символами
//'*)'.Требуется:
//1) проверить правильность вложенности примечаний;
//2) переписать   файл   с   исходным   текстом   так, чтобы
//отсутствовала  вложенность  комментариев  при  сохранении   их
//содержания  и  в  качестве  ограничивающих  символов  остались
//только  фигурные  скобки.Учесть   случай, когда   символы
//примечаний находятся в апострофах.При некорректности  указать
//номера строки и позиции первой ошибки(10).


class Stack
{
public:
	Stack()
	{
		topNode = nullptr;
		count = 0;
	}

	void push(const char& ch)
	{
		Node* newNode = new Node;
		newNode->data = ch;
		newNode->next = topNode;
		topNode = newNode;
		count++;
	}

	void pop()
	{
		if (topNode)
		{
			Node* temp = topNode;
			topNode = topNode->next;
			delete temp;
			count--;
		}
		else
		{
			cerr << "Stack is empty";
		}
	}

	char top()
	{
		if (topNode)
		{
			return topNode->data;
		}
		else
		{
			cerr << "Stack is empty";
			return '\0';
		}
	}

	void clear()
	{
		while (topNode != nullptr)
		{
			Node* temp = topNode;
			topNode = topNode->next;
			delete temp;
		}
	}

	int size()
	{
		return count;
	}

	bool isEmpty()
	{
		return topNode == nullptr;
	}

	~Stack()
	{
		while (topNode)
		{
			pop();
		}
	}

private:
	struct Node
	{
		char data;
		Node* next;
	};

	Node* topNode;
	int count;
};

void ChangeBrackets(fstream& inputFile, fstream& outFile)
{
	inputFile.clear();
	inputFile.seekg(0);
	string result;
	char ch;
	Stack stack;
	bool inString = false;

	while (inputFile.get(ch))
	{
		if (ch == '\'')
		{
			inString = !inString;
			result += ch;
		}
		else if (!inString)
		{
			if (ch == '{')
			{
				if (stack.isEmpty())
				{
					stack.push('{');
					result += '{';
				}
				else
				{
					stack.push('{');
				}
			}
			else if (ch == '(' && inputFile.peek() == '*')
			{
				inputFile.get();
				if (stack.isEmpty())
				{
					result += '{';
				}
				stack.push('(');
			}
			else if (ch == '*' && inputFile.peek() == ')')
			{
				inputFile.get();
				if (!stack.isEmpty() && stack.top() == '(')
				{
					stack.pop();
					if (stack.isEmpty())
					{
						result += '}';
					}
				}
			}
			else if (ch == '}')
			{
				if (!stack.isEmpty() && stack.top() == '{')
				{
					if (stack.size() > 1)
					{
						stack.pop();
					}
					else
					{
						stack.pop();
						result += '}';
					}
				}
			}
			else
			{
				result += ch;
			}
		}
		else
		{
			result += ch;
		}
	}
	outFile << result;

}

bool CheckForNesting(fstream& file, int& pos, int& lineCount)
{
	Stack stack;
	char ch;
	int line = 1;
	bool openBracket = false;
	bool inString = false;

	while (file.get(ch))
	{
		if (ch == '\'')
		{
			inString = !inString;
			pos++;
		}
		if (!inString)
		{
			pos++;
			switch (ch)
			{
			case '{':
				stack.push('{');
				if (!openBracket)
				{
					lineCount = line;
					openBracket = true;
				}
				break;
			case '(':
				if (file.peek() == '*')
				{
					file.get();
					stack.push('(');
					stack.push('*');
					if (!openBracket)
					{
						lineCount = line;
						openBracket = true;
					}
				}
				break;
			case '}':
				if (!stack.isEmpty() && stack.top() == '{')
				{
					stack.pop();
					openBracket = false;
				}
				else
				{
					lineCount = line;
					return false;
				}
				break;
			case '*':
				if (!stack.isEmpty() && stack.top() == '*' && file.peek() == ')')
				{
					file.get();
					stack.pop();
					if (!stack.isEmpty() && stack.top() == '(')
					{
						stack.pop();
						openBracket = false;;
					}
					else
					{
						lineCount = line;
						return false;
					}
				}
				else
				{
					lineCount = line;
					return false;
				}
				break;
			case '\n':
				pos = 0;
				line++;
				break;
			default:

				break;
			}
		}
	}
	return stack.isEmpty();
}

void errorMessage(fstream& file, int& stringLine, int& pos)
{
	file.clear();
	file.seekg(0);
	string line;
	int currentLine = 1;
	while (getline(file, line)) {
		if (currentLine == stringLine) {
			cerr << "Error: Incorrect nesting at line " << stringLine << " position " << pos << "\n";
			break;
		}
		currentLine++;
	}
}

void processFile(const string& inputFilename, const string& outFilename)
{
	fstream inputFile;
	fstream outFile;
	int position = 0;
	int  positionLine = 1;
	inputFile.open(inputFilename, ios::in);
	outFile.open(outFilename, ios::out);
	if (!inputFile.is_open() && !outFile.is_open())
	{
		cerr << "File opening error \n";
	}

	if (CheckForNesting(inputFile, position, positionLine))
	{
		cout << "All brackets are correct\n";
		ChangeBrackets(inputFile, outFile);
	}
	else
	{
		errorMessage(inputFile, positionLine, position);
	}
	inputFile.close();
	outFile.close();
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "ru");

	char choice;
	string inputFile;
	string outFile;

	do
	{
		cout << "Enter the input filename: ";
		cin >> inputFile;
		cout << '\n' << "Entre the output filename: ";
		cin >> outFile;
		cout << '\n';
		processFile(inputFile, outFile);
		cout << "Do you want to process one more file?\n";
		cin >> choice;
	} while (choice == 'y' || choice == 'Y');
}