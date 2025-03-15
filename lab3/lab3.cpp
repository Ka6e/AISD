#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>

using namespace std;

//Compiler Version 19.41.34120 for x86
//Visual Studio 2022
//Васильев Иван ПС-23
//18. Задано бинарное  дерево.Построить  прошитое   дерево,
//соответствующее обходу сверху вниз.Составить программу удале-
//ния поддерева с  корнем  в  заданной  вершине  без  повторного
//построения дерева.Конечное дерево должно оставаться прошитым.
// Конечное дерево должно оставаться прошитым.
//Выдать информацию о нитях исходного и конечного деревьев(11).

struct Node
{
	char ch;
	Node* right;
	bool isFather;
	bool isThreaded;
	Node* left;
	Node* prev;
	Node* next;
	Node* fath;
	int level;

	Node(char ch, int level)
	{
		this->ch = ch;
		isFather = false;
		isThreaded = false;
		fath = nullptr;
		right = nullptr;
		left = nullptr;
		prev = nullptr;
		next = nullptr;
		this->level = level;
	}
};

void insert(Node*& root, char& symbol, int level, int& currentLevel, Node*& lastNode)
{
	Node* newNode = new Node(symbol, level);

	if (!root)
	{
		root = newNode;
		lastNode = newNode;
		currentLevel = level;
		return;
	}
	if (level > currentLevel)
	{
		lastNode->left = newNode;
		lastNode->isFather = true;
		newNode->fath = lastNode;
	}
	else if (level == currentLevel)
	{
		if (lastNode->fath)
		{
			if (lastNode->fath->left && lastNode->fath->right)
			{
				throw std::runtime_error("У узла есть левый и правый потомок\n");

			}
			lastNode->fath->right = newNode;
			newNode->fath = lastNode->fath;
		}
		else
		{
			root->right = newNode;
			newNode->fath = root;
		}
	}

	else
	{
		Node* temp = lastNode;
		while (temp->fath && temp->level >= level) {
			temp = temp->fath;
		}
		if (temp->fath) {
			temp->fath->right = newNode;
			newNode->fath = temp->fath;
		}
		else
		{
			if (root->right)
			{
				throw std::runtime_error("Ошибка вставки узла\n");
			}
			root->right = newNode;
			newNode->fath = root;
		}
	}

	lastNode = newNode;
	currentLevel = level;
}

void threadTree(Node*& root, Node*& prevNode)
{
	if (!root) return;

	if (!root->isFather)
	{
		if (prevNode && !prevNode->isThreaded)
		{
			prevNode->next = root;
			root->prev = prevNode;
		}
		prevNode = root;

	}
	else
	{
		if (prevNode && !prevNode->isFather)
		{
			prevNode->next = root;
			root->prev = prevNode;
			prevNode->isThreaded = true;
		}

	}
	threadTree(root->left, prevNode);
	threadTree(root->right, prevNode);
}

void printThreads(Node* root) {
	if (!root)
	{
		return;
	}
	if (root->next)
	{
		cout << root->ch << "->" << root->next->ch << '\n';
	}
	printThreads(root->left);
	printThreads(root->right);

}

void printTree(Node* root) {
	if (root == nullptr) return;

	std::cout << '\n' << string(root->level, '.') << root->ch << '\t' << root->isFather;
	printTree(root->left);
	printTree(root->right);
}

Node* deleteSubtree(Node*& root, char& symbol, bool& isFound)
{
	if (!root) return nullptr;

	if (root->ch == symbol)
	{
		isFound = true;
	}
	Node* temLeft = deleteSubtree(root->left, symbol, isFound);
	Node* tempRight = deleteSubtree(root->right, symbol, isFound);
	if (isFound) {
		if (root->ch == symbol)
		{
			if (root->prev)
			{
				root->prev->next = root->next;
				if (root->next)
				{
					root->next->prev = root->prev;
					root->fath->isFather = false;
				}
			}
			else
			{
				if ((root->fath->left || root->fath->right) && !root->isFather)
				{
					root->next->prev = nullptr;
					isFound = false;
					if (!root->fath->left || !root->fath->right)
					{
						root->fath->isFather = false;
					}
					delete root;
					root = nullptr;
					return root;
				}
				root->fath->next = root->next;
				if (root->next)
				{
					root->next->prev = root->fath;
				}
			}
			root->fath->isFather = false;
			delete root;
			root = nullptr;
			isFound = false;
		}
		else {
			if (root->prev) {
				root->prev->next = root->next;
				root->next->prev = root->prev;
				delete root;
				root = nullptr;
				return root;
			}
			else
			{
				if (root->fath)
				{
					root->next->prev = nullptr;
					root->fath->next = root->next;
					root->next->prev = root->fath;

				}
			}
			delete root;
			root = nullptr;
		}
	}

	return root;
}

void loadTreeFromFile(const string& filename, Node*& root) {
	char ch;
	fstream inputFile(filename, ios::in);
	int currentLevel = -1;
	int pos;
	Node* lastNode = nullptr;
	try
	{
		if (!inputFile.is_open())
		{
			throw std::runtime_error("Ошибка открытия файла!\n");
		}
		string line;
		while (getline(inputFile, line))
		{
			pos = 0;
			while (line[pos] == '.') pos++;
			ch = line[pos];
			insert(root, ch, pos, currentLevel, lastNode);
		}
		inputFile.close();
	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
		std::exit(1);
	}

}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "ru");

	Node* tree = nullptr;
	Node* prev = nullptr;
	string inputFile = "input.txt";
	char choice, symbol;
	bool isFound = false;

	loadTreeFromFile(inputFile, tree);
	threadTree(tree, prev);

	do {
		cout << "\nМеню:\n";
		cout << "1. Показать дерево\n";
		cout << "2. Показать нити дерева\n";
		cout << "3. Удалить поддерево\n";
		cout << "4. Выйти\n";
		cout << "Введите ваш выбор: ";
		cin >> choice;

		switch (choice) {
		case '1':
			cout << "Дерево (обход сверху вниз): ";
			printTree(tree);
			cout << endl;
			break;
		case '2':
			printThreads(tree);
			break;
		case '3':
			cout << "Введите символ корня поддерева для удаления: ";
			cin >> symbol;
			tree = deleteSubtree(tree, symbol, isFound);
			cout << "Поддерево удалено.\n";
			break;
		case '4':
			cout << "Выход из программы.\n";
			break;
		default:
			cout << "Неверный выбор. Попробуйте снова.\n";
		}
	} while (choice != '4');
}