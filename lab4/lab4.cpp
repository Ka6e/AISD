#include <iostream>
#include <list>
#include <Windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>

//Compiler Version 19.41.34120 for x86
//Visual Studio 2022
//Васильев Иван ПС-23
//25. На диске имеется база  данных  по  физическим  эффектам
//(ФЭ).Каждый  ФЭ  записан в виде тройки компонент : название,
//входная  физическая  величина(вход), выходная   физическая
//величина(выход).Имеется  не более 100 различных физических
//величин.Требуется построить в основной памяти граф связи ФЭ и
//обеспечить  поиск  цепочек  ФЭ от одного до четырех звеньев по
//заданным входу и выходу по возрастанию количества звеньев(12).

struct Vertex;
struct Edge
{
	std::string Phys;
	Edge() = default;
	Edge(std::string name)
	{
		this->Phys = name;
	}

	bool operator==(const Edge& other) const
	{
		return this->Phys == other.Phys;
	}
};

struct Vertex
{
	Vertex() = default;
	Vertex(int number, std::string physicalName)
	{
		this->number = number;
		this->physicalName = physicalName;
	}
	int number;
	std::string physicalName;
	std::list<std::pair<Vertex*, Edge>> adjList;
};

class Graph
{
public:
	std::vector<Vertex> vertex;
};
void fillVertex(std::string fileName, Graph& graph)
{
	std::fstream inputFile(fileName, std::ios::in);
	try
	{

		if (!inputFile.is_open())
		{
			throw std::runtime_error("Ошибка открытия файла!\n");
		}
		std::string line;
		std::string number;
		std::string name;
		while (std::getline(inputFile, line))
		{
			auto pos = line.find(':');
			if (pos != std::string::npos)
			{
				number = line.substr(0, pos);
				name = line.substr(pos + 1);
				Vertex newVertex(std::stoi(number), name);
				graph.vertex.push_back(newVertex);
			}
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what();
	}
	inputFile.close();
}

std::vector<std::string> split(std::vector<std::string>& vec, std::string str)
{
	vec.clear();
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, ';'))
	{
		vec.push_back(item);
	}
	return vec;
}

bool checkForSecondPhys(const std::string& str, int& pos)
{
	for (auto i = 0; i < str.size(); i++)
	{
		if (str[i] == ',')
		{
			pos = i;
			return true;
		}
	}
	return false;
}


void devideString(std::string& firstPhys, std::string& secondPhys, int& pos)
{
	std::string temp = firstPhys;
	firstPhys = temp.substr(0, pos);
	secondPhys = temp.substr(pos + 1);
	pos = 0;
}

Vertex* binarySearch(int number, Graph& graph)
{
	int left = 0;
	int right = graph.vertex.size() - 1;
	while (left <= right)
	{
		int mid = (left + right) / 2;
		if (graph.vertex[mid].number == number)
		{
			return &(graph.vertex[mid]);
		}
		else if (graph.vertex[mid].number > number)
		{
			right = mid - 1;
		}
		else
		{
			left = mid + 1;
		}
	}
}

void createEdges(std::string filename, Graph& graph)
{
	std::vector<std::string> fillLines;
	std::fstream inputFile(filename, std::ios::in);
	try
	{
		if (!inputFile.is_open())
		{
			throw std::runtime_error("Ошибка открытия файла!\n");
		}
		int pos = 0;
		std::string line;
		std::string name;
		std::string firstVertivle;
		std::string secondVerticle;
		std::string lastVerticle;
		while (std::getline(inputFile, line))
		{
			Vertex* start1;
			Vertex* start2;
			Vertex* end;
			split(fillLines, line);
			name = fillLines[0];
			firstVertivle = fillLines[1];
			lastVerticle = fillLines[fillLines.size() - 1];
			if (checkForSecondPhys(firstVertivle, pos))
			{
				devideString(firstVertivle, secondVerticle, pos);
				start1 = binarySearch(std::stoi(firstVertivle), graph);
				start2 = binarySearch(std::stoi(secondVerticle), graph);
				end = binarySearch(std::stoi(lastVerticle), graph);
				Edge newEdge1(name);
				Edge newEdg2(name);
				start1->adjList.push_back(std::make_pair(end, newEdge1));
				start2->adjList.push_back(std::make_pair(end, newEdg2));

			}
			else
			{
				start1 = binarySearch(std::stoi(firstVertivle), graph);
				end = binarySearch(std::stoi(lastVerticle), graph);
				Edge newEdge(name);
				start1->adjList.push_back(std::make_pair(end, newEdge));

			}
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what();
	}
	inputFile.close();
}

std::pair<Vertex*, Edge> findElement(Vertex* vertex, std::list<std::pair<Vertex*, Edge>>& adjlist)
{
	std::pair<Vertex*, Edge> pair;
	for (auto& elemnt : adjlist)
	{
		if (elemnt.first == vertex)
		{
			pair = elemnt;
			return pair;
		}
	}
}

std::vector< std::vector<std::pair<Vertex*, Edge>>> devideVector(std::vector<std::vector<std::pair<Vertex*, Edge>>>& vec, std::vector<std::vector<std::pair<Vertex*, Edge>>>& other, int& index)
{
	if (index == vec.size())
	{
		return other;
	}
	for (auto i = index; i < vec.size() - 1; i++)
	{
		if (vec[i].size() == vec[i + 1].size())
		{
			other.push_back(vec[i]);
		}
		else
		{
			other.push_back(vec[i]);
			index = i + 1;
			break;
		}
	}

	return other;
}

void bfs(Vertex* start, Vertex* end, int maxDepth = 4)
{
	if (!start || !end) {
		std::cout << "Стартовая или конечная вершина не существует.\n";
		return;
	}
	std::queue<std::vector<std::pair<Vertex*, Edge>>> queue;
	std::vector<std::vector<std::pair<Vertex*, Edge>>> allPaths;

	for (const auto& edge : start->adjList)
	{
		std::vector<std::pair<Vertex*, Edge>> initialPath;
		initialPath.emplace_back(edge.first, edge.second);
		queue.push(initialPath);
	}

	while (!queue.empty())
	{
		auto currentPath = queue.front();
		queue.pop();

		Vertex* currentVertex = currentPath.back().first;

		if (currentVertex == end)
		{
			allPaths.push_back(currentPath);
			continue;
		}

		if (currentPath.size() >= maxDepth)
		{
			continue;
		}

		for (const auto& edge : currentVertex->adjList)
		{
			Vertex* neighbor = edge.first;

			bool alreadyInPath = false;
			for (const auto& step : currentPath)
			{
				if (step.first == neighbor)
				{
					alreadyInPath = true;
					break;
				}
			}
			if (alreadyInPath) continue;

			std::vector<std::pair<Vertex*, Edge>> newPath = currentPath;
			newPath.emplace_back(neighbor, edge.second);
			queue.push(newPath);
		}
	}

	if (allPaths.empty())
	{
		std::cout << "Не удалось найти путь от " << start->physicalName << " до " << end->physicalName << ".\n";
	}
	else
	{
		std::cout << "Найдены пути от " << start->physicalName << " к " << end->physicalName << ":\n";
		for (const auto& path : allPaths)
		{
			std::cout << start->physicalName;
			for (const auto& step : path) {
				std::cout << " -> " << step.second.Phys << " -> " << step.first->physicalName;
			}
			std::cout << '\n';
		}
	}
}

void physList(std::ifstream& file)
{
	std::string str;
	while (std::getline(file, str))
	{
		std::cout << str << '\n';
	}
}

int main()
{
	setlocale(LC_ALL, "ru");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Graph graph;
	std::ifstream inputFile("P_NAME.txt");
	try
	{
		if (!inputFile.is_open())
		{
			throw std::runtime_error("Ошибка открытия файла\n");
		}
		fillVertex("P_NAME.txt", graph);
		createEdges("PHYS.txt", graph);
		char choice;
		Vertex* startVerticle = nullptr;
		Vertex* endVertcle = nullptr;
		std::string start, end;
		do
		{
			std::cout << "Меню\n";
			std::cout << "1. Физические явления\n";
			std::cout << "2. Выбрать физические явления\n";
			std::cout << "3. Вывести найденные способы\n";
			std::cout << "4. Выйти\n";
			std::cin >> choice;
			switch (choice)
			{
			case '1':
				physList(inputFile);
				break;
			case '2':
				std::cout << "Выберите начальную вершину\n";
				std::cin >> start;
				std::cout << "Выберите конечную вершину\n";
				std::cin >> end;
				startVerticle = binarySearch(std::stoi(start), graph);
				endVertcle = binarySearch(std::stoi(end), graph);
				break;
			case '3':
				bfs(startVerticle, endVertcle);
				break;
			case '4':
				std::cout << "Выход из программы.\n";
				break;
			default:
				break;
			}

		} while (choice != '4');
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what();
	}
	inputFile.close();
}