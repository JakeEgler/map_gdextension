#pragma once

#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/tile_map.hpp"

#include "godot_cpp/classes/tile_map_layer.hpp"
#include "tile.hpp"

class Map : public godot::Node2D {
	GDCLASS(Map, godot::Node2D)

private:
	int width = 8;
	int height = 8;
	int seed = 0;
	int tile_size = 16; // Size of each tile in pixels
	godot::Vector<Tile *> tiles;
	godot::TileMapLayer *tilemap = nullptr;
	int tile_index(int x, int y) const { return y * width + x; };
	void set_tile(int x, int y, Tile *new_tile);
	void create_tilemap();

protected:
	static void _bind_methods();

public:
	Map();
	void generate_map(int seed);
	void update_tilemap();
	void test_map();
	void _ready();
	Tile *get_tile(int x, int y) const;
	void set_map_size(int width, int height);
	void set_seed(int seed);
	void set_tile_size(int size);
};
