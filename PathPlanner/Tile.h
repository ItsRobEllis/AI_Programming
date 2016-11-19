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

  //Pathfinding
  bool getTileOpened(void) { return m_beenOpened; };
  void setTileOpened(bool _open) { m_beenOpened = _open; };
  bool getTileClosed(void) { return m_beenClosed; };
  void setTileClosed(bool _close) { m_beenClosed = _close; };

  //Getters and setters for coordinates
  int getX(void) { return m_posX; };
  int getY(void) { return m_posY; };
  int setX(int _x) { m_posX = _x; };
  int setY(int _y) { m_posX = _y; };

  int getIndX(void) { return m_indX; };
  int getIndY(void) { return m_indY; };

  //Parents
  std::shared_ptr<Tile> getParent(void) { return m_parent; };
  bool hasParent(void) { return m_parent != nullptr; };
  void setParent(std::shared_ptr<Tile> _newParent) { m_parent = _newParent; };

  //Rendering
  void renderTile();
  
  //Constructor and Destructor
  Tile(int _posX, int _posY, int _tileW, int _tileH, bool _isDarkTile);
  ~Tile();
private:
    //The tile position on-screen in pixels
  int m_posX, 
      m_posY,
    //The tile's index in the tilemap
      m_indX, 
      m_indY,
    //The tile's dimensions
      m_tileW,
      m_tileH;
  bool m_isDark,
       m_beenOpened,
       m_beenClosed;
  TType m_tileType;
  std::shared_ptr<Tile> m_parent;
};

#endif