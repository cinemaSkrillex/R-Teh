

#include <SFML/Graphics.hpp>

#include "Client/UDPClient.hpp"
#include "CollisionHandler.hpp"
#include "Controls.hpp"
#include "Core/ClockEngine.hpp"
#include "Core/View.hpp"
#include "Core/Window.hpp"
#include "ECS/Components/AI.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Components/Controllable.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Health.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Radius.hpp"
#include "ECS/Components/Rotation.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/SpriteSheet.hpp"
#include "ECS/Components/Target.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Registry/Registry.hpp"
#include "ECS/Systems/AISystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/ControlSystem.hpp"
#include "ECS/Systems/DrawSystem.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/RadiusSystem.hpp"
#include "ECS/Systems/RotationSystem.hpp"
#include "ECS/Systems/healthSystem.hpp"
#include "EyeBoss.hpp"
#include "Media/Audio/Music.hpp"
#include "Media/Audio/Sound.hpp"
#include "Media/Graphics/Rendering/Background.hpp"
#include "Media/Graphics/Rendering/Sprite.hpp"
#include "Scripting/LuaManager.hpp"

namespace rtype {
class Game {
   public:
    Game(std::shared_ptr<UDPClient> clientUDP);
    ~Game();
    void               run();
    void               setDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
    void               init_registry();
    void               init_controls();
    void               init_systems();
    RealEngine::Entity createEntity();
    void add_component_to_entity(RealEngine::Entity entity, RealEngine::Position position);

    std::shared_ptr<UDPClient> _clientUDP;

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
    RealEngine::AISystem        _aiSystem;
    RealEngine::RotationSystem  _rotationSystem;
    RealEngine::RadiusSystem    _radiusSystem;
    rtype::Controls             _controls;

    std::unique_ptr<EyeBoss>                            _bossEye;
    RealEngine::Sprite                                  _upSpaceship;
    RealEngine::Sprite                                  _idleSpaceship;
    RealEngine::Sprite                                  _downSpaceship;
    RealEngine::Sprite                                  _testSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _spaceshipSheet;
    RealEngine::Sprite                                  _groundSprite;
    RealEngine::Entity                                  _entity1;
    RealEngine::Entity                                  _entity2;
    RealEngine::Entity                                  _groundEntity;
};
}  // namespace rtype