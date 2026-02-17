#ifndef UG_HF_6_BACKEND_SRC_UNIT_H
#define UG_HF_6_BACKEND_SRC_UNIT_H

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class Unit : public Sprite2D {
	GDCLASS(Unit, Sprite2D);

protected:
	static void _bind_methods();

	int movement;
	Array grid_position;
	Vector2 pixel_position;
	int max_health;
	int current_health;
	int attack_power;
	int weight;
	bool has_acted;
	String unit_name;

	// Animation
	double animation_timer;
	double animation_speed;
	int current_frame_index;

public:
	Unit();
	virtual ~Unit();

	virtual void _process(double delta) override;

	// Getters and Setters
	void set_movement(int p_movement);
	int get_movement() const;

	void set_grid_position(const Array &p_position);
	Array get_grid_position() const;

	void set_pixel_position(const Vector2 &p_position);
	Vector2 get_pixel_position() const;

 void move_to_position(const Vector2& p_position);

	void set_max_health(int p_health);
	int get_max_health() const;

	void set_current_health(int p_health);
	int get_current_health() const;

	void set_attack_power(int p_power);
	int get_attack_power() const;

	void set_weight(int p_weight);
	int get_weight() const;

	void set_has_acted(bool p_acted);
	bool get_has_acted() const;

	void set_name(const String &p_name);
	String get_name() const;

	// Abstract method for turn logic
	virtual void take_turn() = 0;

	virtual void take_damage(int p_amount);
};

} // namespace godot

#endif // UG_HF_6_BACKEND_SRC_UNIT_H
