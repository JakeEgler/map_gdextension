#include "Enemy.h"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <queue>
#include <set>
#include <limits>

Enemy::Enemy() : Unit(), attack_type(EnemyAttackType::MELEE), 
                 attack_range(1), attack_tiles(1), attack_damage(1), applies_web(false), 
                 is_flying(false), is_leaper(false), has_planned_attack(false) {
}

Enemy::~Enemy() {
}

void Enemy::_ready() {
    // Get reference to AnimatedSprite2D node
    animated_sprite = cast_to<AnimatedSprite2D>(get_node_or_null("AnimatedSprite2D"));
    if (animated_sprite) {
        animated_sprite->play("idle");
    }
}

void Enemy::_process(double delta) {
    // Play idle animation when not doing anything
    if (animated_sprite && !has_planned_attack) {
        if (animated_sprite->get_animation() != StringName("idle")) {
            animated_sprite->play("idle");
        }
    }
    
    if (has_planned_attack) {
        // Show attack preview/animation
        // Show projectile path, attack range highlight, etc.
    }
}

void Enemy::take_damage(int damage) {
    current_health -= damage;
    if (current_health <= 0) {
        current_health = 0;
        UtilityFunctions::print(get_name(), " has died!");
        queue_free();
    }
}

// This will eventually be in the game manager or something like that but for now i'm storing it here.
// if (enemy->has_planned_attack) {
//     enemy->attack(enemy->planned_attack_targets);
//     enemy->has_planned_attack = false;
// }

// Take_turn will be called by the game manager when it's this enemy's turn to act
//If an attack is queued up, it will execute the attack. If not, it will decide on an action to take, which could be moving or attacking.
// It will decide where to move and weather or not to attack, and will display where it will attack if it does decide to attack.
// It will then go to the player's turn after it finishes its actions.
void Enemy::take_turn() {
    //Decide where to move here
    check_valid_moves();
    if (!valid_moves.is_empty()) {
        // For now just move to the first valid position, but eventually this will be based on some pathfinding and decision making logic
        move_to_position(valid_moves[0]);
    }

    //Need to add a check if we want to attack or not here

    //if we do add it to the planned attack and display the attack range/area of effect
    planned_attack_targets = Array::make(Vector2(1,1)); //Placeholder for actual target selection logic
    has_planned_attack = true;

    //Then we pass control back to the player and when the player finishes their turn, the game manager will call take_turn again and execute the attack if has_planned_attack is true
}

void Enemy::_bind_methods() {
    // Only bind Enemy-specific methods - parent binding is automatic
    ClassDB::bind_method(D_METHOD("attack", "target_positions"), &Enemy::attack);
    ClassDB::bind_method(D_METHOD("take_turn"), &Enemy::take_turn);
    ClassDB::bind_method(D_METHOD("get_attack_type"), &Enemy::get_attack_type);
    ClassDB::bind_method(D_METHOD("set_attack_type", "p_type"), &Enemy::set_attack_type);
    ClassDB::bind_method(D_METHOD("get_attack_range"), &Enemy::get_attack_range);
    ClassDB::bind_method(D_METHOD("set_attack_range", "p_range"), &Enemy::set_attack_range);
    ClassDB::bind_method(D_METHOD("get_attack_tiles"), &Enemy::get_attack_tiles);
    ClassDB::bind_method(D_METHOD("set_attack_tiles", "p_tiles"), &Enemy::set_attack_tiles);
    ClassDB::bind_method(D_METHOD("get_applies_web"), &Enemy::get_applies_web);
    ClassDB::bind_method(D_METHOD("set_applies_web", "p_applies_web"), &Enemy::set_applies_web);
    ClassDB::bind_method(D_METHOD("get_attack_damage"), &Enemy::get_attack_damage);
    ClassDB::bind_method(D_METHOD("set_attack_damage", "p_damage"), &Enemy::set_attack_damage);
    ClassDB::bind_method(D_METHOD("get_is_flying"), &Enemy::get_is_flying);
    ClassDB::bind_method(D_METHOD("set_is_flying", "p_is_flying"), &Enemy::set_is_flying);
    ClassDB::bind_method(D_METHOD("get_is_leaper"), &Enemy::get_is_leaper);
    ClassDB::bind_method(D_METHOD("set_is_leaper", "p_is_leaper"), &Enemy::set_is_leaper);
    
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_range"), "set_attack_range", "get_attack_range");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_type", PROPERTY_HINT_ENUM, "Melee:0,Melee Web:1,Ranged:2,Ranged Line:3"), "set_attack_type", "get_attack_type");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_tiles"), "set_attack_tiles", "get_attack_tiles");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_damage"), "set_attack_damage", "get_attack_damage");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "applies_web"), "set_applies_web", "get_applies_web");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_flying"), "set_is_flying", "get_is_flying");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_leaper"), "set_is_leaper", "get_is_leaper");
}

void Enemy::attack(Array target_positions) {
    const char* attack_name = "Unknown";
    switch (attack_type) {
        case EnemyAttackType::MELEE:
            attack_name = "Melee";
            break;
        case EnemyAttackType::MELEE_WEB:
            attack_name = "Melee Web";
            break;
        case EnemyAttackType::RANGED:
            attack_name = "Ranged";
            break;
        case EnemyAttackType::RANGED_LINE:
            attack_name = "Ranged Line";
            break;
    }

    UtilityFunctions::print(get_name(), " uses ", attack_name, " attack on targets: ", target_positions);

    switch(attack_type) {
        case EnemyAttackType::MELEE:
            // Attack multiple tiles logic here
            break;
        case EnemyAttackType::MELEE_WEB:
            // Attack one tile and apply web effect
            break;
        case EnemyAttackType::RANGED:
            // Ranged attack logic here
            break;
        case EnemyAttackType::RANGED_LINE:
            // Ranged line attack logic here
            break;
    }
}

// Temporary movement function, will be replaced with correct movement 
void Enemy::move_to_position(const Vector2& new_position) {
    set_global_position(new_position);
    UtilityFunctions::print("Enemy "+ get_name() + " moved to position: " + String::num(new_position.x) + ", " + String::num(new_position.y));
}

void Enemy::check_valid_moves() {
    valid_moves.clear();
    
    // Get current grid position
    if (grid_position.size() < 2) {
        return; // Invalid position
    }
    
    Vector2 start_pos = Vector2(grid_position[0], grid_position[1]);
    
    if (is_leaper) {
        // Leaper movement: check all tiles within range, ignoring path
        // Use Manhattan distance for grid-based movement
        for (int dx = -movement; dx <= movement; dx++) {
            for (int dy = -movement; dy <= movement; dy++) {
                // Skip if beyond movement range (Manhattan distance)
                if (abs(dx) + abs(dy) > movement || (dx == 0 && dy == 0)) {
                    continue;
                }
                
                Vector2 target = start_pos + Vector2(dx, dy);
                
                // TODO: Replace with actual tilemap checking
                bool is_water = false; // Placeholder
                
                // TODO: Replace with actual occupancy checking (check if another unit is on this tile)
                bool is_occupied = false; // Placeholder
                
                // Check if destination tile is walkable
                bool can_land = true;
                if (is_water && !is_flying) {
                    can_land = false;
                }
                if (is_occupied) {
                    can_land = false;
                }
                
                if (can_land) {
                    valid_moves.append(target);
                }
            }
        }
    } else {
        // Normal movement: BFS to find all reachable tiles with pathfinding
        std::queue<Vector2> to_visit;
        std::set<std::pair<int, int>> visited;
        Dictionary distances; // Stores move cost to reach each tile
        
        to_visit.push(start_pos);
        visited.insert({(int)start_pos.x, (int)start_pos.y});
        distances[start_pos] = 0;
        
        // Directions: up, down, left, right
        Vector2 directions[] = {Vector2(0, -1), Vector2(0, 1), Vector2(-1, 0), Vector2(1, 0)};
        
        while (!to_visit.empty()) {
            Vector2 current = to_visit.front();
            to_visit.pop();
            
            int current_distance = distances[current];
            
            // Check all adjacent tiles
            for (int i = 0; i < 4; i++) {
                Vector2 next = current + directions[i];
                std::pair<int, int> next_pair = {(int)next.x, (int)next.y};
                
                // Skip if already visited
                if (visited.find(next_pair) != visited.end()) {
                    continue;
                }
                
                // TODO: Replace this with actual tilemap/grid checking
                bool is_water = false; // Placeholder - implement your tile checking here
                
                // TODO: Replace with actual occupancy checking (check if another unit is on this tile)
                bool is_occupied = false; // Placeholder
                
                // Check if tile is walkable
                bool can_walk = true;
                if (is_water && !is_flying) {
                    can_walk = false;
                }
                if (is_occupied) {
                    can_walk = false;
                }
                
                // Check if within movement range
                int new_distance = current_distance + 1;
                if (can_walk && new_distance <= movement) {
                    visited.insert(next_pair);
                    to_visit.push(next);
                    distances[next] = new_distance;
                    valid_moves.append(next);
                }
            }
        }
    }
}

void Enemy::_on_timer_timeout() {
    // Timer callback logic
}

