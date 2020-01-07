#pragma once

#include <vector>
#include "MatrixLogic/Matriz.h"
#include "Characters/Characters/VaquerosCharacter.h"
using namespace std;

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();

	static vector<LogicTile*> FindPath(LogicTile* start, LogicTile* end, Matriz* _matrix);
	static vector<LogicTile*> FindClosestPath(LogicTile* start, LogicTile* end, int movements, Matriz* _matrix);
	static vector<LogicTile*> FindNearTiles(LogicTile* tile, int depth, Matriz* _matrix);
	static LogicTile* FindPositionWithMoreEnemies(AVaquerosCharacter* character, Matriz* _matrix);

private:
	 class Node
		{
		public:
			Node()
			{
				
			}
			Node(LogicTile* tile, Node* parent, double g, double h)
				: tile(tile),
				  parent(parent),
				  g(g),
				  h(h)
			{
				f = g + h;
			}

			LogicTile* tile;
			Node* parent;
			double f, g, h;
		};

	static bool Transitable(const LogicTile* tile);
	static int CalculateH(const LogicTile* current, const LogicTile* end);
	static void sortedInsertion(vector<Node*>& list, Node* node, AVaquerosCharacter* character = nullptr);
	static int findInVector(const ::Pathfinding::Node* node, const vector<Node*>& nodes);
	static vector<LogicTile*> pathReconstruction(Node* node);
	static vector<LogicTile*> convertToTiles(const vector<Node*>& nodes);
	static vector<LogicTile*> Search(LogicTile* start, LogicTile* end, int depth, Matriz* _matrix, AVaquerosCharacter* character = nullptr);

	
};

