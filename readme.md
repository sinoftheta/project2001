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




