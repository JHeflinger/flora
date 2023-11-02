#include "flpch.h"
#include "SceneManager.h"

namespace Flora {
	SceneManager::SceneManager() {
		m_SceneMap["D:/Dev/FOL/Assets/Scenes/Core.flora"] = R"(Scene: The Game of Life
Primary Camera: 4
Primary Audio Listener: -1
Physics:
  Gravity: 9.80000019
  VelocityIterations: 6
  PositionIterations: 2
Entities:
  - Entity: 4
    TagComponent:
      Tag: Camera
    TransformComponent:
      Translation: [0, 0, 0.100000001]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 1
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 30
        OrthographicNear: -1
        OrthographicFar: 1
      FixedAspectRatio: false
      Visible Borders: false
    ChildComponent:
      - 3
      - 2
      - 1
  - Entity: 3
    TagComponent:
      Tag: Play Button
    TransformComponent:
      Translation: [-1, 14, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Visible: true
      Color: [1, 1, 1, 1]
      Path: $/Textures/play.png
      Filename: play.png
      Type: 0
      TilingFactor: 1
      Rows: 1
      Columns: 1
      Paused: false
      RowCoordinate: 1
      ColumnCoordinate: 1
      SubtextureWidth: 1
      SubtextureHeight: 1
      Frames: 1
      StartFrame: 1
      EndFrame: 1
      CurrentFrame: 1
      FPS: 1
    ScriptComponent:
      ClassName: Game.PlayButton
    ParentComponent:
      Parent: 4
      InheritAll: true
      InheritTransform: true
      InheritSpriteProperties: true
  - Entity: 2
    TagComponent:
      Tag: Pause Button
    TransformComponent:
      Translation: [1, 14, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Visible: true
      Color: [1, 1, 1, 1]
      Path: $/Textures/pause.png
      Filename: pause.png
      Type: 0
      TilingFactor: 1
      Rows: 1
      Columns: 1
      Paused: false
      RowCoordinate: 1
      ColumnCoordinate: 1
      SubtextureWidth: 1
      SubtextureHeight: 1
      Frames: 1
      StartFrame: 1
      EndFrame: 1
      CurrentFrame: 1
      FPS: 1
    ScriptComponent:
      ClassName: Game.PauseButton
    ParentComponent:
      Parent: 4
      InheritAll: true
      InheritTransform: true
      InheritSpriteProperties: true
  - Entity: 1
    TagComponent:
      Tag: Reset Button
    TransformComponent:
      Translation: [0, 14, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Visible: true
      Color: [1, 1, 1, 1]
      Path: $/Textures/reset.png
      Filename: reset.png
      Type: 0
      TilingFactor: 1
      Rows: 1
      Columns: 1
      Paused: false
      RowCoordinate: 1
      ColumnCoordinate: 1
      SubtextureWidth: 1
      SubtextureHeight: 1
      Frames: 1
      StartFrame: 1
      EndFrame: 1
      CurrentFrame: 1
      FPS: 1
    ScriptComponent:
      ClassName: Game.ResetButton
    ParentComponent:
      Parent: 4
      InheritAll: true
      InheritTransform: true
      InheritSpriteProperties: true
  - Entity: 0
    TagComponent:
      Tag: Game Controller
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ScriptComponent:
      ClassName: Game.Controller
      ScriptFields:
        - Name: Frequency
          Type: Float
          Data: 0.100000001
        - Name: Size
          Type: Int
          Data: 50)";
	}

	std::string SceneManager::GetScene(std::string scene) {
		std::replace(scene.begin(), scene.end(), '\\', '/');
		if (m_SceneMap.find(scene) == m_SceneMap.end()) return "";
		return m_SceneMap[scene];
	}
}