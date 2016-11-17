#ifndef _TILE_H_
#define _TILE_H_

class Tile
{
public:
  //Tile Actions
  void click(ALLEGRO_MOUSE_STATE _mouse, bool _isShift);

  //Tile Type
  TType getTileType(void) { return m_tileType; };
  void setTileType(TType _tileType) { m_tileType = _tileType; };

  //Rendering
  void renderTile();
  
  //Constructor and Destructor
  Tile(int _posX, int _posY, int _tileW, int _tileH, bool _isDarkTile);
  ~Tile();
private:
  int m_posX,
      m_posY,
      m_tileW,
      m_tileH;
  bool m_isDark;
  TType m_tileType;
};

#endif