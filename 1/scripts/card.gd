extends Control

@export var mana_cost: int
@export var card_name: String
@export var art_path: String
@export var rarity: String
@export var effect_text: String
@export var health: int
@export var attack: int

func setup(card_data: Dictionary) -> void:
	mana_cost = card_data.mana_cost
	card_name = card_data.name
	art_path = card_data.art
	rarity = card_data.rarity
	effect_text = card_data.effect_text
	health = card_data.health
	attack = card_data.attack
	
	$NameLabel.text = name
	$AttackLabel.text = str(attack)
	$HealthLabel.text = str(health)
	$ManaLabel.text = str(mana_cost)
	$Art.texture = load(art_path)
