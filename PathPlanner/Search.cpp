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
  std::shared_ptr<Tile> m_currentTile; // (0, 0, 10, 10, false);

  std::queue<std::shared_ptr<Tile>> m_tileQueue;
  m_tileQueue.push(_startTile);
  _startTile->setTileOpened(true);

  while(!m_tileQueue.empty())
  {
    m_currentTile = m_tileQueue.front();
    m_tileQueue.pop();
    m_currentTile->setTileClosed(true);

    if (m_currentTile->getTileType() == END)
    {
      std::vector<std::shared_ptr<Tile>> m_path;
      while (m_currentTile->hasParent())
      {
        m_path.push_back(m_currentTile);
        m_currentTile = m_currentTile->getParent();
      }
      return m_path;
    }

    std::vector<std::shared_ptr<Tile>> m_neighbour = _tilemap->getNeighbours(m_currentTile);
    for (unsigned int n = 0; n < m_neighbour.size(); n++)
    {
      if(m_neighbour[n]->getTileClosed() || m_neighbour[n]->getTileOpened())
      {
        continue; //Do nothing
      }
      m_tileQueue.push(m_neighbour[n]);
      m_neighbour[n]->setTileOpened(true);
      m_neighbour[n]->setParent(m_currentTile);
    }
  }
  return {};
}