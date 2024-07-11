# Julgen - Game Engine

![Logo](https://i.imgur.com/koxF0XZ.png)

## Goal
Attempting to simplify the game development process while upholding the C++ Core Guidelines.

## Patterns Used
Game design patterns are used to simplify the development, I have attempted to implement some patterns specific to the needs of Julgen. Some patterns are more specific and should be left out of the engine, but here are some patterns I included for ease of use.

**Observer Pattern** - Julgen has a build in Event class that allows to be used similar to a [Unity Action](https://docs.unity3d.com/ScriptReference/Events.UnityAction.html) or a [UE Delegates](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Delegates/). These can be used to bind a (member) function to a object of type Event. Once Invoke() is called on the event all (member) functions will immediately be invoked. The observer class will require to inherit from IEventListener, this allows for automatic removal of the observer or the sender.

**Command Pattern** - In short, allowing users to encapsulate input requests as objects.

**Singleton** - A simple implementation of Singleton is included in the engine. Use it judiciously.

**Dirty Flag** - Used in various places within the engine. Most notably the transform component; to allow for world and local positions within huge hierarchical structures.

**Update Method - Update Method - Double Buffering** - As most modern game engines, Julgen also employs a Update method. This seemingly easy design pattern allows for very practical game programming. At the same time Julgen includes a helpful GameTime class allowing for frame independent gameplay. While double buffering is quite a brought topic, Julgen uses it within the components using Late Update to allow for more control over squenced operations.

**Component**
The good old Gameobject Component system has been used and allows for more modular and diverse games.


## Used References
[C++ Core Guidlines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
[Game Programming Patterns by Robert Nystrom](https://gameprogrammingpatterns.com)
[Beautiful C++](https://www.informit.com/store/beautiful-c-plus-plus-30-core-guidelines-for-writing-9780137647842)




## cross-platform
![CMake Logo](https://upload.wikimedia.org/wikipedia/commons/thumb/1/13/Cmake.svg/64px-Cmake.svg.png)
**CMake** is used to build the engine 

Julgen is build with both Windows, Linux and web in mind.
The engine should be able to be compiled using both MSVC and GCC.
Web builds can be made using Emscripten.


## Update Loop
<img src="https://i.imgur.com/4FO7eJY.png" style="width:60%;  display: block;
  margin-left: 0;
  margin-right: auto;"/>


## todo / ideals
* **Game Loop**
    The current game loop uses some resources to loop over all components and game objects to check if they are set to be destroyed. There are some similar scenarios and these could be addressed by smartly moving objects between containers. This however should be properly tested for real world performance difference.  
* **Rendering**
    The rendering sorting is currently manually being done on the cpu every frame and should ultimately be replaced with proper rendering. Also allowing hte engine for 3D rendering.
    Rotating and scaling is also on the list ;)

## License
Julgen is free software; you can redistribute it and/or modify it under the terms of the [GNU General Public License](LICENSE.md) as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.