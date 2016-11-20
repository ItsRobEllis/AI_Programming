// AI Programming Y2 Assignment 1 - Path Planning
// By Rob Ellis
// Bournemouth University

#ifndef _CORE_H_
#define _CORE_H_

class Search;
class Tile;
class Tilemap;
class Core
{
public:
	Core();
	~Core();
  //Debug Console Logging
  enum RETURNS { RET_SUCC, RET_ERROR };
  enum TYPE { UNDEFINED, STANDARD, WARN, SEVERE };
  HANDLE getConsole() { return m_console; }
  time_t getRawTime() { return m_rawtime; }

  //Application Variables
  bool m_finished;
  bool m_allowRendering;

  //Keyboard Buttons
  std::vector<bool> m_keys;
  enum KEYS { SPACE, LSHIFT };

  //Allegro
  ALLEGRO_EVENT_QUEUE *m_eventQueue;
  ALLEGRO_TIMER *m_alTimer;
  ALLEGRO_DISPLAY *m_display;
  ALLEGRO_FONT *m_font;
  ALLEGRO_FONT *m_fontsmall;

  //Functions
  void logMsg(TYPE msgType, std::string msgBody);
  int Launch(void);
  int Init(void);
  int Update();
  int Exit(void);

  //Time Handling
  time_t m_rawtime;
  struct tm *m_timer;

  //Getters and Setters
  void setFPS(int _fps) { m_fps = _fps; };
  int getFPS() { return m_fps; }

  //Gamestate handling
  const enum GAMESTATE { MENU, DRAW, RPG };
  void setGameState(GAMESTATE _state) { m_gameState = _state; };
  GAMESTATE getGameState(void) { return m_gameState; };

  //Screen
  void setScreenW(int _w) { m_screenW = _w; };
  void setScreenH(int _h) { m_screenH = _h; };
  int getScreenW(void) { return m_screenW; };
  int getScreenH(void) { return m_screenH; };
  
  //Update functions
  time_t randseed;
  std::shared_ptr<Tilemap> m_tilemap;
  ALLEGRO_EVENT ev;
  int m_currentFrame;
  std::string m_currentGameState;
  std::string m_currentTool;

  //Search functions
  std::shared_ptr<Search> m_pathfinder;
  std::vector<std::shared_ptr<Tile>> m_path;
  
  //Mouse Variables
  ALLEGRO_MOUSE_STATE m_mouse;

  // Static function
  static std::shared_ptr<Core> GetCore();

private:
	// Private constructors
	
  //Debug Console Logging
  HANDLE m_console;
  int m_fps;
  GAMESTATE m_gameState;

  int m_screenW, m_screenH;

  int test;

  // Static stuff
  static std::shared_ptr<Core> s_instance;
};

#endif