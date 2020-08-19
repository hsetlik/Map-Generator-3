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

const int width = 80;
const int height = 43;

class Tile {
public:
    Tile();
    ~Tile();
    void init(int xSet, int ySet, int defaultTexture);
    int x;
    int y;
    int textureIndex;
    SDL_Texture * currentTexture;
    bool isLand;
    int landWeight;
};
//declaring a global 2d vector of tiles for the map


class Map {
public:
    Map();
    ~Map();
    SDL_Texture * grass;
    SDL_Texture * sand;
    SDL_Texture * water;
    void loadAllTextures(SDL_Renderer *renderer);
    void initTileVector();
    void initToOcean();
    void renderMap(SDL_Renderer *renderer);
};

class Landmass {
public:
    Landmass();
    ~Landmass();
    std::vector<std::unique_ptr<Tile>> waterTiles;
    std::vector<std::unique_ptr<Tile>> grassTiles;
    std::vector<std::unique_ptr<Tile>> sandTiles;
    std::vector<std::unique_ptr<Tile>> optionTiles;
    std::vector<std::unique_ptr<Tile>> lastAdjacents;
    void initAllOcean();
    void setTile(int xPos, int yPos);
    void updateAdjacent(int xPos, int yPos);
    void updateOptionsFromLast(int xPos, int yPos);
    void updateLandWeight(std::unique_ptr<Tile>);
    
};



#endif /* MapObjects_hpp */
