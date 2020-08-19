//
//  MapObjects.hpp
//  MapGeneratorV3
//
//  Created by Hayden on 8/19/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
//

#ifndef MapObjects_hpp
#define MapObjects_hpp
#include "/Library/Frameworks/SDL2.framework/Versions/A/Headers/SDL.h"
#include "/Library/Frameworks/SDL2_image.framework/Headers/SDL_image.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <random>
#include <stdio.h>

class Tile {
public:
    Tile();
    ~Tile();
    void init(int x, int y, int defaultTexture);
    int x;
    int y;
    int textureIndex;
    SDL_Texture * currentTexture;
    bool isLand;
    int landWeight;
};
//declaring a global 2d vector of tiles for the map
std::vector<std::vector<Tile>> workingTiles;

class Map {
    
};

#endif /* MapObjects_hpp */
