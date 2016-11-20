#ifndef _TILEMAP_H_
#define _TILEMAP_H_

class Core;
class Tile;
class Tilemap
{
public:
  Tilemap();
  ~Tilemap();
  void create(int _boundY, int _tilesX, int _tilesY);
  bool isDarkTile(int _indX, int _indY);
  bool m_showCoords;
  void addTiles(int _tilesX, int _tilesY);
  void renderTiles();
  void click(ALLEGRO_MOUSE_STATE _mouse, bool _isShift);
  std::vector<std::shared_ptr<Tile>> getNeighbours(std::shared_ptr<Tile> _tile);
  
  //Getting number of tiles horizontally and vertically
  int getTilesX(void) { return m_tilesX; };
  int getTilesY(void) { return m_tilesY; };

  //Clearing tilemap flags for repeated searches
  void clearTileFlags();

  std::shared_ptr<Tile> getStart();
  std::shared_ptr<Tile> getEnd();
private:
  int m_tilesX,
      m_tilesY,
      m_tileW,
      m_tileH;

  std::shared_ptr<Core> m_core;
  
  std::vector<std::vector<std::shared_ptr<Tile>>> m_tilemap;
};
#endif