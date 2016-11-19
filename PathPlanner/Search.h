#ifndef _SEARCH_H_
#define _SEARCH_H_

class Core;
class Search
{
public:
  Search();
  ~Search();
  std::vector<std::shared_ptr<Tile>> BreadthFirst(std::shared_ptr<Tile> _startTile, std::shared_ptr<Tile> _endTile, std::shared_ptr<Tilemap> _tilemap);
private:
  std::shared_ptr<Core> m_core;
};

#endif