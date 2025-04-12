# Mate Engine
Basic Engine powered by OpenGL.

## How to Build
- This engine has been developed with Visual Studio 2022. In order to avoid possible unmanaged issues 2022 is the recommended version.
- Third party dependencies are located outside this repository. Please download them from the `Release` page [here](TODO) and mantain the following structure:
```
---- CustomFolder
   |
   |
    ---- dependencies
   |
   |
    ---- Project Repository
```
- Postbuild event in `Mate.Game` is in charge of copying runtime dependencies like `shaders`, `3D models` (`copyDependencies.bat`) from `Mate` to `Mate.Game`. Basic folders order is mandatory to be maintained.

## Dependencies
- GLFW
- GLAD
- stb_image
- ufbx
- cgltf
