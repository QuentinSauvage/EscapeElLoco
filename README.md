# Escape El Loco
This platformer was made during my final year of master's degree. It is made using C++ and the SFML API and its goal is to keep the game running at 60fps by correctly managing the game loop.

# Controls
_←/→_ : Move on the X-axis.

_↑/↓_ : Interact with an object (chest, key, door and coin). Move up or down on ladders and ropes.

_Space bar_ : Jump

_A_ : Stop audio.

_R_ : Reset positions of the dragons (other properties such as collected keys will not change).

_H_ : Hardcore mode, if the game is too easy.

_Escape_ : Exit the game.

# Goals
Our four dragons, __Yellow__, __Green__, __Blue__ and __Red__, have been kept prisonners by the ruthless wizard __El Loco__. They now have to climb his infernal tower in order to escape this hell while their host tries to take control of their precious bodies.

# How the game works
Since the dragons are bonded by the dragon's pact, they move together. This means for instance that when _Yellow_ goes left, _Green_ goes left too, and so on.

They have to escape each rooms by opening the door relative to their side. To do that, they have to open a chest which will release a key. They finally have to collect this key in order to open the door which will allow them to set them free. This way, once each door has been opened, the level ends.

<details>
  <summary>Cheat</summary>
  
  If you find the game too difficult, press _G_ to enter __God Mode__. This will allow you to chain jumps and reach every places you want.
  
</details>

# Build & Run
```shell
make && ./bin/app
```

# Arts
Music: https://freesound.org/people/envirOmaniac2/sounds/376454/

Jump audio: https://freesound.org/people/OwlStorm/sounds/404793/

Chest audio: https://freesound.org/people/ssierra1202/sounds/391947/

Timer: The Legend of Zelda - Ocarina of Time (Nintendo)

Dino characters: https://arks.itch.io/dino-characters

Tileset: https://rottingpixels.itch.io/castle-platformer-tileset-16x16free
