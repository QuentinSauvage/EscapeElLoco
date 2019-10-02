#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include <SFML/Graphics.hpp>
#include "gameLogic.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
	public:
		sf::VertexArray verticesArray;
		sf::Texture tileset;

		TileMap();

		bool load(GameLogic::Map map);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	    {
	        states.transform *= getTransform();
			states.texture = &tileset;
	        target.draw(verticesArray, states);
	    }

		~TileMap();
};

#endif