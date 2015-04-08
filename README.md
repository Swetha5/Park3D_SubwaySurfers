Our game has 2 stages. We can select a stage by pressing 1 or 2 on the terminal.

First stage:

The world consists of a bike,terrain and few fossils.The rider should collect fossils to get points within "120 seconds".

CONTROLS:
1)Bike can turn left by 'a' key and right by 'd' key
2)One can accelerate bike by pressing up key and apply brakes by down key
3)Right and Left key tilt the bike and turn it left and right when it is in speed.
4)Views can be switched by pressing 'v' button.
5)Press 'ESC' key to exit game.  
Bike has constant acceleration on pressing up key and deaccelarates when key is left.
Bike can tilt(by max 45 degree) and will be balanced by centrifugal force.
The following camera views are implemented:
    a)Driver View
    b)Wheel View
    c)OverHead View
    d)Side View
    e)Follow Cam


Second stage::
The world consists of an human(3D model),hurdles on tracks  and fossils .The  human should collect the fossils without touching the hurdles to get points within 120 seconds. When he encounters a hurdle he can either jump or slide. If he collides with  the hurdle. the game is over


CONTROLS::

1)w - walk mode
2)s - slide mode
3)z - move forward
4)spacebar - to jump
5)j - shift to left track
6)k - shift to right track
7)esc - to quit the interface 


The following camera views are implemented:

1)f - front view
2)b - back view

Lighting effects implemented::

1)daylight
2)night 
3)sunrise

They vary with time

Shadding effects ::

1)terrain
2)human


Textures ::


How to run:
*g++ stry.cpp
* Now type './a.out' (on terminal) to run the executable. {linux}
* To play stage 1 - enter 1
* To play stage 2 - enter 2

Techniques ::

For creating the terrain, Bitmap Representation of Terrain and Elevation Data

Translating bitmap values into usable data is straightforward, assuming you can read the file format of the bitmap. For a given entry (x, y) in a height data bitmap, a corresponding value in the height field can be calculated by taking the value in the bitmap and multiplying it by some scalar: heightField (x, y) = bitmap (x, y) · scaleZ. Using the terrain bitmap is even easier. Since the bitmap is 8-bit, simply set the value (palette index) at any given point (x, y) in the image to a predetermined terrain type. If more than 256 terrain types are needed, you can use the RGB values of a 24-bit image for terrain type indexing instead. 

Take each pixel (x, y) in the height bitmap and duplicates the pixel value bitmap (x, y) inside a rectangular area of pixels scaleX · scaleY in size, in which the upper left corner of the rectangle equals (x · scaleX, y · scaleY). Empirically, the data becomes "pixellated," as though the bitmap is viewed at a higher zoom level. The terrain data is scaled in this way, as well. 

3D Models
We used blender objects as 3D models, using md2files and bmp files. 

Textures:

Texture mapping allows to glue an image of a brick wall (obtained, perhaps, by scanning in a photograph of a real wall) to a polygon and to draw the entire wall as a single polygon. Texture mapping ensures that all the right things happen as the polygon is transformed and rendered. A texture can be applied to a surface in different ways. It can be painted on directly (like a decal placed on a surface), used to modulate the color the surface would have been painted otherwise, or used to blend a texture color with the surface color. 

To use texture mapping, we performed these steps.

Create a texture object and specify a texture for that object.
Indicate how the texture is to be applied to each pixel.
Enable texture mapping.
Draw the scene, supplying both texture and geometric coordinates.


Texture coordinates specify the point in the texture image that will correspond to the vertex you are specifying them for.

