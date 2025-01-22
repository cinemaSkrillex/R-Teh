#ifndef SCENE_HPP_
#define SCENE_HPP_

namespace RealEngine {
class Registry;
}
class Scene {
   public:
    Scene()          = default;
    virtual ~Scene() = default;

    virtual void initialize(RealEngine::Registry& registry) = 0;
    virtual void update(float deltaTime)                    = 0;
    virtual void render()                                   = 0;
};

#endif /* SCENE_HPP_ */