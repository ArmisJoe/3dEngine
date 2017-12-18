# QUEENGINE3D

QUEENGINE3D is a 3D Engine meant for videogames developed in C++ by students Pau Olive and Josep Huguet at Universitat Politecnica de Catalunya (UPC).
Subject: 3D Engines

[Link to Latest Release](https://github.com/ArmisJoe/3dEngine/releases/tag/Assignment3)

## Github & Repository:

[Link to source code for QUEENGINE3D](https://github.com/ArmisJoe/3dEngine)

Josep Huguet: https://github.com/ArmisJoe

Pau Olive: https://github.com/rcpauor32

License (MIT): https://opensource.org/licenses/MIT

## Team
![i](https://lh5.googleusercontent.com/ZQcvYFLGfVJX9vGFCEjgGJ3tMcfRgIs53snMLqUhngkG8hRmmy8EsTX17WZIteZvYpOf_axICuaQLZLnEjQH=w1680-h895-rw)

### Pau Olive (Left)
In charge of:
- GUI
- Console, About, Ispector & Hierarchy Panels
- Wireframe & Render
- File System (w/DeVil)
- Loading by Drag & Drop
- Components Material, Mesh and Material
- Importer to own format, and loader.
- Scene Serialization (+ Play & Pause)
- Component Animation
- Interpolation & Blend

### Josep Huguet (Right)
In charge of:
- Set Up first code
- Config panel
- Component Camera & Module
- AABBs for Game Objects
- Mouse Picking from scene
- Adaptative Octree & camera culling (accelerate)
- Skinning
- Component Transform & Bone

## Engine Subsystems:
- Informative Panels: reflect module performance and help the user to navigate through different elements.
- Drag & Drop Loading: lets the user import elements directly to the scene just by dragging the element.
- Own Importer: Elements imported to the scene are saved to our own format and loaded after that.
- Scene Serialization: The user can save the elements in the scene in a JSON format and load them whenever they want.
- Animations: Objects with an animation will run on play-time.
- Mouse Picking: Picking elements from the scene with the mouse directly.
- Adaptative Octree: That stores and subdives according to the elements inserted, it is also used to accelerate camera culling.

## Animation System:
For the last delivery, users are able to load an object with a an animation (only supports .fbx and .dae fromats).
The animation is loaded separated from the code, in a Component Animation that stores the Animation nodes for each frame and their position. The bones are loaded in the Component Bone storing the weights needed to apply a transformation to the mesh.
On each update, the animation nodes send the new updated position to every child from the animated object. After that, the mesh creates a copy of itself to which all vertex transformation are applied (Skinning) before being sent to the GPU.

![i](https://media.giphy.com/media/l4EoRTG4ksTAiuQ7e/giphy.gif)
![i](https://media.giphy.com/media/l4EpcOlnQbtKt1ckM/giphy.gif)

## Video Demonstration
[Link to video](https://www.youtube.com/watch?v=RjmUNF4LWTo&feature=youtu.be)
## Other Relevant Information

### Camera Controls

Mouse wheel to Zoom In - Out
Mouse right click to rotate the camera.
Mouse right click + WASD to move the camera.
R or F to move camera Up and Down.

### Panels

Panel about gives important info about the project.
Panel Config gives running information and allows to change settings.
The Console panel will log all actions done automatically.
Panel Inspector gives information about the current GameObjects. (Such as geometry, transform or texture).

### How To Use

Drag the street .fbx to the scene.
Drag the Idle_alien.fbx to the scene.
Select items from the hierarchy with the left mouse button and open them on the inspector with the right mouse button. 
Press play to play animation, and stop to stop it.
Drag the frame counter to go back and forth through the animation.
