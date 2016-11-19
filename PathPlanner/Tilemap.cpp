#include "stdafx.h"
#include "Tilemap.h"

Tilemap::Tilemap()
{
	m_core = Core::GetCore();
}

Tilemap::~Tilemap()
{
}

void Tilemap::create(int _boundY, int _tilesX, int _tilesY)
{
  //PLEASE NOTE WHEN CREATING TILEMAPS
  //The tile numbers should be a factor of the correct dimension of the bounding box
  //For example, the tile area height is 704px, so a good number of tiles vertically is 22
  m_core->logMsg(Core::STANDARD, "Creating Tilemap");
  m_tileW = (m_core->getScreenW()) / _tilesX;
  m_tileH = (m_core->getScreenH() - _boundY) / _tilesY;
  m_tilesX = _tilesX;
  m_tilesY = _tilesY;

  //This block checks to see if the tile width and height are properly aligned to the bounding box
  double _checkX = (float)m_core->getScreenW() / (float)_tilesX;
  double _checkY = ((float)m_core->getScreenH() - (float)_boundY) / (float)_tilesY;
  if (_checkX != floor(_checkX) || _checkY != floor(_checkY))
  {
    //It warns the user about this in the debug console
	  m_core->logMsg(Core::WARN, "One or more tile dimensions do not factor into area dimensions");
    if (_checkX != floor(_checkX) || _checkY != floor(_checkY))
    {
		m_core->logMsg(Core::WARN, "Horizontal factor is incorrect");
    }
    if (_checkY != floor(_checkY))
    {
		m_core->logMsg(Core::WARN, "Vertical factor is incorrect");
    }
  }
  else
  {
    //If all is fine, it lets the user know
	  m_core->logMsg(Core::STANDARD, "Tile dimension factor OK");
  }

  for (int r = 0; r < _tilesX; r++)
  {
    std::vector<std::shared_ptr<Tile>> m_row;
    for (int c = 0; c < _tilesY; c++)
    {
      m_row.push_back(nullptr);      
    }
    m_tilemap.push_back(m_row);
  }
  m_core->logMsg(Core::STANDARD, "Tilemap created");
  addTiles(_tilesX, _tilesY);
}

void Tilemap::addTiles(int _tilesX, int _tilesY)
{
	m_core->logMsg(Core::STANDARD, "Adding tiles to tilemap");
  bool _isDark = false;  

  for (int r = 0; r < _tilesX; r++)
  {
    for (int c = 0; c < _tilesY; c++)
    {
      // Colour the tile dependant on index
      bool _isDark = isDarkTile(r, c);

      //Create a tile
      std::shared_ptr<Tile> _newTile = std::make_shared<Tile>(r * m_tileW, c * m_tileH, m_tileW, m_tileH, _isDark);

      m_tilemap[r][c] = _newTile;
    }
  }
  m_core->logMsg(Core::STANDARD, "Tiles added");
}

void Tilemap::renderTiles()
{
  for (int r = 0; r < m_tilesX; r++)
  {
    for (int c = 0; c < m_tilesY; c++)
    {
      // Colour the tile dependant on index
      bool _isDark = isDarkTile(r, c);
      //Render a tile
      m_tilemap[r][c]->renderTile();
    }
  }
}

bool Tilemap::isDarkTile(int _indX, int _indY)
{
  if (_indX % 2 == 0)
  {
    if (_indY % 2 == 0)
    {
      return false;
    }
    return true;
  }
  else
  {
    if (_indY % 2 == 0)
    {
      return true;
    }
    return false;
  }
}

void Tilemap::click(ALLEGRO_MOUSE_STATE _mouse, bool _isShift)
{
  for (int r = 0; r < m_tilesX; r++)
  {
    for (int c = 0; c < m_tilesY; c++)
    {
      // Checks to see if a start or end point exists 
      if (_mouse.buttons & 1 && _isShift == true)
      {
        if (m_tilemap[r][c]->getTileType() == START)
        {
          //If it exists, it removes it
          (m_tilemap[r][c]->setTileType(FLOOR));
        }
      }
      if (_mouse.buttons & 2 && _isShift == true)
      {
        if (m_tilemap[r][c]->getTileType() == END)
        {
          (m_tilemap[r][c]->setTileType(FLOOR));
        }
      }
      m_tilemap[r][c]->click(_mouse, _isShift);
    }
  }
}

std::vector<std::shared_ptr<Tile>> Tilemap::getNeighbours(std::shared_ptr<Tile> _tile)
{
  std::vector<std::shared_ptr<Tile>> m_neighbours;
  if (_tile->getIndY() + 1 < m_tilesY || _tile->getTileType() != OBSTACLE)
  {
    m_neighbours.push_back(m_tilemap[_tile->getIndY() + 1][_tile->getIndX()]);
  }
  if (_tile->getIndY() - 1 >= 0 || _tile->getTileType() != OBSTACLE)
  {
    m_neighbours.push_back(m_tilemap[_tile->getIndY() - 1][_tile->getIndX()]);
  }
  if (_tile->getIndX() + 1 < m_tilesX || _tile->getTileType() != OBSTACLE)
  {
    m_neighbours.push_back(m_tilemap[_tile->getIndY()][_tile->getIndX() + 1]);
  }
  if (_tile->getIndX() - 1 >= 0 || _tile->getTileType() != OBSTACLE)
  {
    m_neighbours.push_back(m_tilemap[_tile->getIndY()][_tile->getIndX() - 1]);
  }
  return m_neighbours;
}

std::shared_ptr<Tile> Tilemap::getStart()
{
  for (int r = 0; r < m_tilesX; r++)
  {
    for (int c = 0; c < m_tilesY; c++)
    {
      if (m_tilemap[r][c]->getTileType() == START)
      {
        return m_tilemap[r][c];
      }
    }
  }
  m_core->logMsg(Core::WARN, "Startpoint not found, search cannot run!");
  return nullptr;
}

std::shared_ptr<Tile> Tilemap::getEnd()
{
  for (int r = 0; r < m_tilesX; r++)
  {
    for (int c = 0; c < m_tilesY; c++)
    {
      if (m_tilemap[r][c]->getTileType() == END)
      {
        return m_tilemap[r][c];
      }
    }
  }
  m_core->logMsg(Core::WARN, "Endpoint not found, search cannot run!");
  return nullptr;
}