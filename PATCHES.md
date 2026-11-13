# Changes from upstream

This is the Asciiverse packaging of [Cataclysm](https://github.com/Whales/Cataclysm),
Whales' post-apocalyptic roguelike (2010 to 2013), licensed under the
Creative Commons Attribution-ShareAlike 3.0 Unported License (`LICENSE`,
added by Whales on 2013-03-27). Its community fork became Cataclysm: Dark
Days Ahead; this is the original.

Upstream is the `upstream` remote. The `asciiverse` branch starts from
upstream's `master` at `f9b2b24` (2022-02-01, a README update; the last
code is `0062bb2` of 2013-01-17, "Last commit before I wipe my hard
drive"), with its full history.

## Making it build and run

`e722627`, one commit. Whales' last commit left work in progress, and the
2012 code predates 64-bit and today's compilers:

- `map.cpp`: an unfinished faster `map::sees()` ("WIP") broke off in the
  middle of a statement; removed. The working `map::sees()` after it is
  unchanged.
- `item.cpp`: `food->health` had been renamed `healthy` in `itype.h` but
  not here.
- `iuse.cpp`: the new vitamins item called `add_msg()` without `g->`.
- `calendar.h`, `calendar.cpp`: `calendar::operator=` takes a const
  reference, so `game_message` can be assigned again.
- `setvector.cpp`: string pointers were read through `va_arg` as `int`,
  which cuts them in half on 64-bit; the game crashed at start. They are
  read as `char *` now.
- Non-void functions that fell off their end return now
  (`mapbuffer::add_submap`, `pcomp`, `invert_color`,
  `game::pl_refill_vehicle`, `vehicle::fire_turret_internal`).
  `add_submap` smashed the stack on the first new game at `-O1` and up.

## Ours

- **Folders** (`main.cpp`, not on Windows): when the game's data is
  beside the program (an installed copy), it plays from the player's own
  folder: `$XDG_STATE_HOME/cataclysm` (usually `~/.local/state/cataclysm`),
  or `~/Library/Application Support/cataclysm` on macOS. The shipped data
  files (`motd`, `LAB_NOTES`, `NAMES_*`, `NPC_HINTS`) are copied there at
  every start; `options.txt`, `keymap.txt`, character templates and
  `save/` are the player's and are kept. Run from the source folder, it
  uses the current folder as before.
- `main.cpp`: no `system("clear")` at exit; ncurses restores the screen.
- `monster.cpp`: a debug view wrote up to 11 bytes into `char buff[2]`.

Not added: session logging (the game has no single input point to hook;
left for a later release).

## Packaging (new files)

- `asciiverse/build.sh`: builds with upstream's Makefile, compiler, flags
  and curses from the environment.
- `asciiverse/package.sh`: the av archive: `cataclysm`, `data/` (the six
  read-only files), `LICENSE`, `README.txt`, `README.upstream`. Left out:
  `data/termfont` and `data/FONTDATA`, used only by upstream's own Windows
  window.
- `asciiverse/README.txt`: how to play it.
- `asciiverse/record.sh`: records `asciiverse/cataclysm.cast`, the 100x30
  catalog preview, with hub's castrec.
- `.github/workflows/release.yml`: static musl Linux (x86-64, ARM64) in
  Alpine and macOS (x86-64, ARM64), on a pushed tag. No Windows: upstream's
  Windows build draws in a window of its own (`catacurse.cpp`).
- `.gitignore`.
