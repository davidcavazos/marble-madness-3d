marble-madness-3d
=================

Remake of Marble Madness in 3D

Controls: (not implemented yet, but this is the plan)
w:      move up
a:      move left
s:      move down
d:      move right
p:      pause
SPACE:  move faster while pressed
ESC:    menu

**** Installation ****
NOTE: the following are the only OSs I have compiled the project

>> Arch Linux
1. Make sure development software is installed
pacman -S gcc make cmake git

2. Install dependencies
pacman -S boost sdl mesa bullet assimp

3. Download from GitHub repository
Navigate to the folder the source code will be downloaded
git clone git://github.com/davido262/marble-madness-3d.git

4. Compile and install project
Navigate to the folder where the build will be compiled (preferably outside the source)
cmake git-source-folder -G "Unix Makefiles"
make
Run the executable under src/


>> openSUSE
1. Make sure development software is installed
From YaST check the following: gcc-g++ make cmake-gui git

2. Install dependencies
From YaST check the following: boost-devel sdl-devel Mesa-devel

2.1 Install Bullet Physics
Navigate to the folder where the build will be compiled (preferably outside the source)
cmake ../bullet-trunk-rev2600 -G "Unix Makefiles" -DBUILD_EXTRAS=off -DBUILD_DEMOS=off -DINSTALL_LIBS=on
make
sudo make install

2.2 Install Open Asset Import Library
Navigate to the folder where the build will be compiled (preferably outside the source)
cmake ../assimp--3.0.1270-source-only -G "Unix Makefiles" -DBUILD_ASSIMP_SAMPLES=off
make
sudo make install

3. Download from GitHub repository
Navigate to the folder the source code will be downloaded
git clone git://github.com/davido262/marble-madness-3d.git

4. Compile and install project
Navigate to the folder where the build will be compiled (preferably outside the source)
ccmake git-source-folder -G "Unix Makefiles"
Press [c] to configure
Press [g] to generate and exit
make
Run the executable under src/


>> Windows
1. Make sure development software is installed
2. Install dependencies
2.1 Install Bullet Physics
"C:\Program Files\CMake 2.8\bin\cmake.exe" ..\bullet-trunk-rev2600 -G "Visual Studio 10" -DBUILD_EXTRAS=off -DBUILD_DEMOS=off -DINSTALL_LIBS=on
2.2 Install Open Asset Import Library
3. Download from GitHub repository
4. Compile and install project


**** Commands ****
NOTE: not all commands and/or objects are implemented yet

entity
    set position
    set position-{x,y,z}
    set rotation
    set rotation-{x,y,z}
    set scale
    set scale-{x,y,z}
    move-{x,y,z}
    rotate-{x,y,z}
    scale-{x,y,z}

player (entity)

camera (entity)

game
    set score
    add-to-score
    toggle-pause
    show-menu

gamestate
    load-welcome-menu
    load-main-menu
    load-level-1
    load-level-2
    load-level-3
    load-level-4
    load-level-5
    load-win-screen
    load-lose-screen
    load-game-over-screen
    load-high-scores-screen
    load-credits-screen

timer
    reset
    start
    stop

**** Game entities ****
NOTE: this is not final

Marble
    player
    AI

GreenSlug
AcidPool
VacuumCleaner
Bopper (hammers)
Bird

Bridge (rises and lowers at an interval)
DownChute (fall through hole and get down)
DoubleDownChute (same, with two entrances)
UpChute
WaveFloor (regular sine-line waves in the ground)
Catapult
GroundPiston
Teletransporter
MovingFloor (appears and disappears at an interval)

Heightmap
    color theme
    height
    type (flat, angled)

Decoration
    sidebars
    arrows
    decals
    spikes
    flags
    goal

GUI
    score
    time
    race name

for more information on the game:
http://www.neoseeker.com/resourcelink.html?rlid=114160&rid=106293