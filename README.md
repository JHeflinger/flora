# Flora
Flora is a passion project to create a viable game engine for personal use out of pure C++! Flora started out
as a way to understand how game engines work and gain a deeper understanding on game development. Although it pales
in comparison to industry-standard game engines like Unity or Unreal Engine, Flora aims to be a simple, easy to handle
open-source game engine for new developers who desire an easy way to start game development that allows easy customization
to the game engine itself!

## Features
Flora includes but is not limited to the following features:
- Intuitive Graphical Interface
- Entity Component System
- OpenGL, DirectX, and Vulkan support
- Scripting with C# or native scripting with C++
- Asset Library
- Build capablities
- 2D and 3D Renderer
- And so much more!

## How to Use
Flora itself is split into 2 main components! This makes it easier for developers to branch out and use flora as not just 
an editor, but as an API in their own standalone solutions! 

### Flora
Flora refers to the actual API reference. This includes all the capabilities of the game engine, meaning developers can easily
take just Flora itself and use it to develop without a graphical editor, or make one on their own!

### Fauna
Fauna refers to the provided graphical editor that implements Flora. New developers are recommended to utilize this editor themselves
for their projects! Fauna offers all the basic capabilities of Flora, and make managing your project much eaiser.

## Progress
Flora is currently still in the early development stages, but a stable release is planned to be out before end the end of 2023! Currently
Flora only supports Windows using OpenGL, but once a stable release is flushed out DirectX and Vulkan support will not be far behind!

## Getting Started
To get started, run the following to clone the repository:

```
git clone --recursive https://github.com/JHeflinger/flora.git
```

This will initialize and download all the required git submodules for the dependencies Flora uses. If you've already
cloned Flora normally, you can also run the following to download submodules:

```
git submodule update --init --recursive
```

Once everything is properly cloned, run the build script in the scripts folder!

```
./scripts/Setup.bat
```

Note that this builds project files for **Visual Studio 2019**. If you use a different version of visual studio, you may need to use a different version of premake
(located in the `vendor/premake/bin` folder) and modify the respective build script. 

This will generate all the solution files needed using premake. Once that succeeds, you're all set! You can now open the .sln file in Visual Studio 2019 and start 
using Flora! The startup project should be specified to Fauna, so you'll be able to just press Run and Debug to try it out!