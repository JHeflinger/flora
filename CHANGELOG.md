# Change Log
All notable changes to this project will be documented in this file as of Fauna Version 0.1.1 and Flora Version 0.1.1

## Fauna [0.3.1] - 2023-10-24

### Added

- Added TextComponent into the properties panel
- Grid options were added
- Build system template was added

## Flora [0.3.1] - 2023-10-24

### Added

- Font class can take .ttf files and render font
- DrawString was added to Renderer2D to draw strings with a font
- Added MSDF font rendering

### Fixed

- Removed FLORA_API declspec declarations

## Fauna [0.3.0] - 2023-10-24

### Added

- The project system can now be used to load and edit different game projects! Note that this feature is still experimental and under testing.
- The script engine is now available! Developers can now use a pre configured C# visual studio project provided by Fauna to add scripts to their entities for custom behavior!
- AudioListenerComponent and AudioSourceComponents are now in the editor! Note that these components are not yet supported in the script engine.
- LabelComponent is now in the editor! Entites can now be labeled via pre set labels defined in the project settings.
- Documentation is now available via the "Application" menu that documents how to use the script library!
- Selected entities are now outlined!
- Camera entities can now have a visible camera view box so developers can see the range of view for their cameras
- Physics colliders can now be toggled visible
- The editor camera can now be toggled on runtime to override the primary camera!

### Changed

- All asset paths are based off of the project directory now!
- gizmo keybinds will only work when focused in the viewport
- Native scripts are now unsupported in Fauna
- ScriptManagerComponent is being temporarily removed for the transition to C# scripting
- SpriteRendererComponent animations can now be paused via properties panel.

### Fixed

- Fixed issue where a simulated scene being stopped would also unselect a selected entity
- Fixed issue where dragging the editor camera by mouse was far too sensitive

## Flora [0.3.0] - 2023-10-24

### Added

- The project system backend can now be used to manage global settings for a game!
- Audio listeners and sources are now valid components with editable properties and even surround sound! Note that the audio system only supports .wav files at the moment, and stereo will only work in .wav files that have been configured for stereo.
- Labels are now available to categorize entities via the LabelComponent. Note that labels must be defined via the project settings in order to work.
- Circle physics colliders are now available!
- The script engine is now implemented to interface with the backend!

### Changed

- OpenAL and the .NET runtime 4.7.2 are now dependencies the user will now have to manually install. Note that it is planned for 1.0 to include these in an installer for the future.
- Animations can now be paused

### Fixed

- the OnDestroy() function for scriptable entities now works when disposing of an entity via Scene.DestroyEntity()
- Copy pasting entities is now more robust after testing and no longer crashes the engine at certain junctions.

## Fauna [0.2.1] - 2023-06-10

### Added

- Stats for line rendering and circle rendering have been integrated into the stats panel
- Editor settings now save line and circle rendering stats
- Added new circle renderer component to render circles with thickness, radius, and fade
- Console panel now features a developer input which can be handled and interacted with in scripts

### Fixed

- Fixed issue in shaders that blocked select clicking in the editor
- Fixed issue in shaders that cause blending to not work sometimes
- Fixed bug in content browser that cause scene filepath to not update when renaming scenes

## Flora [0.2.0] - 2023-06-10

### Added

- Renderer2D can now render circles
- Renderer2D can now render lines
- PhysicsWrapper around box2D vendor library to easily call flora functions to manipulate physics objects

### Changed

- An AssetManager class was added so every entity that used a texture wouldn't create a new texture object despite using an already loaded texture

## Fauna [0.2.0] - 2023-05-10

### Added

- 2D physics are now implemented! There is now a RigidBody2D component and BoxCollider2D component that can be used to give entities physics bodies with restitution, density, and friction properties!
- Physics panel that can now control the gravity and preciseness of the physics world. Note that increasing the preciseness over the recommended amount may lead to performance issues.
- Stats panel can now control minimim Y Axis and value scalars on the real time graphs
- Errors logged with FL_CORE_ERROR and FL_ERROR now automatically force a popup in the editor
- If no primary camera is selected, console will warn the user
- Stats panel now shows runtime performance in the stats panel, including CPU usage, memory usage, and disk usage. GPU usage is added, but unreliable based on graphics card
- Example "Angry Birds" scene to demo editor capabilities

### Changed

- Primary camera is now enforced through Flora's Scene for security and efficiency
- Fauna now defaults to full screen on startup

### Fixed

- closing the editor while playing the scene would prompt saving
- closing Fauna from the application menu would not work
- Content browser would glitch out and seize up when having file names that overflowed
- Stat panel reset button would crash the program

## Flora [0.1.4] - 2023-05-10

### Added

- New function in the Scene class to get entities from the scene based on their tag

### Changed

- Scene now holds the primary camera entity

### Fixed

- Texture slot 4 would not render correctly when using the texture shader

## Fauna [0.1.3] - 2023-05-02

### Added

- Stats panel now features a graph of info over time that can be customized to specific stats over specific times
- Play button can now be used to run a simulation of behavior
- ScriptManagerComponent GUI can now rearrange the order of scripts through drag and drop
- Content Browser can now make new files, scenes, and folders through right click

### Fixed

- Fixed issue where dragging parents into children in the scene hierarchy would cause a stack overflow due to unhandled recursion

## Flora [0.1.3] - 2023-05-02

### Added

- Native scripts will now automatically bind during runtime if not already binded
- Scenes now have set names rather than depending on the scene's filename
- ImPlot integrated into Flora's ImGui integration

### Changed

- Native scripts now only will run during game runtime, not editor runtime
- Yaml extended operators have been moved into "YamlUtils.h"
- Moved "Structures.h" into Fauna

## Fauna [0.1.2] - 2023-04-25

### Added

- Cut, copy, and paste functionality for the content browser
- Cut, copy, and paste functionality for the viewport and scene hierarchy
- Content browser now has a default right click menu for pasting and adding new folders
- You can now delete entities from scene hierarchy/viewport and content from content browser using the "delete" key

### Changed

- Script manager component should now be able to remove scripts

### Fixed

- Fixed issue where app would crash if the last selected entity wasn't saved in previous editor session (fixed via Flora 0.1.2)

## Flora [0.1.2] - 2023-04-25

### Added

- Clipboard structure that can hold a filepath and an entity
- Panel enumeration to differentiate between panels efficiently

### Changed

- Components now have copy constructors
- Scene now has a copy entity function that can copy an entity to the scene or to a specific parent
- Scene will now automatically bind native script components if they are not bound. However, this introduces the instability of the NativeScriptComponent to the scene.

### Fixed

- Fixed issue that serializer did not check if selected entity existed in scene when deserializing editor settings

## Fauna [0.1.1] - 2023-04-22
 
### Added

- Stats panel now shows real time video performance stats such as FPS and timestep
- Active scene name is now shown in the top menu bar
- Content browser can now delete and rename files via right click
- Content browser can now open scene files via right click
- Content browser can now move files via dragging and dropping
 
### Changed
  
- Stats panel now features a 2 column format with plans to add in graph popups to show data over time
 
### Fixed
 
- Content browser used to only display filenames on the first row with slightly off centered names

## Flora [0.1.1] - 2023-04-20

### Added

- Serializer will now serialize the recently added ScriptManagerComponent
- Scene can now get if Viewport is focused
