
## Project 5

My exigence a la project-5 was the restrictiveness of other arduboy games when shooting in action. Because there is no joystick, they tend to either shoot in the direction of the movement (which is limited to UPDOWNLEFTRIGHT) OR they do the same in some other system. 

I don't think that it would be distasteful to remove the aiming aspect of shooting. Think Brawl Stars, where you can tap on the attack joystick to shoot in the direction of the nearest enemy. That game design is satisfying, and it could definitely be ported to the arduboy.

* Possibly, down the road I could add an aiming ability as an option, but not the default.

### Step one: make a satisfying movement system for enemies and the player

- [x] take input
- [x] include velocity
- [x] fine tune the drag and speed
- [x] automate enemies to directional movement

### Step two: organize the functionality into a world-entity sytem

- [ ] combine struct for entities that does shared functionality between player and enemy
- [ ] create world array with handler
- [ ] define pointer to player, with seperate functionality in the player codeblock
- [ ] seperate enemy functionality 

### Step three: shooting mechanic & health system

### Step four: game loop (high score system?)
