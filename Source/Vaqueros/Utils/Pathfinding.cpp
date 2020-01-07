#include "Pathfinding.h"
#include <iterator>


Pathfinding::Pathfinding()
{
}

Pathfinding::~Pathfinding()
{
}



//Returns reachable tiles 
vector<LogicTile*> Pathfinding::FindNearTiles(LogicTile* tile, int depth, Matriz* _matrix)
{
	return Search(tile, nullptr, depth, _matrix);
}

// devuelve la casilla con más enemigos a tiro
LogicTile* Pathfinding::FindPositionWithMoreEnemies(AVaquerosCharacter* character, Matriz* _matrix)
{
	return Search(character->GetVaqueroLocation(), nullptr, character->GetNumMovements(), _matrix, character).at(0);
}

//Returns the path to follow
vector<LogicTile*> Pathfinding::FindPath(LogicTile* start, LogicTile* end, Matriz* _matrix)
{
	return Search(start, end, 0, _matrix);
}

vector<LogicTile*> Pathfinding::FindClosestPath(LogicTile* start, LogicTile* end, int movements, Matriz* _matrix)
{
	vector<LogicTile*> path = Search(start, end, 0, _matrix);
	int rest = 0;
	if(movements < (int)(path.size()) )
	{
		rest = (int) path.size() - movements;
	}
	vector<LogicTile*>::const_iterator first = path.begin();
	vector<LogicTile*>::const_iterator last = path.begin() + (int)path.size()-rest;
	return vector<LogicTile*>(first, last);
}

//Algorithm that makes A* or Breadth-first search depending on wether "end" has a value or not
vector<LogicTile*> Pathfinding::Search(LogicTile* start, LogicTile* end, int depth, Matriz* _matrix, AVaquerosCharacter* character)
{
	vector<Node*> openNodes;
	vector<Node*> closedNodes;


	//IMPLEMENTACION
	int h = 0;
	if (end != nullptr) { h = CalculateH(start, end); } //H is 0 if the algorithm is Breadth-first search 
	Node*  stNode = new Node(start, nullptr, 0, h); //Initialize the first node
	openNodes.push_back(stNode);
	Node* current;

	while (!openNodes.empty())
	{
		//takes the first node and erases it from open nodes
		current = openNodes.at(0);
		openNodes.erase(openNodes.cbegin());

		// If the taken node is the end the algorithm ends and returns the path
		if (current->tile == end && end != nullptr)
		{
			return pathReconstruction(current);
		}

		//Takes all the reachable tiles from the current tile
		TArray<LogicTile*> neighbours = _matrix->neighbours(current->tile->getLogicalX(), current->tile->getLogicalY());

		for (LogicTile* neighbour : neighbours)
		{
			// If the actor can't pass through the tile then continue with other tile
			if (!Transitable(neighbour))
			{
				//Comprobación de que el end no esté ocupado ni es la casilla de salida para quedarnos justo al lado.
				if (neighbour == end && end != start) return pathReconstruction(current);


				continue;
			}

			h = 0;
			if (end != nullptr) { h = CalculateH(start, end); } //H is 0 if the algorithm is Breadth-first search 
			Node *successor = new Node(neighbour, current, current->g + neighbour->getCost(), h);

			//si el coste es mayor que la profundidad pasa (si el depth es 0 es un pathfinding a una casilla que se sabe que se puede alcanzar)
			if(successor->g > depth && depth > 0)
			{
				continue;
			}


			int posOpen = findInVector(successor, openNodes);

			if (posOpen != -1)
			{
				//If the tile is in the open vector with a lower g then continue
				if (openNodes.at(posOpen)->g <= successor->g) continue;
			}
			else
			{
				int posClosed = findInVector(successor, closedNodes);

				if (posClosed != -1)
				{
					//If the tile is in the closed vector with a lower g then continues
					if (closedNodes.at(posClosed)->g <= successor->g) continue;
					//else adds the node to open vector and deletes the node in closed vector
					
					closedNodes.erase(closedNodes.begin() + posClosed);
				}
				sortedInsertion(openNodes, successor);
			}
		}

		//Si existe un character ordenará los closednodes por el número de enemigos que tiene a tiro
		// siendo la primera casilla la que más enemigos tiene a tiro
		if(character)
		{
			sortedInsertion(closedNodes, current, character);
		}
		else
		{
			closedNodes.push_back(current);
		}

	}


	if (end == nullptr) {
		return convertToTiles(closedNodes);
	}

	return {};
}

//Returns true if the actor can pass through the tile
bool Pathfinding::Transitable(const LogicTile* tile)
{
	return tile->_walkable && !tile->_occupied;
}

//Returns the h value of a node
int Pathfinding::CalculateH(const LogicTile* current, const LogicTile* end)
{
	//Manhattan
	return (abs(end->getLogicalX() - current->getLogicalX()) +
		abs(end->getLogicalY() - current->getLogicalY()));
}

// Inserts neatly a node in a vector of nodes. Si se le pasa un character dará prioridad a la casilla con más enemigos a tiro, si no
// dará prioridad a las casillas con menos f.
void Pathfinding::sortedInsertion(vector<Node*> &list, Node* node, AVaquerosCharacter* character)
{	
	if(character)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			if (character->objetivesInRange(node->tile) >= character->objetivesInRange((*it)->tile))
			{
				list.insert(it, node);
				return;
			}
		}
		list.push_back(node);
	}
	else
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			if (node->f <= (*it)->f)
			{
				list.insert(it, node);
				return;
			}
		}
		list.push_back(node);
	}
	
}

// Find a node in a vector using its tile
int Pathfinding::findInVector(const ::Pathfinding::Node* node, const vector<::Pathfinding::Node*>& nodes)
{
	int nodessize = nodes.size();
	for (int i = 0; i < nodessize; ++i)
	{
		if (node->tile == nodes.at(i)->tile)
		{
			return i;
		}
	}
	return -1;
}

// Returns the tile path
vector<LogicTile*> Pathfinding::pathReconstruction( ::Pathfinding::Node* node)
{
	//Si no se ha encontrado nada o si la casilla devuelta es la de salida
	if(node == nullptr || node->parent == nullptr)
	{
		return {};
	}
	vector<LogicTile*> path;
	LogicTile* tileAfter = node->tile;
	Node *current = node;
	do
	{
		path.insert(path.cbegin(), tileAfter);
		current = current->parent;
		tileAfter = current->tile;
	} while (current->parent != nullptr);

	return path;
}

// Converts a Node vector in a ULogicTile vector
vector<LogicTile*> Pathfinding::convertToTiles(const vector<Node*>& nodes)
{
	vector<LogicTile*> rVector = vector<LogicTile*>();
	for(Node* node : nodes)
	{
		rVector.push_back(node->tile);
	}

	return rVector;
}



