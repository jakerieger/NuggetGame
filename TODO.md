# TODO

### 3/27/24

- [x] Fixed timestep for physics simulation
- [ ] Debug renderer for physics bodies
- [x] Font rendering
- [x] Level class that can be easily serialized
- [ ] Correct shape collider for nugget (currently just a circle)
- [x] Parallelize input events so multiple key presses can be registered simultaneously
- [x] Script for building and copying release files so game builds can be easily deployed

### 4/4/24

- [x] Revamp camera class to follow player character

**Notes**:

~~Tilemaps can be handled by exporting the list of tile IDs from Tiled to a CSV format.
Read in the map width and height with origin 0,0 at bottom center of the screen.~~ **Completed.**

For collision, there are two options. The most efficient would be the use an edge collider and generate
positions seperate from the tilemap. The other option is to generate box colliders for each tile
which would be easier to implement but sacrifices performance a little bit.