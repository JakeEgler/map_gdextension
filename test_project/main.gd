extends Node2D

@onready var map := $Map

func _ready():
	print("=== Map Test Start ===")

	# Generate map
	map.generate_map(randi())

	# Test get_tile
	var rand_x = randi() % 8
	var rand_y = randi() % 8
	var tile = map.get_tile(rand_x, rand_y)

	if tile == null:
		print("Tile is NULL")
		return

	print("Tile ID: ", tile.get_tile_id())
	print("Tile Terrain: ", tile.get_tile_terrain())
	print("Tile Occupant: ", tile.get_tile_occupant())

	print("=== Map Test End ===")
