//
//  MapRenderer.hpp
//  MapGeneratorV3
//
//  Created by Hayden on 8/19/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
//

#ifndef GameGlass_hpp
#define GameGlass_hpp
#include "MapObjects.hpp"


class Display{
public:
    Display();
    ~Display();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    //handles user inputs and whatever the hell else
    void handleEvents();
    //updates all game objects
    void update();
    //draws appropriate graphics based on updated game logic
    void render();
    //manages memory
    void clean();
    bool running() {return isRunning;}
    
private:
    int _redlevel = 255;
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
};


#endif /* GameGlass_hpp */
