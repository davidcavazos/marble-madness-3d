marble-madness-3d
=================

Remake of Marble Madness in 3D

Dependencies in Arch Linux:
pacman -S boost sdl

Currently in its initial development.
Only opens an SDL window, close it with ESC.
Very primitive input system implemented.


Controls:
w:      move up
a:      move left
s:      move down
d:      move right
p:      pause
SPACE:  move faster while pressed
ESC:    menu

Object {
    list commands
    void registerCommand(string command, void function(void));
}

Game : Object {
    void togglePause();
    void showMenu();
}

GameState : Object {
    void loadWelcomeMenu();
    void loadMainMenu();
    void loadLevel1();
    void loadLevel2();
    void loadLevel3();
    void loadLevel4();
    void loadLevel5();
    void loadWinScreen();
    void loadLoseScreen();
    void loadGameOverScreen();
    void loadHighScoresScreen();
    void loadCreditsScreen();
}

Timer : Object {
    void reset();
    void start();
    void stop();
}

Entity : Object {
    string name;
    uint id;
    Entity* parent
    list components
    list children
}

abstract Actor : Entity {
    void moveUp();
    void moveLeft();
    void moveDown();
    void moveRight();
}

Marble : Actor {
    void setMoveSpeedFast();
    void setMoveSpeedNormal();
    void breakToPieces();
    void dissolveInAcid();
}

GreenSlug : Actor {
    void jump();
}

AcidPool : Actor {
}

Bird : Actor {
}

abstract Trap {
    void activate();
    void deactivate();
}

VacuumCleaner : Trap {
}

Bopper : Trap {
}



Game entities:

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

http://www.neoseeker.com/resourcelink.html?rlid=114160&rid=106293