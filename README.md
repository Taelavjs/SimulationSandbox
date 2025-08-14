# FallingSandsSumoSimulation

A falling sands sandbox with physics-based character, dirty-rect optimization, Terrain generation from noise maps and Object Oriented Principled design. Currently there are 3 types of pixels :

https://github.com/user-attachments/assets/7d3be67e-d518-457d-b8d8-1b5df02a79d8

1. Sand(yellow) - Can fall vertically OR horizontally - if in free fall, gains velocity as it falls
2. Water(blue) - Can fall vertically OR horizontally - if in free fall, gains velocity as it falls
3. Gas(Light gray) - Rises vertically, through solids and liquids. Can rise with velocity.
4. Oil(dark grey if unlit, red-gray if on fire) - same as water, but sinks below water due to density
5. Napalm(red) - Sets oil on fire and continues to burn until it turns to smoke. Acts as a liquid and floats with its density.

The green rectangles are the dirty rect optimization algorithm updating. Any squares without a green rect are not being updated. 




