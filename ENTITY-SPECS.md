## Movement

### Player
The player uses a **cardinal with diagonals** input system to move around the screen.
- When in diagonal movement, x and y vel increase (acceleration or dx/dy) are weighted to VelZ += 5
- When moving in a cardinal direction VelZ += 7
- There is a jolt in acceleration (dx *= 3) for the player when their velocity is under 2px/sec
- 

## Physics
Position is measured in pixels (px).
Velocity is measured in pixels per second (px / 60 frames). 
Velocity is limited to 60 px per sec.

The velocity is applied to the position every X frames, 
Where X = the inverse of velocity (times 60 frames / second to convert to frames)