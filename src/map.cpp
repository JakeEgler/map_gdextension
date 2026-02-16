#include "map.hpp"
#include "godot_cpp/classes/random_number_generator.hpp"
#include "godot_cpp/classes/tile_set_atlas_source.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/vector2i.hpp"
#include "tile.hpp"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <godot_cpp/classes/tile_set.hpp>
#include <godot_cpp/classes/tile_set_atlas_source.hpp>

using namespace godot;

Map::Map() {}

void Map::test_map() {
	UtilityFunctions::print("Testing map");
}

void Map::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_tile", "x", "y"), &Map::get_tile);
	ClassDB::bind_method(D_METHOD("set_map_size", "width", "height"), &Map::set_map_size);
	ClassDB::bind_method(D_METHOD("set_seed", "seed"), &Map::set_seed);
	ClassDB::bind_method(D_METHOD("test_map"), &Map::test_map);
	ClassDB::bind_method(D_METHOD("generate_map", "seed"), &Map::generate_map);
	ClassDB::bind_method(D_METHOD("update_tilemap"), &Map::update_tilemap);
}

void Map::set_map_size(int width, int height) {
	this->width = width;
	this->height = height;
	tiles.resize(width * height);
}

void Map::set_tile_size(int size) {
	tile_size = size;
}

void Map::create_tilemap() {
	UtilityFunctions::print("Creating new TileMapLayer");
	if (!tilemap) {
		// TileMapLayer
		tilemap = memnew(godot::TileMapLayer);
		tilemap->set_name("TileMapLayer");
		add_child(tilemap);

		// TileSet
		Ref<TileSet> tileset;
		tileset.instantiate();
		tilemap->set_tile_set(tileset);

		// Atlas
		Ref<TileSetAtlasSource> atlas;
		atlas.instantiate();

		Ref<Texture2D> texture = ResourceLoader::get_singleton()->load("res://assets/terrain_texture.png");
		atlas->set_texture(texture);

		atlas->set_texture_region_size(Vector2i(16, 16));

		int source_id = tileset->add_source(atlas);

		tilemap->set_tile_set(tileset);

		UtilityFunctions::print("TileMapLayer + TileSet created");
	}
}

void Map::set_seed(int seed) { this->seed = seed; }

void Map::set_tile(int x, int y, Tile *new_tile) {
	tiles.set(tile_index(x, y), new_tile);
}

Tile *Map::get_tile(int x, int y) const { return tiles[tile_index(x, y)]; }

void Map::generate_map(int seed) {
	UtilityFunctions::print("Generating map with seed: ", seed);

	Ref<RandomNumberGenerator> rng;
	rng.instantiate();
	rng->set_seed(seed);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (!tiles[tile_index(x, y)]) {
				//UtilityFunctions::print("Tile is null");
				continue;
			}

			tiles[tile_index(x, y)]->set_tile_id(tile_index(x, y));

			int r = rng->randi_range(0, 9);
			//UtilityFunctions::print("Random number: ", r);

			if (r < 2) {
				tiles[tile_index(x, y)]->set_tile_terrain(1);
				//UtilityFunctions::print("Generating: ", x, ",", y);
			}

			else if (r < 3) {
				tiles[tile_index(x, y)]
						->set_tile_terrain(2);
				//UtilityFunctions::print("Generating: ", x, ",", y);
			} else {
				tiles[tile_index(x, y)]->set_tile_terrain(0);
				int r = rng->randi_range(0, 63);
				if (r < 5) {
					//tiles[tile_index(x, y)]->set_tile_occupant(newmem(City)); // = "city";
				}

				else if (r < 15) {
					//tiles[tile_index(x, y)]->set_tile_occupant(newmem(Forest)); // = "forest";
				}
			}
			//UtilityFunctions::print("Generating: ", x, ",", y);
			//UtilityFunctions::print(tiles[tile_index(x, y)]->get_tile_id());
			//UtilityFunctions::print(tiles[tile_index(x, y)]->get_tile_terrain());
			//UtilityFunctions::print(tiles[tile_index(x, y)]->get_tile_occupant());
		}
	}
}

void Map::update_tilemap() {
	UtilityFunctions::print("Updating tilemap");
	if (!tilemap) {
		UtilityFunctions::print("Tilemap is null");
		return;
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile *tile = tiles[tile_index(x, y)];
			int terrain = tile->get_tile_terrain();

			Vector2i atlas_coords;

			if (terrain == 0) {
				atlas_coords = Vector2i(0, 0); // Land
			} else if (terrain == 1) {
				atlas_coords = Vector2i(1, 0); // Forest
			} else if (terrain == 2) {
				atlas_coords = Vector2i(2, 0); // Water
			}

			tilemap->set_cell(Vector2i(x, y), 0, atlas_coords, 0);

			// Position the Tile node at the tilemap cell's world position
			Vector2i cell_pos = Vector2i(x, y);
			Vector2 world_pos = tilemap->map_to_local(cell_pos);
			tile->set_position(world_pos);
		}
	}
}

void Map::_ready() {
	UtilityFunctions::print("Width: ", width);
	UtilityFunctions::print("Height: ", height);
	if (width <= 0 || height <= 0) {
		UtilityFunctions::print("Invalid map size");
		return;
	}

	create_tilemap();

	tiles.clear();
	tiles.resize(width * height);
	for (int i = 0; i < width * height; i++) {
		Tile *new_tile = memnew(Tile);
		new_tile->set_name("Tile_" + godot::UtilityFunctions::str(i));
		tiles.set(i, new_tile);
		tilemap->add_child(new_tile);
	}
	UtilityFunctions::print("tiles vector size: ", tiles.size());
	generate_map(0);
	update_tilemap();
}
