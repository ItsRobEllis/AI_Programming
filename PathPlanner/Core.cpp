#include "stdafx.h"
#include "Core.h"

// Static variable predefine
std::shared_ptr<Core> Core::s_instance = nullptr;

Core::Core()
{
  m_console = GetStdHandle(STD_OUTPUT_HANDLE);
  m_finished = false;

  for (int i = 0; i < 4; i++)
  {
    m_keys.push_back(false);
  }
  m_allowRendering = false;
  setFPS(60);
  setScreenW(1024);
  setScreenH(768);
  m_tilemap = nullptr;
  m_currentFrame = 0;
}

Core::~Core()
{
  m_keys.clear();
}

int Core::Launch(void)
{
  if (this->Init() == RET_SUCC)
  {
    if (this->Update() == RET_SUCC)
    {
      if (this->Exit() == RET_SUCC)
      {
        // Program Ends Gracefully
        return RET_SUCC;
      }
    }
  }
  // If the program didn't end gracefully, error
  return RET_ERROR;
}

int Core::Init()
{
  int width = 1024;
  int height = 768;

  SetConsoleTitle(TEXT("Debug Console"));	//Debug Console Window Title
  SetConsoleTextAttribute(getConsole(), 15); // Sets the colour of the console text to white, operating systems outside of Windows ignore this

  // Initialise Allegro
  logMsg(STANDARD, "Initialising Allegro5");
  if (!al_init()) {
    logMsg(SEVERE, "Failed to initialise Allegro");
    return RET_ERROR;
  }
  logMsg(STANDARD, "Allegro initialised successfully");

  //Installs
  logMsg(STANDARD, "Installing keyboard");
  al_install_keyboard();
  logMsg(STANDARD, "Installing mouse");
  al_install_mouse();
  logMsg(STANDARD, "Installing font addon");
  al_init_font_addon();
  logMsg(STANDARD, "Installing TTF addon");
  al_init_ttf_addon();
  logMsg(STANDARD, "Installing primitives addon");
  al_init_primitives_addon();

  //Create the tilemap
  m_tilemap = std::make_shared<Tilemap>();
  m_tilemap->create(64, 32, 22);

  //Create an instance of search
  m_pathfinder = std::make_shared<Search>();

  //Create a display
  m_display = al_create_display(getScreenW(), getScreenH());
  m_currentGameState = "Playing";
  m_currentTool = "Wall";

  // Set the window settings up
  al_set_window_title(m_display, "Rob's Path Planner");

  //Setup a queue and a timer to handle the events in the program
  logMsg(STANDARD, "Creating event queue");
  m_eventQueue = al_create_event_queue();
  logMsg(STANDARD, "Event queue created");

  logMsg(STANDARD, "Creating timer");
  m_alTimer = al_create_timer( 1.f / getFPS());
  logMsg(STANDARD, "Timer created");

  logMsg(STANDARD, "Installing font");
  m_font = al_load_font("arial.ttf", 12, 0);
  logMsg(STANDARD, "Font installed");

  al_register_event_source(m_eventQueue, al_get_display_event_source(m_display));
  al_register_event_source(m_eventQueue, al_get_timer_event_source(m_alTimer));
  al_register_event_source(m_eventQueue, al_get_keyboard_event_source());
  al_register_event_source(m_eventQueue, al_get_mouse_event_source());
  al_start_timer(m_alTimer);
  
  return RET_SUCC;
}

int Core::Update()
{
  while (!m_finished)
  {
    al_wait_for_event(m_eventQueue, &ev);

    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) //Monitor key presses
    {
      switch (ev.keyboard.keycode)
      {
        case ALLEGRO_KEY_ESCAPE:
        {
          m_finished = true;
          break;
        }
        case ALLEGRO_KEY_SPACE:
        {
          //RYANHERE
          if (m_tilemap->getStart() != nullptr && m_tilemap->getEnd() != nullptr)
          {
            std::vector<std::shared_ptr<Tile>> m_path = m_pathfinder->BreadthFirst(m_tilemap->getStart(), m_tilemap->getEnd(), m_tilemap);
            if (!m_path.empty())
            {
              for (int i = 0; i < m_path.size(); i++)
              {
                std::cout << m_path[i]->getIndX() << ", " << m_path[i]->getIndY() << std::endl;
              }
            }
            else
            {
              std::cout << "THE PATH IS EMPTY\n";
            }
          }        
          m_keys[SPACE] = true;
          break;
        }
        case ALLEGRO_KEY_LSHIFT:
        {
          m_currentTool = "Start/End";
          m_keys[LSHIFT] = true;
          break;
        }
      }
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) //Monitors key releases
    {
      if (ALLEGRO_KEY_LSHIFT)
      {
        m_currentTool = "Wall";
        m_keys[LSHIFT] = false;
      }
      // Do nothing
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //Monitors game window closing
    {
      m_finished = true;
      break;
    }
    else if (ev.type == ALLEGRO_EVENT_TIMER)
    {
      m_allowRendering = true;
      m_currentFrame++;

      al_get_mouse_state(&m_mouse);
      if (m_mouse.buttons & 1) //LMB
      {
        m_tilemap->click(m_mouse, m_keys[LSHIFT]);
      }
      else if (m_mouse.buttons & 2) //RMB
      {
        m_tilemap->click(m_mouse, m_keys[LSHIFT]);
      }
      else if (m_mouse.buttons & 4) //MMB
      {
        
      }

      //Seed the time
      srand(time(&randseed) * m_currentFrame);

      if (m_allowRendering && al_is_event_queue_empty(m_eventQueue))
      {
        //Rendering Functions
        m_allowRendering = false;
        m_tilemap->renderTiles();

        al_draw_filled_rectangle(0, 0, getScreenW(), 65, al_map_rgb(255, 255, 255));
        al_draw_line(0, 65, getScreenW(), 65, al_map_rgb(229, 229, 229), 2);

        // Draw Debug Diagnostics and UI tooltips
        al_draw_textf(m_font, al_map_rgb(0, 0, 0), 10, 10, 0, "Current Gamestate: %s", m_currentGameState.c_str());
        al_draw_textf(m_font, al_map_rgb(0, 0, 0), 10, 25, 0, "FPS: %i", getFPS());
        al_draw_textf(m_font, al_map_rgb(0, 0, 0), 10, 40, 0, "Frame: %i", m_currentFrame);
        al_draw_textf(m_font, al_map_rgb(0, 0, 0), getScreenW() - 10, 10, ALLEGRO_ALIGN_RIGHT, "Hit Esc to Exit");
        al_draw_textf(m_font, al_map_rgb(0, 0, 0), getScreenW() - 10, 40, ALLEGRO_ALIGN_RIGHT, "Current tool: %s", m_currentTool.c_str());

        //Update display
        al_flip_display();
        //al_clear_to_color(al_map_rgb((rand() % 255), (rand() % 255), (rand() % 255)));
        al_clear_to_color(al_map_rgb(255, 0, 255));
      }
    }
  }
  return RET_SUCC;
}

int Core::Exit()
{
  logMsg(STANDARD, "Game finished, cleaning up");

  //Install Keyboard
  logMsg(STANDARD, "Uninstalling keyboard");
  al_uninstall_keyboard();
  logMsg(STANDARD, "Uninstalling mouse");
  al_uninstall_mouse();

  // Clean up
  al_destroy_font(m_font);
  logMsg(STANDARD, "Destroyed fonts");
  al_destroy_timer(m_alTimer);
  logMsg(STANDARD, "Destroyed timer");
  al_destroy_event_queue(m_eventQueue);
  logMsg(STANDARD, "Destroyed event queue");
  al_destroy_display(m_display);
  logMsg(STANDARD, "Destroyed display");

  // This code only runs if the program is on a debug build
  // _getch pauses the program until a key is pressed
  #if _DEBUG
    SetConsoleTextAttribute(m_console, 8);
    std::cout << "Press any key to exit debug window...\n";
    _getch();
  #endif
 
  return RET_SUCC;
}

void Core::logMsg(TYPE msgType, std::string msgBody)
{
  time(&m_rawtime);
  m_timer = localtime(&m_rawtime);
  std::cout << "[" << std::setfill('0') << std::setw(2) << m_timer->tm_hour << ":" << std::setw(2) << m_timer->tm_min << ":" << std::setw(2) << m_timer->tm_sec << " ";
  switch (msgType)
  {
    case STANDARD:
    {
      std::cout << "DEBUG";
      break;
    }
    case WARN:
    {
      SetConsoleTextAttribute(m_console, 14);
      std::cout << "WARN";
      SetConsoleTextAttribute(m_console, 15);
      break;
    }
    case SEVERE:
    {
      SetConsoleTextAttribute(m_console, 12);
      std::cout << "SEVERE";
      SetConsoleTextAttribute(m_console, 15);
      break;
    }
    case UNDEFINED:
    {
      std::cout << "UNDEFINED";
      break;
    }
  }
  std::cout << "]: " << msgBody << std::endl;
}

// static functions
std::shared_ptr<Core> Core::GetCore()
{
	if (!s_instance)
	{
		s_instance = std::make_shared<Core>();
	}

	return s_instance;
}