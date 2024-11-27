

#include <SFML/Graphics.hpp>
#include "Core/Window.hpp"
#include "Core/View.hpp"
#include "Core/ClockEngine.hpp"
#include "Media/Graphics/Rendering/Sprite.hpp"
#include "Media/Graphics/Rendering/Background.hpp"
#include "Media/Audio/Sound.hpp"
#include "Media/Audio/Music.hpp"
#include "Scripting/LuaManager.hpp"
#include "ECS/Registry/Registry.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/SpriteSheet.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Controllable.hpp"
#include "ECS/Systems/DrawSystem.hpp"
#include "ECS/Systems/ControlSystem.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "Controls.hpp"

namespace rtype {
class Game {
  public:
    Game();
    ~Game();
    void   run();
    void   setDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
    void   init_registry();
    void   init_controls();
    void   init_movement_system();
    Entity createEntity();
    void   add_component_to_entity(Entity entity, Position position);

  private:
    float                  _deltaTime = 0.f;
    RealEngine::Window     _window;
    RealEngine::View       _view;
    sf::Clock              _clock;
    RealEngine::LuaManager _luaManager;
    Registry               _registry;
    DrawSystem             _drawSystem;
    ControlSystem          _controlSystem;
    MovementSystem         _movementSystem;
    rtype::Controls        _controls;

    RealEngine::Sprite                                  _upSpaceship;
    RealEngine::Sprite                                  _idleSpaceship;
    RealEngine::Sprite                                  _downSpaceship;
    std::unordered_map<std::string, RealEngine::Sprite> _spaceshipSheet;
    Entity                                              _entity1;
    Entity                                              _entity2;
};
} // namespace rtype