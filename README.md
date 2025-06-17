# Material Shader Example

This project is a small example on how to do a `FMaterialShader` in Unreal Engine.

It also shows how to hook into the Nanite visibility buffer to replace shading bins with custom ones.

## Structure

All the relevant C++ code is here: https://github.com/Phyronnaz/MaterialShaderExample/tree/master/Plugins/MaterialShaderPlugin/Source/MaterialShaderPlugin/Private

The only shader in this project is here: https://github.com/Phyronnaz/MaterialShaderExample/blob/master/Plugins/MaterialShaderPlugin/Shaders/MaterialShaderExample.usf

## What this does

This example plugin adds a new actor class, `MaterialShaderExampleActor`. There should only be one placed per level.

This actor has a few settings:

* `MaterialToReplace`: this is the material to replace in the Nanite visibility buffer
 
* `MaterialSelector`: this is the material handling material selection. It will be called by our custom material shader. The BaseColor.r output will be used as index into the `NewMaterials` array.

* `NewMaterials`: the materials to replace `MaterialToReplace` with, selected by `MaterialSelector`

## Screenshots

The floor material here is replaced by red & blue materials:

<img width="400" alt="image" src="https://github.com/user-attachments/assets/475c849e-cd9a-4d73-b828-c31397d11b09" />

Red and blue are two fully different materials:

<img width="353" alt="image" src="https://github.com/user-attachments/assets/42da3248-26fa-4a9c-a20e-e3d068f2e609" />

The material selector is using noise to pick which material to render:

<img width="400" alt="image" src="https://github.com/user-attachments/assets/68fe9b0a-0ee1-4fb6-8b9a-bdc9ea2e4e9d" />

The actor settings, note the material indices:

<img width="331" alt="image" src="https://github.com/user-attachments/assets/004c0a24-6b10-457e-aa7b-f9b4eac49e91" />

