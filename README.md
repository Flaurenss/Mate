# Mate Engine
Basic Engine powered by OpenGL.

## How to Build
- This engine has been developed with Visual Studio 2022. In order to avoid possible unmanaged issues 2022 is the recommended version.
- Third party dependencies are located outside this repository. Please download them from the `Release` page and mantain the following structure:
```
-----> Your_folder
        |
        |
        ---> dependencies
        |
        |
        ---> Project Repository
```
- Postbuild event in `Mate.Game` is in charge of copying runtime dependencies like `shaders`, `3D models` (`copyDependencies.bat`) from `Mate` to `Mate.Game`. Basic folders order is mandatory to be maintained.

## Hot to Use

Please refer to [HOWTO-USE.md](./Mate/HOWTO-USE.md) for a more detailed explanation on how to use the engine and implement your first game.

## External Dependencies
- [GLAD](https://glad.dav1d.de/) (OpenGL v4.6)
- [GLFW v3.4](https://github.com/glfw/glfw/releases/tag/3.4)
- [stb_image](https://github.com/nothings/stb/tree/master)
- [ufbx](https://github.com/ufbx/ufbx)
- [cgltf v1.15](https://github.com/jkuhlmann/cgltf/releases/tag/v1.15)
- [Jolt v5.3.0](https://github.com/jrouwe/JoltPhysics/releases/tag/v5.3.0)
- [ozz v0.16](https://github.com/guillaumeblanc/ozz-animation/releases/tag/0.16.0)
- [SoLoud v20200207](https://solhsa.com/soloud/downloads.html)

## License

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg