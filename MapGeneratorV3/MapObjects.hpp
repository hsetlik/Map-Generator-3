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

const int factorAdjacent = 8;
const int factorWithin3 = 3;
const int recencyBias = 0;

class Tile {
public:
    Tile();
    ~Tile();
    void init(int xSet, int ySet);
    int x;
    int y;
    SDL_Texture * currentTexture;
    void setTexture(SDL_Texture * newTexture);
    bool isLand;
    int landWeight;
};
//declaring a global 2d vector of tiles for the map
class Map {
public:
    Map();
    ~Map();
    void loadAllTextures(SDL_Renderer *renderer);
    void updateAllTextures();
    void initTileVector();
    void initToOcean();
    void renderMap(SDL_Renderer *renderer);
    Tile* tileClicked();
};

class Landmass {
public:
    Landmass();
    ~Landmass();
    void initMap(Map chosenMap);
    Map map;
    void setupAllOcean();
    //tiles can be set& accessed either by coordinates or by tile pointer, functions with the same name do the same thing
    void setTile(Tile* tile, SDL_Texture* texture);
    void setTile(int xPos, int yPos, SDL_Texture* texture);
    void updateAdjacent(int xPos, int yPos);
    void updateAdjacent(Tile* centerTile);
    void updateOptionsFromLast(int xPos, int yPos);
    void updateOptionsFromLast(Tile* tile);
    void clicked();
    void cleanOptions();
    void updateLandWeights();
    int textureAdjacent(SDL_Texture *texture, Tile* tile);
    int textureWithinThree(SDL_Texture *texture, Tile* tile);
    void createLandmassUnweighted(int size); //note: one land tile must be placed before this function can be called
    void createLandmassWeighted(int size); //note: one land tile must be placed before this function can be called
    Tile* chooseOptionUnewighted();
    Tile* chooseOptionWeighted();
private:
    std::vector<Tile*> landTiles;
    std::vector<Tile*> waterTiles;
    std::vector<Tile*> grassTiles;
    std::vector<Tile*> sandTiles;
    std::vector<Tile*> optionTiles;
    std::vector<Tile*> lastAdjacents;
};



#endif /* MapObjects_hpp */
