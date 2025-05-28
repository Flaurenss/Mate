# Mate Engine
Basic Engine powered by OpenGL.

## How to Build
- This engine has been developed with Visual Studio 2022. In order to avoid possible unmanaged issues 2022 is the recommended version.
- Third party dependencies are located outside this repository. Please download them from the `Release` page and mantain the following structure:
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
- [GLAD](https://glad.dav1d.de/) (OpenGL v4.6)
- [GLFW v3.4](https://github.com/glfw/glfw/releases/tag/3.4)
- [stb_image](https://github.com/nothings/stb/tree/master)
- [ufbx](https://github.com/ufbx/ufbx)
- [cgltf v1.15](https://github.com/jkuhlmann/cgltf/releases/tag/v1.15)
- [Jolt v5.3.0](https://github.com/jrouwe/JoltPhysics/releases/tag/v5.3.0)
- [ozz v0.16](https://github.com/guillaumeblanc/ozz-animation/releases/tag/0.16.0)
- [SoLoud v20200207](https://solhsa.com/soloud/downloads.html)
