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

void Tile::init(int xSet, int ySet, int defaultTexture){
    x = xSet;
    y = ySet;
    textureIndex = defaultTexture;
    isLand = false;
    landWeight = 0;
}

std::vector<std::vector<Tile>> visibleTiles;

Map::Map(){
}
Map::~Map(){
}
void Map::loadAllTextures(SDL_Renderer *renderer){
    SDL_Surface * tempSurface1 = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/TopDownScroller/Tiles/Tile_0000_Ocean_Default.png");
    sand = SDL_CreateTextureFromSurface(renderer, tempSurface1);
    SDL_FreeSurface(tempSurface1);
    SDL_Surface * tempSurface2 = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/TopDownScroller/Tiles/Tile_0001_Grass_Default.png");
    grass = SDL_CreateTextureFromSurface(renderer, tempSurface2);
    SDL_FreeSurface(tempSurface2);
    SDL_Surface * tempSurface3 = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/TopDownScroller/Tiles/Tile_0002_Sand_Default.png");
    water = SDL_CreateTextureFromSurface(renderer, tempSurface3);
    SDL_FreeSurface(tempSurface3);
}

void Map::initTileVector(){
     for(int x = 0; x < width; x++){
            std::vector<Tile> temp;
            for(int y = 0; y < height; ++y){
                Tile thisTile;
                thisTile.init(x, y, 0);
                temp.push_back(thisTile);
                }
            visibleTiles.push_back(temp);
    }
    
    
}

void Map::initToOcean(){
    for(int x = 0; x < width; ++x){
        for(int y = 0; y < height; ++y){
            //zero is the index representing the ocean texture
            visibleTiles[x][y].init(x, y, 0);
        }
    }
}


void Map::renderMap(SDL_Renderer *renderer){
    for(int x = 0; x < width; ++x){
        for(int y = 0; y < height; ++y){
            int texIndex = visibleTiles[x][y].textureIndex;
            switch(texIndex){
                case 0:{
                    visibleTiles[x][y].currentTexture = water;
                }
                case 1:{
                    visibleTiles[x][y].currentTexture = grass;
                }
                case 2: {
                    visibleTiles[x][y].currentTexture = sand;
                }
            }
            SDL_Rect destRect;
            destRect.w = 15;
            destRect.h = 15;
            destRect.x = 15 * x;
            destRect.y = 15 * y;
            SDL_RenderCopy(renderer, visibleTiles[x][y].currentTexture, NULL, &destRect);
        }
    }
}
