# Ethiopian Flag & Nike logo  - openGL transformations Demo

A small OpenGL project that renders two animated objects side by side: 
        Ethiopian flag (with a waving effect)  
        Nike swoosh 
      Both objects can be translated, rotated, and scaled independently using the keyboard.


## What it looks like

The screen  is split in to two sections. 
    .The flag is on the left
    .The logo is on the right. 

The flag uses a sine-wave animation so the stripes look like they're moving in the wind.
The star in the center is drawn manually using thick line geometry and miter joins to keep the corners clean.

The swoosh is generated from connected cubic Bézier curves instead of using textures or image assets.


## Getting it running

You need **freeglut** (or any GLUT-compatible library) and a C++ compiler that supports OpenGL.

**On Ubuntu / Debian:**
  ```bash
sudo apt install freeglut3-dev
g++ main.cpp -o demo -lGL -lGLU -lglut
./demo  
```


**On macOS (with Homebrew):**
```bash
brew install freeglut
g++ main.cpp -o demo -framework OpenGL -framework GLUT
./demo
 ```


**On Windows:**  
Add the freeglut include and lib paths to your project , link against `freeglut.lib` and `opengl32.lib`, and build normally.
For a full  setup tutorial, see:  https://www.youtube.com/watch?v=-vmkNGwlKEo&t=335step-by-step



## Controls

| Key | What it does |
|-----|-------------|
| `TAB` | Switch between the flag and the swoosh |
| `W / S` | Move up / down |
| `A / D` | Move left / right |
| `E / C` | Rotate clockwise / counter-clockwise |
| `+ / -` | Scale up / down |
| `R` | Reset everything back to default |
| `Q` | Quit |

Arrow keys also work for moving things around if that's more comfortable.

The flag has a minimum scale of 0.5× so the star geometry doesn't break down at tiny sizes. The swoosh can go smaller, down to 0.05×.


**How The Drawing Work**

**Flag stripes**
The flag waves by splitting each stripe into many small vertical rectangles (quads).
Each point of these rectangles moves slightly up and down using a sine-wave formula based on its position and a continuously changing wave value. As the animation updates frame by frame, the whole flag appears to ripple naturally like fabric in the wind.

**The star**

The star was one of the hardest parts to make look smooth.
Instead of drawing a simple flat shape, the star is created from five thick connected lines that link the outer points together. To keep the sharp tips clean, the program calculates special corner joins using averaged normal vectors (called miter joins). Small triangles are added between segments to remove gaps, and a safety limit is used so the corners do not stretch too far when the lines become almost parallel.

**The logo**

The curved swoosh shape is made using cubic Bézier curves.
One curve forms the lower edge and another forms the upper edge. The program samples points along both curves and connects them together with GL_QUAD_STRIP to create a smooth filled ribbon-like shape.

## Project structure 

```
main.cpp    — everything, all in one file
README.md   — you're reading it
```

The project uses only basic immediate-mode OpenGL with some mathematical calculations — no external assets, shaders, or textures are required.

---

## Known limitations

The project uses old-style OpenGL (immediate mode and fixed-function pipeline). This was done to make the code simpler and easier to understand. However, it may not work on some modern OpenGL systems unless compatibility mode is enabled.

The waving animation updates about every 16 milliseconds, which is close to 60 FPS. Since it does not use real time calculations, the animation may run a little faster on very powerful computers.

The star was manually designed using fixed coordinate points. If the size of the circle changes, the star coordinates must also be resized to keep the correct shape and position.



##Group members##
1,| FIREWOLD HADIGU..........03222/15|
2,| RAHEL GENETU...........01945/16|
3,| MUSAB AMAN..............00454/16|
4,| TIGIST ASRAT   ........01582/16|
5,|SOLOMON BIADG...........02980/16|
6,|SINETIBEB BEFIKRU .........02037/16|
