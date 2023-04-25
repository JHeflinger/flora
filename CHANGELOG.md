# Change Log
All notable changes to this project will be documented in this file as of Version 0.1.1

## Fauna [0.1.2] = 2023-04-25

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
