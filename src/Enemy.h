#ifndef ENEMY_H
#define ENEMY_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
#include "Unit.h"

using namespace godot;

enum class EnemyAttackType {
    MELEE,
    MELEE_WEB,
    RANGED,
    RANGED_LINE
};

class Enemy : public Unit {
    GDCLASS(Enemy, Unit)

public:
    Enemy();
    ~Enemy();

    void _ready() override;
    void _process(double delta) override;
    void move_to_position(const Vector2& new_position);
    void check_valid_moves();

    // Implement the pure virtual function from Unit
    void take_turn() override;

    // Attack system
    EnemyAttackType attack_type;
    int attack_range;
    int attack_tiles;
    int attack_damage;
    bool applies_web;
    bool is_flying;
    bool is_leaper;

    void attack(Array target_positions);
    void take_damage(int damage);

    // Stored attack action
    Array planned_attack_targets;
    bool has_planned_attack;
    
    // Valid movement tiles array, this gets populated by check_valid_moves and can be used by the game manager to display valid movement options to the player when it's this enemy's turn
    Array valid_moves;
    
    // Getters and setters for properties to be exposed to Godot
    int get_attack_type() const { return static_cast<int>(attack_type); }
    void set_attack_type(int p_type) { attack_type = static_cast<EnemyAttackType>(p_type); }
    int get_attack_range() const { return attack_range; }
    void set_attack_range(int p_range) { attack_range = p_range; }
    int get_attack_tiles() const { return attack_tiles; }
    void set_attack_tiles(int p_tiles) { attack_tiles = p_tiles; }
    bool get_applies_web() const { return applies_web; }
    void set_applies_web(bool p_applies_web) { applies_web = p_applies_web; }
    int get_attack_damage() const { return attack_damage; }
    void set_attack_damage(int p_damage) { attack_damage = p_damage; }
    bool get_is_flying() const { return is_flying; }
    void set_is_flying(bool p_is_flying) { is_flying = p_is_flying; }
    bool get_is_leaper() const { return is_leaper; }
    void set_is_leaper(bool p_is_leaper) { is_leaper = p_is_leaper; }

protected:
    static void _bind_methods();

private:
    void _on_timer_timeout();
    godot::AnimatedSprite2D* animated_sprite = nullptr;
};

#endif // ENEMY_H