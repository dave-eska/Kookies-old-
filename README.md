# Kookies 

## State : Still Very Early On Development. Probably have a lot of _un-diagnosed_ bugs

## Features : 
    
* Tiles or Items or Defined in * res/items *. This Made Players Changing or Adding New Items Without Re-Compiling Possible.

* Maps are also defined inside res/maps. ( But I haven't Found A Way To Load Different Maps WIthout Re-Compiling ).

* Entitiy is a base class and is easily extendable (Easy to make new type of entity).

* Tiles:
    
    * Seed Tile -> Can grow, Can be picked up.

    * Transition Tile -> Can change the current level.

* Entity:

    * Cat -> Can follow you with x amount of speed

    * NPC -> Have the ability to make conversation with NPC. The text can be edited without re-compiling.

## Hint For Modders : 

* Tile ID's : 

```cpp
enum TileID {
    Air_Tile = 0,

    Grass_Tile = 1,
    Farmland_Tile,
    Brickwall_Tile,
    Floor_Tile,

    Doorup_Tile = 5,
    Doordown_Tile,

    Chest_Tile = 7,
    Craftingtable_Tile,

    Transition_Tile = 9,
    Itemarea_Tile,
    PlaceArea_Tile,

    Chocolate_Tile = 12,
    Chocolatecake_Tile,
    Cherry_Tile,
    Milk_Tile,
    Cookie_Tile,
    Cocoabeans_Tile,

    Emptybag_Tile = 18,
    Bagofcherry_Tile,
    Bagofcocoabeans_Tile,

    Cherryseeds_Tile = 21,
    Cocoabeansseed_Tile,

    ... (tile.h)
};
```

## Known Bug : 

    * Sword is somehow placeable and doesn't have any texture when placed.

    * Hoe gets replaced with fruit when harvesting.
