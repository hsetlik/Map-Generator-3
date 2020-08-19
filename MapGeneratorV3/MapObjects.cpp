//
//  MapObjects.cpp
//  MapGeneratorV3
//
//  Created by Hayden on 8/19/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
//

#include "MapObjects.hpp"

Tile::Tile(){
}
Tile::~Tile(){
}

void Tile::init(int xSet, int ySet){
    x = xSet;
    y = ySet;
    isLand = false;
    landWeight = 0;
}

void Tile:: setTexture(SDL_Texture *newTexture){
    currentTexture = newTexture;
}

std::vector<std::vector<Tile>> visibleTiles;

SDL_Texture * grass;
SDL_Texture * sand;
SDL_Texture * water;



Map::Map(){
}
Map::~Map(){
}
void Map::loadAllTextures(SDL_Renderer *renderer){
    SDL_Surface * tempSurface1 = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/MapGeneratorV3/MapGeneratorV3/PNG assets/sandBase.png");
    sand = SDL_CreateTextureFromSurface(renderer, tempSurface1);
    SDL_FreeSurface(tempSurface1);
    SDL_Surface * tempSurface2 = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/MapGeneratorV3/MapGeneratorV3/PNG assets/grassBase.png");
    grass = SDL_CreateTextureFromSurface(renderer, tempSurface2);
    SDL_FreeSurface(tempSurface2);
    SDL_Surface * tempSurface3 = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/MapGeneratorV3/MapGeneratorV3/PNG assets/waterBase.png");
    water = SDL_CreateTextureFromSurface(renderer, tempSurface3);
    SDL_FreeSurface(tempSurface3);
}

void Map::initTileVector(){
     for(int x = 0; x < width; x++){
            std::vector<Tile> temp;
            for(int y = 0; y < height; ++y){
                Tile thisTile;
                thisTile.init(x, y);
                temp.push_back(thisTile);
                }
            visibleTiles.push_back(temp);
    }
}

void Map::initToOcean(){
    printf("map initializing to ocean. . . ");
    for(int x = 0; x < width; ++x){
        for(int y = 0; y < height; ++y){
            //zero is the index representing the ocean texture
            
            visibleTiles[x][y].setTexture(water);
        }
    }
    printf("finished\n");
}



void Map::renderMap(SDL_Renderer *renderer){
    //updateAllTextures();
    for(int x = 0; x < width; ++x){
        for(int y = 0; y < height; ++y){
            SDL_Rect destRect;
            destRect.w = 15;
            destRect.h = 15;
            destRect.x = 15 * x;
            destRect.y = 15 * y;
            SDL_RenderCopy(renderer, visibleTiles[x][y].currentTexture, NULL, &destRect);
        }
    }
}

Tile* Map::tileClicked(){
    double _clickX;
    double _clickY;
    int xClickPx;
    int yClickPx;
    SDL_GetMouseState(&xClickPx, &yClickPx);
    _clickX = xClickPx / 15;
    _clickY = yClickPx / 15;
    int clickX = round(_clickX);
    int clickY = round(_clickY);
    printf("Tile clicked at: %d, %d\n", clickX, clickY);
    return &visibleTiles[clickX][clickY];
}

Landmass::Landmass(){
    
}
Landmass::~Landmass(){
    
}
void Landmass::initMap(Map chosenMap){
    map = chosenMap;
    numMembers = 0;
    numOptions = 0;
    map.initToOcean();
}

void Landmass::setupAllOcean(){
    for(int x = 0; x < width; ++x){
        for(int y = 0; y < height; ++y){
            if(visibleTiles[x][y].currentTexture == water){
                waterTiles.push_back(&visibleTiles[x][y]);
            }
        }
    }
}

void Landmass::setTile(int xPos, int yPos, SDL_Texture* texture){ //coordinate version
    visibleTiles[xPos][yPos].setTexture(texture);
    visibleTiles[xPos][yPos].isLand = true;
    if(texture == grass){
        printf("Grass at: %d, %d\n", xPos, yPos);
    }
}

void Landmass::clicked(){
    Tile* clickedTile = map.tileClicked();
    setTile(clickedTile, grass);
    
}

void Landmass:: setTile(Tile* tile, SDL_Texture* texture){ //pointer version
    int x = tile->x;
    int y = tile->y;
    visibleTiles[x][y].setTexture(texture);
}
void Landmass::updateAdjacent(Tile *centerTile){
    //gets rid of the 4 pointers stored in the vector after its last use
    lastAdjacents.clear();
    int x = centerTile->x;
    int y = centerTile->y;
    if(y > 0){
    lastAdjacents.push_back(&visibleTiles[x][y - 1]);//top
    }
    if(x < 79){
    lastAdjacents.push_back(&visibleTiles[x + 1][y]);// right
    }
    if(y < 42){
    lastAdjacents.push_back(&visibleTiles[x][y + 1]);//bottom
    }
    if(x > 0){
    lastAdjacents.push_back(&visibleTiles[x - 1][y]);
    }
}

void Landmass::updateAdjacent(int xPos, int yPos){
    //gets rid of the 4 pointers stored in the vector after its last use
    lastAdjacents.clear();
    int x = xPos;
    int y = yPos;
    if(y > 0){
    lastAdjacents.push_back(&visibleTiles[x][y - 1]);//top
    }
    if(x < 79){
    lastAdjacents.push_back(&visibleTiles[x + 1][y]);// right
    }
    if(y < 42){
    lastAdjacents.push_back(&visibleTiles[x][y + 1]);//bottom
    }
    if(x > 0){
    lastAdjacents.push_back(&visibleTiles[x - 1][y]);
    }
}
void Landmass::cleanOptions(){
    for(int i = 0; i < numOptions; ++i){
        if(optionTiles[i]->isLand){
            optionTiles.erase(optionTiles.begin() + i);
            --numOptions;
        }
    }
}

void Landmass::updateOptionsFromLast(Tile *tile){
    updateAdjacent(tile);
    std::vector<Tile*> allowed;
    for(int i = 0; i < lastAdjacents.size(); ++i){
        bool allowed = true;
        if(lastAdjacents[i]->isLand){
            allowed = false;
        }
        for(int n = 0; n < optionTiles.size(); ++n){
            if(optionTiles[n] == lastAdjacents[i]){
                allowed = false;
            }
        }
        if(allowed){
            optionTiles.push_back(lastAdjacents[i]);
        }
    }
    cleanOptions();
}

void Landmass::updateOptionsFromLast(int xPos, int yPos){
    Tile* tile = &visibleTiles[xPos][yPos];
    updateAdjacent(tile);
    for(int i = 0; i < lastAdjacents.size(); ++i){
        bool isAllowed = true;
        if(lastAdjacents[i]->isLand){
            isAllowed = false;
        }
        for(int n = 0; n < optionTiles.size(); ++n){
            if(optionTiles[n] == lastAdjacents[i]){
                isAllowed = false;
            }
        }
        if(isAllowed){
            optionTiles.push_back(lastAdjacents[i]);
        }
    }
    cleanOptions();
}

