extends Node2D

@onready var player1_hand_node = $Player1Hand
@onready var card_scene = preload("res://Card.tscn")
@export var cards_json_path: String = "res://cards.json"

var all_cards_data: Array = []

func _ready():
	load_all_cards()
	fill_hand_for_player(player1_hand_node, 5)

func load_all_cards():
	var file = FileAccess.open(cards_json_path, FileAccess.READ)
	var json_string = file.get_as_text()
	all_cards_data = JSON.parse_string(json_string)
	file.close()

func fill_hand_for_player(hand_node: Node, count: int):
	for i in range(min(count, all_cards_data.size())):
		var card_data = all_cards_data[i]
		var card_instance = card_scene.instantiate()
		card_instance.setup(card_data)
		hand_node.add_child(card_instance)
		
		# Positioning - simple horizontal layout
		card_instance.position = Vector2(i * 120, 0)
