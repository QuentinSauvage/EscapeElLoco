#include "tileMap.hpp"
#include <iostream>

using namespace std;
using namespace sf;

TileMap::TileMap() {}

bool TileMap::load(GameLogic::Map map) {
	if (!tileset.loadFromFile("sprites/background.png"))
    //    return false;

	verticesArray.setPrimitiveType(Quads);
	verticesArray.resize(map.height * map.width * 4);

	for(int i = 0; i < map.width; i++) {
		for(int j = 0; j < map.height; j++) {
			int tileNumber = map.map[i][j];

			int tileU = tileNumber % (tileset.getSize().x / 128);
			int tileV = tileNumber / (tileset.getSize().x / 128);

			Vertex *quad = &verticesArray[(i + j * map.width) * 4];

			//define quad corners clockwise
			quad[0].position = Vector2f(i * 128, j * 128);
			quad[1].position = Vector2f((i+1) * 128, j * 128);
			quad[2].position = Vector2f((i+1) * 128, (j+1) * 128);
			quad[3].position = Vector2f(i * 128, (j+1) * 128);

			//define quad texture coordinates clockwise
			quad[0].texCoords = Vector2f(tileU * 128, tileV * 128);
			quad[1].texCoords = Vector2f((tileU + 1) * 128, tileV * 128);
			quad[2].texCoords = Vector2f((tileU + 1) * 128, (tileV + 1) * 128);
			quad[3].texCoords = Vector2f(tileU * 128, (tileV + 1) * 128);
		}
	}
	return true;
}


TileMap::~TileMap() {}