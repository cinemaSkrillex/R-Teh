

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
#include "ECS/Systems/CollisionSystem.hpp"
#include "Controls.hpp"
#include "CollisionHandler.hpp"

namespace rtype {
class Game {
  public:
    Game();
    ~Game();
    void               run();
    void               setDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
    void               init_registry();
    void               init_controls();
    void               init_systems();
    RealEngine::Entity createEntity();
    void add_component_to_entity(RealEngine::Entity entity, RealEngine::Position position);

  private:
    float                       _deltaTime = 0.f;
    RealEngine::Window          _window;
    RealEngine::View            _view;
    sf::Clock                   _clock;
    RealEngine::LuaManager      _luaManager;
    RealEngine::Registry        _registry;
    RealEngine::DrawSystem      _drawSystem;
    RealEngine::ControlSystem   _controlSystem;
    RealEngine::MovementSystem  _movementSystem;
    RealEngine::CollisionSystem _collisionSystem;
    rtype::Controls             _controls;

    RealEngine::Sprite                                  _upSpaceship;
    RealEngine::Sprite                                  _idleSpaceship;
    RealEngine::Sprite                                  _downSpaceship;
    RealEngine::Sprite                                  _groundSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _spaceshipSheet;
    RealEngine::Entity                                  _entity1;
    RealEngine::Entity                                  _entity2;
    RealEngine::Entity                                  _groundEntity;
};
} // namespace rtype