

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