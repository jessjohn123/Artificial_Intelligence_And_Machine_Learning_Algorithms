#include "PathSearch.h"
#define A_STAR 1
#define hWeight 1.2f


namespace fullsail_ai {
	namespace algorithms {

		bool isGreater(PlannerNode* const &lhs, PlannerNode* const &rhs)
		{

#if A_STAR
			return (lhs->finalCost > rhs->finalCost);
#endif

			return false;
		}

		PathSearch::PathSearch() : openEnhancer(isGreater)
		{
			Start = nullptr;
			End = nullptr;  
			FoundIT = false;
		}

		PathSearch::~PathSearch()
		{

		}

		void PathSearch::initialize(TileMap* _tileMap)
		{
			QueryPerformanceFrequency(&m_frequency);

			for (int i = 0; i < _tileMap->getColumnCount(); i++)
			{
				for (int j = 0; j < _tileMap->getRowCount(); j++)
				{
					if (_tileMap->getTile(j, i)->getWeight() == 0)
						continue;

					SearchNode* newNode = new SearchNode();

					newNode->m_Tile = _tileMap->getTile(j, i);

					if (j % 2 != 0)
					{
						if (_tileMap->getTile(j - 1, i) && _tileMap->getTile(j - 1, i)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j - 1, i));
						if (_tileMap->getTile(j - 1, i + 1) && _tileMap->getTile(j - 1, i + 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j - 1, i + 1));

						if (_tileMap->getTile(j, i - 1) && _tileMap->getTile(j, i - 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j, i - 1));
						if (_tileMap->getTile(j, i + 1) && _tileMap->getTile(j, i + 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j, i + 1));

						if (_tileMap->getTile(j + 1, i) && _tileMap->getTile(j + 1, i)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j + 1, i));
						if (_tileMap->getTile(j + 1, i + 1) && _tileMap->getTile(j + 1, i + 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j + 1, i + 1));
					}

					else if (j % 2 == 0)
					{
						//Top
						if (_tileMap->getTile(j - 1, i - 1) && _tileMap->getTile(j - 1, i - 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j - 1, i - 1));
						if (_tileMap->getTile(j - 1, i) && _tileMap->getTile(j - 1, i)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j - 1, i));
						//Right
						if (_tileMap->getTile(j, i - 1) && _tileMap->getTile(j, i - 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j, i - 1));
						//Left
						if (_tileMap->getTile(j, i + 1) && _tileMap->getTile(j, i + 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j, i + 1));
						//Bottom
						if (_tileMap->getTile(j + 1, i - 1) && _tileMap->getTile(j + 1, i - 1)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j + 1, i - 1));
						if (_tileMap->getTile(j + 1, i) && _tileMap->getTile(j + 1, i)->getWeight() != 0)
							newNode->m_neighbors.push_back(_tileMap->getTile(j + 1, i));
					}
					m_Tiles.push_back(newNode);
				}
			}

			for (size_t i = 0; i < m_Tiles.size(); i++)
			{
				SearchGraph[m_Tiles[i]->m_Tile] = m_Tiles[i];
			}
		}

		void PathSearch::enter(int startRow, int startColumn, int goalRow, int goalColumn)
		{

			FoundIT = false;

			for (size_t i = 0; i < m_Tiles.size(); i++)
			{
				if (m_Tiles[i]->m_Tile->getRow() == startRow && m_Tiles[i]->m_Tile->getColumn() == startColumn)
					Start = SearchGraph[m_Tiles[i]->m_Tile];
				if (m_Tiles[i]->m_Tile->getRow() == goalRow && m_Tiles[i]->m_Tile->getColumn() == goalColumn)
					End = SearchGraph[m_Tiles[i]->m_Tile];
			}

			PlannerNode* starter = new PlannerNode();
			starter->parent = nullptr;
			starter->vertex = Start;
			openEnhancer.push(starter);
			m_visted[Start->m_Tile] = openEnhancer.front();

			openEnhancer.front()->heuristicCost = estimate(Start->m_Tile, End->m_Tile);
			openEnhancer.front()->givenCost = 0;
			openEnhancer.front()->finalCost = openEnhancer.front()->heuristicCost * hWeight;
			
		}

		float PathSearch::estimate(Tile* start, Tile* goal)
		{
			//Distane formula
			int x = (goal->getXCoordinate() - start->getXCoordinate());
			x *= x;		
			int y = (goal->getYCoordinate() - start->getYCoordinate());
			y *= y;

			return sqrt((x + y));
		}

		void PathSearch::update(long timeslice)
		{
			QueryPerformanceCounter(&m_timeStart);
			long slicer = timeslice;


#if A_STAR
			while (true)
			{
				PlannerNode* current = openEnhancer.front();
				openEnhancer.pop();

				if (current->vertex == End)
				{
					//We Done code goes here
					m_visted[current->vertex->m_Tile] = current;
					FoundIT = true;
					return;
				}

				int size = current->vertex->m_neighbors.size();
				for (int i = 0; i < size; i++)
				{
					if (current->vertex->m_neighbors[i]->getWeight() == 0)
						continue;

					float tempGivenCost = current->givenCost + estimate(current->vertex->m_Tile, current->vertex->m_neighbors[i]) * current->vertex->m_neighbors[i]->getWeight();

					if (m_visted[current->vertex->m_neighbors[i]] == nullptr)
					{
						SearchNode* successor = SearchGraph[current->vertex->m_neighbors[i]];
						PlannerNode* successorNode = new PlannerNode();
						successorNode->vertex = successor;
						successorNode->parent = current;
						successorNode->givenCost = tempGivenCost;
						successorNode->heuristicCost = estimate(successor->m_Tile, End->m_Tile);
						successorNode->finalCost = successorNode->givenCost + successorNode->heuristicCost * hWeight;
						m_visted[current->vertex->m_neighbors[i]] = successorNode;
						openEnhancer.push(successorNode);
					}

					else
					{
						if (tempGivenCost < m_visted[current->vertex->m_neighbors[i]]->givenCost)
						{
							PlannerNode* SuccessorNode = m_visted[current->vertex->m_neighbors[i]];
							openEnhancer.remove(SuccessorNode);
							SuccessorNode->givenCost = tempGivenCost;
							SuccessorNode->finalCost = SuccessorNode->givenCost + SuccessorNode->heuristicCost * hWeight;
							SuccessorNode->parent = current;
							openEnhancer.push(SuccessorNode);
						}
					}
				}
				QueryPerformanceCounter(&m_timeEnd);
				slicer -= static_cast<long>(m_timeEnd.QuadPart - m_timeStart.QuadPart) / static_cast<long>(m_frequency.QuadPart);

				if (slicer <= 0)
					return;

			}
#endif
		}

		void PathSearch::exit()
		{
			while (!open.empty())
				open.pop();
			for (auto i = m_visted.begin(); i != m_visted.end(); i++)
			{
				delete i->second;
			}
			m_visted.clear();

			while (!openEnhancer.empty())
				openEnhancer.pop();
		}

		void PathSearch::shutdown()
		{
			FoundIT = false;
			exit();

			for (int i = 0; i < m_Tiles.size(); i++)
			{
				delete m_Tiles[i];
			}

			m_Tiles.clear();
		}

		bool PathSearch::isDone() const
		{
			return FoundIT;
		}

		std::vector<Tile const*> const PathSearch::getSolution() const
		{
			std::vector<Tile const*> temp;
			PlannerNode* check = nullptr;
			std::unordered_map<Tile*, PlannerNode*>::const_iterator iter;

			iter = m_visted.find(End->m_Tile);
			temp.push_back(End->m_Tile);

			if (iter != m_visted.end())
				check = iter->second;
			else
				return temp;

			while (true)
			{
				if (check == nullptr)
					break;
				temp.push_back(check->vertex->m_Tile);
				check = check->parent;
			}
			return temp;
		}
	}
}  // namespace fullsail_ai::algorithms


