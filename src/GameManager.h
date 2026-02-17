#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/array.hpp>
#include "map.hpp"
#include "Enemy.h"

using namespace godot;

class GameManager : public Node2D {
    GDCLASS(GameManager, Node2D)

protected:
    static void _bind_methods();

public:
    GameManager();
    ~GameManager();
    void _ready() override;
    void _unhandled_input(const Ref<InputEvent> &p_event) override;

private:
    Map* map = nullptr;
    Array enemy_units;
    void place_enemies_on_random_tiles();
    void place_players_on_random_tiles();
    void enemy_move_and_attack_nearest_player();
    Unit* find_nearest_player(const Vector2 &from_pos, const Array &player_nodes) const;
    Tile* find_nearest_open_tile_to(const Vector2 &target_pos) const;
    void clear_enemy_occupancy(Enemy* enemy);
    Tile* get_random_valid_tile();
};

#endif // GAME_MANAGER_H
