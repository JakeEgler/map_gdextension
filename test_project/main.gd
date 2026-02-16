extends Node2D

func _ready() -> void:
	var map:Map = Map.new()
	map.test_map()
	
	var tile:Tile = Tile.new()
	tile.test_tile()

	#map.generate_map(67)
	#map.update_tilemap()
