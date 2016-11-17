#ifndef _TILEMAP_H_
#define _TILEMAP_H_

class Tile;
class Tilemap
{
public:
  Tilemap();
  ~Tilemap();
  void create(int _boundY, int _tilesX, int _tilesY);
  bool isDarkTile(int _indX, int _indY);
  void addTiles(int _tilesX, int _tilesY);
  void renderTiles();
  void click(ALLEGRO_MOUSE_STATE _mouse, bool _isShift);
private:
  int m_tilesX,
      m_tilesY,
      m_tileW,
      m_tileH;

  std::shared_ptr<Core> m_core;
  
  std::vector<std::vector<std::shared_ptr<Tile>>> m_tilemap;
};
#endif