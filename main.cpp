/* Main Loop for cataclysm
 * Linux only I guess
 * But maybe not
 * Who knows
 */

#if (defined _WIN32 || defined WINDOWS)
	#include "catacurse.h"
#else
	#include <curses.h>
#endif

#include <ctime>
#include "game.h"
#include "color.h"
#include "options.h"
#include "mapbuffer.h"

#if !(defined _WIN32 || defined WINDOWS)
#include <cstdlib>
#include <string>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

// Asciiverse: the game reads data/ and writes data/, save/ and a few files
// in the current folder. When its read-only data sits beside the program
// (an installed copy), run it from the player's own folder instead:
// $XDG_STATE_HOME/cataclysm (~/.local/state/cataclysm), or
// ~/Library/Application Support/cataclysm on macOS. The shipped data files
// are copied there at every start; the player's own (options.txt,
// keymap.txt, character templates) are left alone.
static std::string exe_dir()
{
 char buf[PATH_MAX];
#ifdef __APPLE__
 uint32_t size = sizeof(buf);
 if (_NSGetExecutablePath(buf, &size) != 0)
  return "";
 char real[PATH_MAX];
 if (!realpath(buf, real))
  return "";
 std::string p(real);
#else
 ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
 if (n <= 0)
  return "";
 buf[n] = 0;
 std::string p(buf);
#endif
 size_t slash = p.rfind('/');
 return slash == std::string::npos ? "" : p.substr(0, slash);
}

static void mkdirs(const std::string &path)
{
 for (size_t i = 1; i <= path.size(); i++) {
  if (i == path.size() || path[i] == '/')
   mkdir(path.substr(0, i).c_str(), 0755);
 }
}

static void use_state_folder()
{
 std::string from = exe_dir();
 if (from.empty())
  return;
 from += "/data";
 std::string shipped[] = { "motd", "LAB_NOTES", "NAMES_FEMALE", "NAMES_LAST",
                           "NAMES_MALE", "NPC_HINTS" };
 std::ifstream probe((from + "/motd").c_str());
 if (!probe.good())
  return; // run from the source folder: keep the current folder
 probe.close();
 const char *home = getenv("HOME");
 std::string state;
#ifdef __APPLE__
 if (home)
  state = std::string(home) + "/Library/Application Support/cataclysm";
#else
 const char *xdg = getenv("XDG_STATE_HOME");
 if (xdg && *xdg)
  state = std::string(xdg) + "/cataclysm";
 else if (home)
  state = std::string(home) + "/.local/state/cataclysm";
#endif
 if (state.empty())
  return;
 mkdirs(state + "/data");
 mkdirs(state + "/save");
 for (int i = 0; i < 6; i++) {
  std::ifstream in((from + "/" + shipped[i]).c_str(), std::ios::binary);
  if (!in.good())
   continue;
  std::ofstream out((state + "/data/" + shipped[i]).c_str(), std::ios::binary);
  out << in.rdbuf();
 }
 if (chdir(state.c_str()) != 0)
  return;
}
#endif

int main(int argc, char *argv[])
{
#if !(defined _WIN32 || defined WINDOWS)
 use_state_folder();
#endif
 srand(time(NULL));

// ncurses stuff
 initscr(); // Initialize ncurses
 noecho();  // Don't echo keypresses
 cbreak();  // C-style breaks (e.g. ^C to SIGINT)
 keypad(stdscr, true); // Numpad is numbers
 init_colors(); // See color.cpp
 curs_set(0); // Invisible cursor

 rand();  // For some reason a call to rand() seems to be necessary to avoid
          // repetion.
 bool quit_game = false;
 game *g = new game;
 MAPBUFFER = mapbuffer(g);
 MAPBUFFER.load();
 load_options();
 do {
  g->setup();
  while (!g->do_turn());
  if (g->game_quit())
   quit_game = true;
 } while (!quit_game);
 MAPBUFFER.save();
 erase(); // Clear screen
 endwin(); // End ncurses
 return 0;
}

