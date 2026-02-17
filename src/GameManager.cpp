#include "GameManager.h"
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <limits>

using namespace godot;

GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::_bind_methods() {
  // No custom methods to bind yet
}

void GameManager::_ready() {
  set_process_unhandled_input(true);

  // Find the map node (assumes it's a child of the GameManager)
  map = Object::cast_to<Map>(get_node_or_null("Map"));
  if (!map) {
    UtilityFunctions::print("Map node not found!");
    return;
  }

  // Find all Enemy units in the scene tree
  enemy_units.clear();
  Array all_nodes = get_tree()->get_nodes_in_group("enemies");
  for (int i = 0; i < all_nodes.size(); i++) {
    Enemy *enemy = Object::cast_to<Enemy>(all_nodes[i]);
    if (enemy) {
      enemy_units.append(enemy);
    }
  }

  place_enemies_on_random_tiles();
  place_players_on_random_tiles();
}

void GameManager::_unhandled_input(const Ref<InputEvent> &p_event) {
  Ref<InputEventKey> key_event = p_event;
  if (!key_event.is_valid()) {
    return;
  }

  if (key_event->is_pressed() && !key_event->is_echo() &&
      key_event->get_keycode() == KEY_P) {
    UtilityFunctions::print("=== P key pressed - Starting enemy turn ===");
    enemy_move_and_attack_nearest_player();
    get_viewport()->set_input_as_handled();
  }
}

void GameManager::place_players_on_random_tiles() {
  if (!map)
    return;
  Array player_nodes = get_tree()->get_nodes_in_group("player");
  for (int i = 0; i < player_nodes.size(); i++) {
    Unit *player = Object::cast_to<Unit>(player_nodes[i]);
    if (!player)
      continue;
    Tile *tile = get_random_valid_tile();
    if (!tile)
      continue;
    tile->set_tile_occupant(player);
    Vector2 target_pixel_pos = tile->get_position();
    player->set_pixel_position(target_pixel_pos);
    player->move_to_position(target_pixel_pos);
    // Calculate grid_position from pixel position
    const int TILE_SIZE = 16;
    Array grid_pos;
    grid_pos.resize(2);
    grid_pos[0] = (int)(target_pixel_pos.x / TILE_SIZE);
    grid_pos[1] = (int)(target_pixel_pos.y / TILE_SIZE);
    player->set_grid_position(grid_pos);
  }
}

void GameManager::place_enemies_on_random_tiles() {
  if (!map)
    return;
  Ref<RandomNumberGenerator> rng;
  rng.instantiate();
  for (int i = 0; i < enemy_units.size(); i++) {
    Enemy *enemy = Object::cast_to<Enemy>(enemy_units[i]);
    if (!enemy)
      continue;
    Tile *tile = get_random_valid_tile();
    if (!tile)
      continue;
    // Set tile occupant
    tile->set_tile_occupant(enemy);
    // Move enemy to tile's position
    Vector2 target_pixel_pos = tile->get_position();
    enemy->set_pixel_position(target_pixel_pos);
    enemy->move_to_position(target_pixel_pos);
    // Calculate grid_position from pixel position
    const int TILE_SIZE = 16;
    Array grid_pos;
    grid_pos.resize(2);
    grid_pos[0] = (int)(target_pixel_pos.x / TILE_SIZE);
    grid_pos[1] = (int)(target_pixel_pos.y / TILE_SIZE);
    enemy->set_grid_position(grid_pos);
  }
}

Unit *GameManager::find_nearest_player(const Vector2 &from_pos,
                                       const Array &player_nodes) const {
  Unit *nearest = nullptr;
  double best_dist_sq = std::numeric_limits<double>::max();

  for (int i = 0; i < player_nodes.size(); i++) {
    Unit *player = Object::cast_to<Unit>(player_nodes[i]);
    if (!player) {
      continue;
    }

    Vector2 player_pos = player->get_global_position();
    double dist_sq = from_pos.distance_squared_to(player_pos);
    if (dist_sq < best_dist_sq) {
      best_dist_sq = dist_sq;
      nearest = player;
    }
  }

  return nearest;
}

void GameManager::enemy_move_and_attack_nearest_player() {
  Array player_nodes = get_tree()->get_nodes_in_group("player");
  UtilityFunctions::print("Found ", player_nodes.size(),
                          " player(s) in 'player' group");
  if (player_nodes.is_empty()) {
    UtilityFunctions::print("No players found to attack.");
    return;
  }

  UtilityFunctions::print("Processing ", enemy_units.size(), " enemy unit(s)");
  for (int i = 0; i < enemy_units.size(); i++) {
    Enemy *enemy = Object::cast_to<Enemy>(enemy_units[i]);
    if (!enemy) {
      UtilityFunctions::print("Enemy ", i, " is null, skipping");
      continue;
    }

    UtilityFunctions::print("Processing enemy: ", enemy->get_name());
    Unit *target =
        find_nearest_player(enemy->get_global_position(), player_nodes);
    if (!target) {
      UtilityFunctions::print("  No target found for enemy ",
                              enemy->get_name());
      continue;
    }

    UtilityFunctions::print("  Target found: ", target->get_name());
    Vector2 target_pos = target->get_global_position();
    Tile *destination = find_nearest_open_tile_to(target_pos);
    if (!destination) {
      UtilityFunctions::print("  No valid destination tile found");
      continue;
    }

    UtilityFunctions::print("  Moving enemy to destination tile");
    clear_enemy_occupancy(enemy);
    destination->set_tile_occupant(enemy);

    Vector2 destination_pos = destination->get_position();
    enemy->set_pixel_position(destination_pos);
    enemy->move_to_position(destination_pos);

    // Update grid_position when enemy moves
    const int TILE_SIZE = 16;
    Array grid_pos;
    grid_pos.resize(2);
    grid_pos[0] = (int)(destination_pos.x / TILE_SIZE);
    grid_pos[1] = (int)(destination_pos.y / TILE_SIZE);
    enemy->set_grid_position(grid_pos);

    UtilityFunctions::print("  Executing attack on ", target->get_name(),
                            " for ", enemy->get_attack_damage(), " damage");
    enemy->attack(Array::make(target_pos));
    target->take_damage(enemy->get_attack_damage());
  }
  UtilityFunctions::print("=== Enemy turn complete ===");
}

Tile *GameManager::find_nearest_open_tile_to(const Vector2 &target_pos) const {
  if (!map) {
    return nullptr;
  }

  Vector2 target_local = map->to_local(target_pos);
  int target_x = static_cast<int>(Math::round(target_local.x / 16.0));
  int target_y = static_cast<int>(Math::round(target_local.y / 16.0));

  const int offsets[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  Tile *nearest = nullptr;
  double best_dist_sq = std::numeric_limits<double>::max();

  for (int i = 0; i < 4; i++) {
    int nx = target_x + offsets[i][0];
    int ny = target_y + offsets[i][1];
    if (nx < 0 || ny < 0 || nx >= 8 || ny >= 8) {
      continue;
    }

    Tile *tile = map->get_tile(nx, ny);
    if (!tile) {
      continue;
    }
    if (tile->get_tile_terrain() == 2) {
      continue;
    }
    if (tile->get_tile_occupant() != nullptr) {
      continue;
    }

    Vector2 tile_pos = tile->get_position();
    double dist_sq = tile_pos.distance_squared_to(target_pos);
    if (dist_sq < best_dist_sq) {
      best_dist_sq = dist_sq;
      nearest = tile;
    }
  }

  return nearest;
}

void GameManager::clear_enemy_occupancy(Enemy *enemy) {
  if (!map || !enemy) {
    return;
  }

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      Tile *tile = map->get_tile(x, y);
      if (!tile) {
        continue;
      }
      if (tile->get_tile_occupant() == enemy) {
        tile->set_tile_occupant(nullptr);
        return;
      }
    }
  }
}

Tile *GameManager::get_random_valid_tile() {
  if (!map)
    return nullptr;
  Array valid_tiles;
  // Collect all valid tiles (not occupied, not water)
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      Tile *tile = map->get_tile(x, y);
      if (!tile)
        continue;
      if (tile->get_tile_terrain() == 2)
        continue; // Skip water
      if (tile->get_tile_occupant() != nullptr)
        continue; // Skip occupied
      valid_tiles.append(tile);
    }
  }
  if (valid_tiles.is_empty())
    return nullptr;
  Ref<RandomNumberGenerator> rng;
  rng.instantiate();
  int idx = rng->randi_range(0, valid_tiles.size() - 1);
  return Object::cast_to<Tile>(valid_tiles[idx]);
}
