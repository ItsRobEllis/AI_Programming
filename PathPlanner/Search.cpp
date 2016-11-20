#include "stdafx.h"
#include "Search.h"

Search::Search()
{
  m_core = Core::GetCore();
}

Search::~Search()
{
}

std::vector<std::shared_ptr<Tile>> Search::BreadthFirst(std::shared_ptr<Tile> _startTile, std::shared_ptr<Tile> _endTile, std::shared_ptr<Tilemap> _tilemap)
{
  m_core->logMsg(Core::STANDARD, "Starting search");
  m_hasSearched = true;
  std::shared_ptr<Tile> m_currentTile;

  std::vector<std::shared_ptr<Tile>> m_path;

  std::queue<std::shared_ptr<Tile>> m_tileQueue;
  m_tileQueue.push(_startTile);
  _startTile->setTileOpened(true);

  while(!m_tileQueue.empty())
  {
    m_currentTile = m_tileQueue.front();
    m_tileQueue.pop();
    m_currentTile->setTileClosed(true);

    if (m_currentTile == _endTile)
    {  
      while (m_currentTile->hasParent())
      {
        m_path.push_back(m_currentTile);
        m_currentTile = m_currentTile->getParent();
      }
      m_core->logMsg(Core::STANDARD, "Search ended!");
      m_path.push_back(_startTile);
      return m_path;
    }

    std::vector<std::shared_ptr<Tile>> m_neighbour = _tilemap->getNeighbours(m_currentTile);
    for (unsigned int n = 0; n < m_neighbour.size(); n++)
    {
      if(m_neighbour[n]->getTileClosed() || m_neighbour[n]->getTileOpened())
      {
        continue; //Do nothing and finish the loop
      }
      m_tileQueue.push(m_neighbour[n]);
      m_neighbour[n]->setTileOpened(true);
      m_neighbour[n]->setParent(m_currentTile);
    }
  }
  m_core->logMsg(Core::SEVERE, "Search ended unexpectedly!");
  return {};
}

void Search::renderPath(std::vector<std::shared_ptr<Tile>> _path, bool _canRender)
{
  if (_canRender)
  {
    for (int p = 0; p < _path.size(); p++)
    {
      if (p != _path.size() - 1)
      {
        al_draw_line(_path[p]->getX() + (_path[p]->getW() / 2), _path[p]->getY() + (_path[p]->getH() / 2), _path[p + 1]->getX() + (_path[p + 1]->getW() / 2), _path[p + 1]->getY() + (_path[p + 1]->getH() / 2), al_map_rgb(90, 90, 210), 5);
      }
    }
  }
} 