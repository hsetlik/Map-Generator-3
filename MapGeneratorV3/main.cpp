//
//  main.cpp
//  MapGeneratorV3
//
//  Created by Hayden on 8/19/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
//

#include "MapRenderer.hpp"

Display *windowDisplay= nullptr;
const int windowWidth = 1200;
const int windowHeight = 645;
const int FPS = 60;
const int frameTime = 1000/FPS;

Uint32 frameStart;
int frameDelay;

int main(int argc, const char * argv[]) {
    windowDisplay = new Display();
    //grid = new Grid();
    windowDisplay-> init("Map Generator", 600, 600, windowWidth, windowHeight, false);
    while(windowDisplay->running()){
        frameStart = SDL_GetTicks();
        windowDisplay->handleEvents();
        windowDisplay->update();
        windowDisplay->render();
        frameDelay = SDL_GetTicks() - frameStart;
        if(frameTime > frameDelay){
            SDL_Delay(frameTime-frameDelay);
        }
    }
    windowDisplay->clean();
    return 0;
}
