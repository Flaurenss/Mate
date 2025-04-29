# TODO List

## TODO
- [ ] Refactor Render system
- [ ] Implement [animations](https://github.com/KhronosGroup/glTF-Tutorials/blob/main/gltfTutorial/gltfTutorial_007_Animations.md).
- [ ] Add more collections to Model concept as: animations, textures...
- [ ] Implement text rendering.
- [ ] Implement basic lighting.
- [ ] Add cubemaps.
- [ ] Add event system (input at least).
- [ ] Refactor ECS implementation to follow a more Data Oriented Design including destroy entities and related data.
- [ ] Implement scripting.
- [ ] Add quaternions to be used in physics simulation.
- [ ] Fix graphics memory management with ECS.
- [ ] Implement parent-children concept.
- [ ] Implement UI for debug.
- [ ] Separate Import model behaviour from model concept.
- [ ] Add Texture independent concept so it can be added freely to any entity.
- [ ] Investigate and implement Job thread engine.
- [ ] Pack dependencies as nuget.

## In progress
- [ ] Refactor Game project

## DONE
- [x] Implement collision physics engine.
- [x] Separate Draw from Model object.
- [x] Implement main procedural game movement.
- [x] Clean camera component.
- [x] Update README with build instructions.
- [x] Add player movement.
- [x] Refactor `GltfImporter` `ProcessPrimitive` method.
- [x] Fix `Matrix4` operations to follow left to right order.