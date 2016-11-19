#include "stdafx.h"
#include "Tile.h"

Tile::Tile(int _posX, int _posY, int _tileW, int _tileH, bool _isDarkTile)
{
  //logMsg(STANDARD, "Created tile!");

  m_tileType = FLOOR;
  m_posX = _posX;
  m_posY = _posY;
  m_tileW = _tileW;
  m_tileH = _tileH;
  m_isDark = _isDarkTile;
  m_parent = nullptr;
  m_indX = m_posX / m_tileW;
  m_indY = m_posY / m_tileH;
  m_beenOpened = false;
  m_beenClosed = false;
}

Tile::~Tile()
{
}

void Tile::click(ALLEGRO_MOUSE_STATE _mouse, bool _isShift)
{
  if (_mouse.x >= m_posX && _mouse.x <= (m_posX + m_tileW)
    && _mouse.y - 65 >= m_posY && _mouse.y - 65 <= (m_posY + m_tileH))
  {
    if (_isShift)
    {
      if (_mouse.buttons & 1)
      {
        //A tile is clicked
        setTileType(START);
      }
      if (_mouse.buttons & 2)
      {
        //A tile is erased
        setTileType(END);
      }
    }
    else
    {
      if (_mouse.buttons & 1)
      {
        //A tile is clicked
        setTileType(OBSTACLE);
      }
      if (_mouse.buttons & 2)
      {
        //A tile is erased
        setTileType(FLOOR);
      }
    }
  } 
}

void Tile::renderTile()
{
  int _padding = 65;
  if (m_tileType == FLOOR)
  {
    if (m_isDark == true) { al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(231, 231, 240)); }
    else { al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(222, 222, 231)); }
  }
  else if (m_tileType == FLOOR && m_beenClosed == true)
  {
    if (m_isDark == true) { al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(200, 200, 255)); }
    else { al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(222, 222, 231)); }
  }
  else if (m_tileType == OBSTACLE)
  {
    if (m_isDark == true) { al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(131, 131, 140)); }
    else { al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(122, 122, 131)); }
  }
  else if (m_tileType == START)
  {
    al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(0, 170, 40));
  }
  else if (m_tileType == END)
  {
    al_draw_filled_rectangle(m_posX, _padding + m_posY, (m_posX + m_tileW), (_padding + m_posY + m_tileH), al_map_rgb(170, 0, 40));
  }
}