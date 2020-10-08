#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Pair
{
	private:
	int _x;
	int _y;

	public:

	Pair(): _x(), _y() {}

	Pair(int x, int y): _x(x), _y(y) {}

	int GetX()
	{
		return _x;
	}

	int GetY()
	{
		return _y;
	}

	friend bool operator ==(const Pair& p1, const Pair& p2)
	{
		return p1._x == p2._x && p1._y == p2._y;
	}

	friend ostream& operator << (ostream& out, Pair p)
	{
		out << '{'<< p._x << ", " << p._y << "}\n";
		return out;
	}
};

class AlgorithmTerry
{
	private:
	bool** _matrix;
	int _size;

	public:
	AlgorithmTerry(string fileName)
	{
		ReadFromFile(fileName);
	}

	void ReadFromFile(string fileName)
	{
		ifstream in;
		in.open(fileName);
		if(in.is_open() == false)
		{
			cout << "Файл не найден.\n";
		}

		string line;
		getline(in, line);
		_size = stoi(line);
		MemoryAllocation();
		
		int col = 0;
		while(getline(in, line))
		{
			SetValueToMatrix(line, col++);
		}
		in.close();
	}

	void Solve(int startVertex, int endVertex)
	{
		if(CheckVertex(startVertex) && CheckVertex(endVertex))
		{
			startVertex -= 1;
			endVertex -= 1;
			vector<Pair> group = GetGroup();
			
			DeleteStartAndEndVertexes(group, startVertex, endVertex);

			vector<int> path;
			path.push_back(startVertex);
			vector<Pair> checkedPairs;

			int currentVertex = startVertex;
			while(currentVertex != endVertex)
			{
				bool isNextDelete = false;
				for(int i = 0; i < group.size(); i++)
				{
					if(currentVertex == group[i].GetY())
					{
						RemoveByValue(group, group[i]);
						isNextDelete = true;
						break;
					}
				}
				if(isNextDelete)
				{
					continue;
				}

				for(int i = 0; i < group.size(); i++)
				{
					int countValueLeft = 0;
					int indexForDelete = 0;
					for(int i = 0; i < group.size(); i++)
					{
						if(currentVertex == group[i].GetX())
						{
							indexForDelete = i;
							countValueLeft++;
						}
					}
					if(countValueLeft == 0)
					{
						path.pop_back();
						currentVertex = path[path.size() - 1];
						break;
					}

					if(currentVertex == group[i].GetX())
					{
						

						checkedPairs.push_back(group[i]);
						path.push_back(group[i].GetY());
						currentVertex = group[i].GetY();
						RemoveByValue(group, group[i]);
						break;
					}
				}
			}

			cout << "Путь: ";
			for(int i = 0; i < path.size() - 1; i++)
			{
				cout << path[i] + 1 << " --> ";
			}
			cout << path[path.size() - 1] + 1<< '\n';
		}
		else
		{
			cout << "Введите корректные вершины!";
			exit(0);
		}
	}

	template < typename T>
	int GetIndexFirstOccurrence(vector<T> group, T value)
	{
		for(int i = 0; i < group.size(); i++)
		{
			if(group[i] == value)
			{
				return i;
			}
		}
		cout << "Такого значения - нет!\n";
		exit(0);
	}

	void DeleteStartAndEndVertexes(vector<Pair>& group, int startVertex, int endVertex)
	{
		vector<Pair> groupForDelete;
		for(Pair item : group)
		{
			if(item.GetX() == endVertex)
			{
				groupForDelete.push_back(item);
				continue;
			}
			if(item.GetY() == startVertex)
			{
				groupForDelete.push_back(item);
			}
		}

		for(int i = 0; i < groupForDelete.size(); i++)
		{
			for(int j = 0; j < group.size(); j++)
			{
				if(group[j] == groupForDelete[i])
				{
					RemoveByValue(group, group[j]);
					break;
				}
			}
		}
	}

	vector<Pair> GetGroup()
	{
		vector<Pair> pairs;
		for(int i = 0; i < _size; i++)
		{
			for(int j = 0; j < _size; j++)
			{
				if(_matrix[i][j] == 1)
				{
					pairs.push_back(Pair(i, j));
				}
			}
		}
		return pairs;
	}

	template < typename T>
	void RemoveByValue(vector<T>& values, T value)
	{
		values.erase(remove(values.begin(), values.end(), value), values.end());
	}

	template < typename T >
	void ShowList(vector<T> values)
	{
		for(int i = 0; i < values.size(); i++)
		{
			cout << values[i];
		}
	}

	void ShowMatrix()
	{
		for(int i = 0; i < _size; i++)
		{
			for(int j = 0; j < _size; j++)
			{
				cout << _matrix[i][j] << ' ';
			}
			cout << '\n';
		}
	}

	private:

	bool CheckVertex(int a)
	{
		return a >= 1 && a <= _size;
	}

	void SetValueToMatrix(string line, int row)
	{
		int col = 0;
		for(int i = 0; i < line.size(); i++)
		{
			if(line[i] == ' ')
			{
				continue;
			}
			else if(line[i] == '0' || line[i] == '1')
			{
				_matrix[row][col++] = (line[i] == '0') ? 0 : 1;
			}
			else
			{
				cout << "Некорректные символы в матрице!!!";
				exit(0);
			}
		}
	}

	void MemoryAllocation()
	{
		_matrix = new bool* [_size];
		for(int i = 0; i < _size; i++)
			_matrix[i] = new bool[_size];
	}
};

int main()
{
	setlocale(LC_ALL, "rus");
	AlgorithmTerry algorithm("matrix.txt");
	algorithm.ShowMatrix();
	algorithm.Solve(6, 7);

	return 0;
}