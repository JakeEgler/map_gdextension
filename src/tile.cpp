#include "tile.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/property_info.hpp"

using namespace godot;

void Tile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_tile_id"), &Tile::get_tile_id);
	ClassDB::bind_method(D_METHOD("get_tile_terrain"), &Tile::get_tile_terrain);
	ClassDB::bind_method(D_METHOD("get_tile_occupant"), &Tile::get_tile_occupant);
	ClassDB::bind_method(D_METHOD("set_tile_id", "id"), &Tile::set_tile_id);
	ClassDB::bind_method(D_METHOD("set_tile_terrain", "terrain"), &Tile::set_tile_terrain);
	ClassDB::bind_method(D_METHOD("set_tile_occupant", "occupant"), &Tile::set_tile_occupant);
	ClassDB::bind_method(D_METHOD("test_tile"), &Tile::test_tile);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_id"), "set_tile_id", "get_tile_id");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "terrain"), "set_tile_terrain", "get_tile_terrain");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "occupied_by"), "set_tile_occupant", "get_tile_occupant");
}

void Tile::test_tile() {
	print_line("Testing tile");
}
int Tile::get_tile_id() const { return tile_id; }
int Tile::get_tile_terrain() const { return terrain; }
Object *Tile::get_tile_occupant() const { return occupied_by; }
void Tile::set_tile_id(int id) { tile_id = id; }
void Tile::set_tile_terrain(int new_terrain) { terrain = new_terrain; }
void Tile::set_tile_occupant(Object *occupant) { occupied_by = occupant; }
