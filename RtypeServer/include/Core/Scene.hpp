#ifndef SCENE_HPP_
#define SCENE_HPP_

namespace RealEngine {
class Registry;
}
class Scene {
   public:
    virtual ~Scene() = default;

    // Initialize the scene
    virtual void initialize(RealEngine::Registry& registry) = 0;

    // Update the scene
    virtual void update(float deltaTime) = 0;

    // Render the scene
    virtual void render() = 0;
};

#endif /* SCENE_HPP_ */