# hex_editor

A minimal, dependency-free terminal hex viewer and editor written in C using POSIX `termios`, `ioctl`, and ANSI escape sequences.

## Build

Requires `gcc` (or a C17-compatible compiler), `make`, and a POSIX-compliant terminal (Linux, macOS, BSD).

```sh
make
```

To remove the compiled binary:

```sh
make clean
```

## Usage

```sh
./hex_editor <filename>
```

Minimum terminal size: **77 columns** by **3 rows** (1 data row + 2 status/mode rows). The display automatically redraws on window resize (`SIGWINCH`).

## Keybindings

| Key | Action |
| :--- | :--- |
| `h` / `j` / `k` / `l` | Move cursor left / down / up / right |
| `0` / `$` | Jump to start / end of current 16-byte row |
| `g` / `G` | Jump to first / last row of file |
| `H` / `M` / `L` | Jump to top / middle / bottom of visible viewport |
| `Ctrl-F` / `Ctrl-B` | Scroll down / up one full page |
| `Ctrl-D` / `Ctrl-U` | Scroll down / up half a page |
| `q` | Quit |

## Project Structure

- `Makefile` — Build configuration (`-std=c17 -Wall -Wextra -Wpedantic`).
- `src/main.c` — Event loop, `SIGWINCH` signal setup, and key dispatch.
- `src/editor.h` / `src/editor.c` — Editor state, cursor movement, and viewport scrolling.
- `src/display.h` / `src/display.c` — Hex/ASCII grid rendering, status bar, and mode bar.
- `src/terminal.h` / `src/terminal.c` — Raw terminal mode (`termios`), alternate screen buffer, window size queries (`TIOCGWINSZ`), and unbuffered key reads.
- `src/file.h` / `src/file.c` — File loading and bounds-checked memory allocation.

## Roadmap

- Multi-key sequences (`gg`) and numeric count prefixes (e.g., `12j`, `5Ctrl-D`)
- Hex/ASCII pane toggling (`Tab`)
- Overwrite mode and saving changes to disk