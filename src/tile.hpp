#pragma once

#include "godot_cpp/classes/node2d.hpp"

class Tile : public godot::Node2D {
	GDCLASS(Tile, Node2D)
private:
	int tile_id;
	int terrain = 0; // 0 = land, 1 = forest, 2 = water
	Object *occupied_by; // a pointer to whatever is in this spot. Includes
						 // mechs, enemies, cities, mountains

protected:
	static void _bind_methods();

public:
	Tile() {
		tile_id = 0;
		terrain = 0;
		occupied_by = nullptr;
	}

	void test_tile();
	int get_tile_id() const;
	int get_tile_terrain() const;
	Object *get_tile_occupant() const;
	void set_tile_id(const int id);
	void set_tile_terrain(const int terrain);
	void set_tile_occupant(Object *occupant);
};
