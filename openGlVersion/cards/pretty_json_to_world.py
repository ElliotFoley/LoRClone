import json
import itertools
from pathlib import Path

# Mapping from CardType to FLECS CardType strings
CARDTYPE_TO_FLECS = {
    "Unit": "CARDTYPE_UNIT",
    "Spell": "CARDTYPE_SPELL",
    "Artifact": "CARDTYPE_ARTIFACT"
}

def card_to_flecs(card_key, card, uid, index):
    return {
        "name": f"#{uid}",
        "tags": ["components.CardTag"],
        "components": {
            "components.Zone": {
                "zone": "ZONE_DECK"
            },
            "components.ManaCost": {
                "manaCost": card.get("manaCost", 0)
            },
            "components.Attack": {
                "attack": card.get("attack", 0)
            },
            "components.Health": {
                "health": card.get("health", 0)
            },
            "components.CardType": {
                "type": CARDTYPE_TO_FLECS.get(card.get("CardType"), "CARDTYPE_UNKNOWN")
            },
            "components.Owner": {
                "playerId": 0
            },
            "components.Name": {
                "name": card.get("name", card_key)
            },
            "components.ArtPath": {
                "artPath": card.get("artPath", "")
            },
            "components.EffectText": {
                "effectText": card.get("effectText", "")
            },
            "components.Index": {
                "index": index
            },
            "components.Position": {
                "x": 0,
                "y": 0
            },
            "components.Size": {
                "width": 0,
                "height": 0
            },
            "components.Rotation": {
                "angle": 0
            },
            "components.Render": {
                "shaderProgram": 3,
                "vao": 1,
                "texture": 1
            }
        }
    }


if __name__ == "__main__":
    with open("cards_pretty.json", "r") as f:
        cards = json.load(f)

    uid_gen = itertools.count(600)   # unique IDs starting from 600
    index_gen = itertools.count(0)   # sequential index
    
    out_dir = Path("./cards")  # current directory (could make a "cards" subfolder if you want)

    for card_key, card in cards.items():
        uid = next(uid_gen)
        index = next(index_gen)
        flecs_card = card_to_flecs(card_key, card, uid, index)

        out_path = out_dir / f"{card_key}.json"
        with open(out_path, "w") as f:
            json.dump(flecs_card, f, indent=2)

        print(f"Wrote {out_path}")
