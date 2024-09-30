



configure file
```c++
#define CHECK_CMAKE false

#define SHADOW_ENABLE true

//////////////////////////////////////////////////////////////////////////////
// Input/Output File Config

// Compulsory
// #define SCENE_FILE_NAME "basic.scene"
// #define SCENE_FILE_NAME "spheramid.scene"
#define SCENE_FILE_NAME "mesh.scene"

// Optional (or define a name)
#define OBJ_FILE_NAME "subdiv.obj"

// Optional (or define a name)
// Default (SCENE_FILE_NAME + ".bmp")
// #define OUTPUT_IMAGE_NAME "output.bmp"

#define IMAGE_WIDTH 1000
#define IMAGE_HEIGHT 1000

```


output exmaple
> basic
![alt](basic.bmp)

> spheramid
![alt](spheramid.bmp)

> mesh
![alt](mesh.bmp)

```
Profiling:

for example for Sheramid we do have

with no SHADOW:
Total render time: 1.431968 seconds
Average time per pixel: 0.000001 seconds
Total lighting calculation time: 0.643970 seconds
Average lighting calculation time per pixel: 0.000001 seconds

with SHADOW:
Total render time: 20.180865 seconds
Average time per pixel: 0.000018 seconds
Total lighting calculation time: 18.221199 seconds
Average lighting calculation time per pixel: 0.000018 seconds
```