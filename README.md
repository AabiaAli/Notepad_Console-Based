# Console Notepad (C++)
## Overview
This project implements a basic text editor in the Windows console using C++. It supports typing, file loading and saving, undo/redo, word search, and autocomplete functionality. It is built using 2-D doubly linked list and N-ary trees without using any arrays, strings, or STL containers.

## Features
- Text Editing: Type characters into the editor with support for multiple lines.
- Cursor Movement: Move the cursor using arrow keys.
- Undo/Redo: Press Tab to undo and Shift to redo recent changes.
- Search: Press 1 during editing to search for a word.
- Autocomplete: Press 9 to show suggestions based on previously typed words. Then press keys 1–9 to insert a suggestion.
- File Handling: Load (2) and save (3) files using full pathnames.
- New File: Start a new blank file with option 1.
- Exit: Exit using option 4 or press ESC in the editor to return to menu.

## Editor Instructions
### Controls:
- Arrow keys — Move cursor
- Enter — Add new line
- Backspace — Delete character
- Tab — Undo
- Shift — Redo
- 1 — Search
- 9 — Show autocomplete suggestions
- 1-9 — Select a suggestion
- ESC — Exit editing and return to menu

### Menu Options:
- New File
- Load File
- Save File
- Exit

## Text Interface
The editor is purely console-based and uses ASCII text for display. There are no graphics or GUI windows.

## Build Instructions
- Windows OS required
- Compile using any standard C++ compiler with WinAPI support
- Example using g++:
g++ -o notepad main.cpp Notepad.cpp -lstdc++fs

## License
This project is free to use and modify. Made for learning and educational purposes.
