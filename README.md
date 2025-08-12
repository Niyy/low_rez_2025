# Useful Commands
Command to build this project on the laptop.
`cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER="C:\ProgramData\mingw64\mingw64\bin\gcc.exe" -DCMAKE_CXX_COMPILER="C:\ProgramData\mingw64\mingw64\bin\g++.exe" -DCMAKE_MAKE_PROGRAM="C:\ProgramData\mingw64\mingw64\bin\mingw32-make.exe" -DCMAKE_RC_COMPILER="C:\\ProgramData\\mingw64\\mingw64\\bin\\windres.exe"`

# Goal
Puzzle city builder where you take on members of The Order of the Hammer and Shield. Defenders of the lands left unattended and undefended. You will be tasked with preparing border towns for upcoming assults from the shadows. A threat will begin to muster somewhere on the map. You can choose to go offensive, defensive, or even a little bit of both.

# Things I need to do
1. Build the tiled world.
    > it will need to have path blocking elements.
    > it will need to have removable elemenst.
    > it will need to have spawn points.
    > only 2d map sadly.
2. Create actors. They are the back bone of your defense.
    > the main actors I will create are the members of the order and enemies.
    > they will need to fight each other.
    > they will need to be able to interact with the world.
    > they will need to build.
    > they will need to be able to gather.
    > they will need to be able to travers the world.
