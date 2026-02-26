# Backgammon
A command line backgammon game, written in C, developed with CLion/CMake.
## Requirements
- Windows
- gcc
- CMake 4.1 or higher
## Installation and build
1. Clone the repository: git clone https://github.com/DamiEbn/Backgammon.git
2. Go to folder: cd Backgammon
3. Run CMake: 
   - mkdir build
   - cd build
   - cmake ..
   - cmake --build .
4. Run the game: Backgammon.exe
## Known issues
- Only tested on Windows
- Using arrow keys during name input causes name to not be recognized correctly (e.g. "Laura" ≠ "Laura")
- Two-decimal-digit stone counts in a single field shift the entire board layout
- players[x].rolled_out and players[x].thrown_out are never printed
## Not implemented
- Doubling cube
- Resigning a game
- Undoing a move
## License
MIT