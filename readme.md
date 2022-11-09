melee-like engine

# Linux Build

```bash
$ cmake . -DOpenGL_GL_PREFERENCE=GLVND  # raylib opengl preference
```
we will prob want to use FIXMATH_FAST_SIN, take a closer look in lib/libfixmath/libfixmath/fix16_trig.c

![fixed point capsule collision](https://i.imgur.com/3QwXJt7.gif)


git submodule add -b <branch> <url> <relative_path_4m_root>

git submodule sync


in .gitmodules: 
- "ignore = dirty" means ignore all changed files
- "ignore = untracked" means isgnore all untracked files


fgl files attained from: https://github.com/mhfan/fgl

todo: rename fgl to fix_vec3 or something its sufficently different at this point imo

**DUMBASS ALERT DUMBASS ALERT GOTTA USE EXTERN "C" WITH C FILES**

3d course grain collision notes:

active hitboxes only look for opponent tangable hurtboxes

search list-> bounding box?

flatten to 2d AABB based on bone position + largest bone radius?


need a menu system:
https://github.com/RobLoach/raylib-nuklear


need a multiplayer connect screen with these fields:

port to bind:
stage:

your player number: [p1, p2, p3, p4]
your tag: ""
your character: 

opponent player number: [p1, p2, p3, p4]
opponent tag:
opponent character:
opponent ip

https://stackoverflow.com/questions/48554758/why-create-an-include-directory-in-c-and-c-projects

TODO: get rid of src/include, make fgl_vec3 into its own library (Fix16Vec3) and put it in lib/Fix16Vec3. It may turn into its own library one day or I may push it upstream to libfixmath (prob after quaternions & the skeleton stuff)


TODO: need to namespace stuff, but want to keep as much as possible in c for simplicity?


next steps:
1. [ ] add https://github.com/RobLoach/raylib-nuklear to project
2. [ ] create main menu scene (state update + render)
3. [ ] create ggpo info scene (state update + render)
    - use nk_input_glyph as text box
    - 
4. [ ] finalize(?) mini engine (scene stack + arbitrary update logic) be able two switch between engine scenes
5. integrate ggpo
6. game settings initialization/syncronization (use sockets?)

- [ ] assess rewrite in rust
- [ ] seperate engine into library

phase 2:
1. [ ] fixed point skeleton
2. [ ] asset packing & import (Doom's .wad analog) (need unreal, ogre, irrlicht assesments)
3. [ ] blender rig -> fixed point rig conversion tool
4. [ ] stage building tool
5. [ ] stage / ecb course grain + fine grain collision
6. [ ] sound assets + sound thread

phase 3:
1. [ ] windows build
2. [ ] assemble art team
3. [ ] asset creation
4. [ ] game finalization/release
