//
//  MapObjects.cpp
//  MapGeneratorV3
//
//  Created by Hayden on 8/19/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
//

#include "MapObjects.hpp"

std::random_device device;
std::mt19937 generator(device());


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
SDL_Texture * option;



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
    
    SDL_Surface * tempSurface4 = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/MapGeneratorV3/MapGeneratorV3/PNG assets/waterBaseOption.png");
    option = SDL_CreateTextureFromSurface(renderer, tempSurface4);
    SDL_FreeSurface(tempSurface4);
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
    firstPlaced = false;
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



void Landmass::clicked(){
    Tile* clickedTile = map.tileClicked();
    setTile(clickedTile, grass);
    updateOptionsFromLast(clickedTile);
    createLandmassUnweighted(10);
}


void Landmass::clickedWeighted(){
    Tile* clickedTile = map.tileClicked();
    setTile(clickedTile, grass);
    updateOptionsFromLast(clickedTile);
    updateLandWeights();
    createLandmassWeighted(25);
}

void Landmass:: setTile(Tile* tile, SDL_Texture* texture){ //pointer version
    int x = tile->x;
    int y = tile->y;
    visibleTiles[x][y].setTexture(texture);
    visibleTiles[x][y].isLand = true;
    std::uniform_int_distribution<> offsetRange(0, 3);
    int randomValue = offsetRange(generator);
    if(firstPlaced == false){
        std::uniform_int_distribution<> indexRange(0, 3);
        updateAdjacent(tile);
        int randomIndex = indexRange(generator);
        lastAdjacents[randomIndex]->landWeight += randomValue;
        firstPlaced = true;
    }
    
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
    for(int i = 0; i < optionTiles.size(); ++i){
        if(optionTiles[i]->isLand){
            optionTiles.erase(optionTiles.begin() + i);
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
            //this just highlights the tiles being considered as options at a given point
            if(optionTiles.back()->currentTexture == water){
            optionTiles.back()->currentTexture = option;
            }
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

int Landmass::textureAdjacent(SDL_Texture *texture, Tile *tile){
    updateAdjacent(tile);
    int toCheck = (int)lastAdjacents.size();
    int landNeighbors =0;
    for(int i = 0; i < toCheck; ++i){
        if(lastAdjacents[i]->currentTexture == texture){
            landNeighbors++;
        }
    }
    printf("%d adjacent land tiles\n", landNeighbors);
    return landNeighbors;
}

int Landmass::textureWithinThree(SDL_Texture *texture, Tile *tile){
    int landInThree = 0;
    updateAdjacent(tile);
    int rounds = (int)lastAdjacents.size();
    for(int n = 0; n < rounds; ++n){
        updateAdjacent(lastAdjacents[n]);
        int toCheck = (int)lastAdjacents.size();
        for(int i = 0; i < toCheck; ++i){
            if(lastAdjacents[i]->currentTexture == texture){
               landInThree++;
            }
        }
    }
    printf("%d land tiles within three\n", landInThree);
    return landInThree;
}

void Landmass::updateLandWeights(){
    for(int i = 0; i < optionTiles.size(); ++i){
        int numAdjacent = textureAdjacent(grass, optionTiles[i]);
        int numInThree = textureWithinThree(grass, optionTiles[i]);
        //numAdjacent += textureAdjacent(sand, optionTiles[i]);
        //numInThree += textureWithinThree(sand, optionTiles[i]);
        double adjacentWeight = pow(factorAdjacent, (numAdjacent + 1));
        double inThreeWeight = pow(factorWithin3, (numInThree + 1));
        int weight = adjacentWeight + inThreeWeight;
        optionTiles[i]->landWeight = weight;
        //printf("Option %d, %d weight is %d\n", optionTiles[i]->x, optionTiles[i]->y, weight);
    }
}

Tile* Landmass::chooseOptionUnweighted(){
    int indexChoice;
    int ceiling = (int)optionTiles.size();
    Tile* chosenTile;
    std::uniform_int_distribution<int> Range(recencyBias, (ceiling - 1));
    //printf("Cieling : %d\n", ceiling);
    indexChoice = Range(generator);
    chosenTile = optionTiles[indexChoice];
    return chosenTile;
}

void Landmass::createLandmassUnweighted(int size){
    for(int i = 0; i < size; ++i){
        Tile * choiceTile = chooseOptionUnweighted();
        setTile(choiceTile, grass);
        updateOptionsFromLast(choiceTile);
    }
}

void Landmass::randomizeWeights(int maxOffset){
    std::uniform_int_distribution<> offsetRange(0, maxOffset);
    for(int i = 0; i < optionTiles.size(); ++i){
        int randOffset = offsetRange(generator);
        optionTiles[i]->landWeight += randOffset;
    }
}

Tile* Landmass::chooseOptionWeighted(){
    Tile* tileChoice;
    int weightSum = 0;
    for(int i =0; i < optionTiles.size(); ++i){
        int tempWeight = optionTiles[i]->landWeight;
        weightSum+= tempWeight;
    }
    printf("sum of weights: %d\n", weightSum);
    tileChoice = nullptr;
    std::uniform_int_distribution<int> weightedRange(recencyBias, weightSum);
    int random = weightedRange(generator);
    printf("Starting random number: %d\n", random);
    for(int i = 0; i < optionTiles.size(); ++i){
        if(random < optionTiles[i]->landWeight){
            tileChoice = optionTiles[i];
        } else {
            random -= optionTiles[i]->landWeight;
        }
    }
    return tileChoice;
}
void Landmass::createLandmassWeighted(int size){
    for(int i = 0; i < 5; ++i){
        Tile * choiceTile = chooseOptionUnweighted();
        setTile(choiceTile, grass);
        updateOptionsFromLast(choiceTile);
    }
    updateLandWeights();
    randomizeWeights(noiseAmount);
    for(int n = 0; n < (size - 5); ++n){
    Tile* choiceTile = chooseOptionWeighted();
    printf("Choice weight: %d\n", choiceTile->landWeight);
    printf("Placing tile at %d, %d\n", choiceTile-> x , choiceTile-> y);
    setTile(choiceTile, grass);
    updateOptionsFromLast(choiceTile);
    updateLandWeights();
    randomizeWeights((n / 3) * noiseAmount);
    }
}
