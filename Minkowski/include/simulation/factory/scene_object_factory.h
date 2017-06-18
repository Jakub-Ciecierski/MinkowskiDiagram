#ifndef PROJECT_SCENE_OBJECT_FACTORY_H
#define PROJECT_SCENE_OBJECT_FACTORY_H

#include <math/math_ifx.h>
#include <memory>

namespace ifx{
class RenderComponent;
class Texture2D;
class Model;
class Mesh;
enum class TextureTypes;
}

class Program;

class SceneObjectFactory {
public:

    SceneObjectFactory();

    ~SceneObjectFactory();

    std::shared_ptr<ifx::RenderComponent> CreateLine(
            const glm::vec3& p1,
            const glm::vec3& p2,
            const glm::vec3& color);

    std::shared_ptr<ifx::RenderComponent> CreateCircle(float radius);
private:

    std::shared_ptr<ifx::Texture2D> CreateSolidColorTexture(
            const glm::vec3& color,
            ifx::TextureTypes texture_type,
            int n=1, int m=1);
    std::shared_ptr<Program> LoadMainProgram();
    std::shared_ptr<ifx::Model> CreateLineModel(const glm::vec3& p1,
                                                const glm::vec3& p2);
    std::unique_ptr<ifx::Mesh> CreateLineMesh(
            const glm::vec3& p1, const glm::vec3& p2);

    std::unique_ptr<ifx::Mesh> LoadCircleMesh(float radius);
    std::shared_ptr<ifx::Model> LoadCircleModel(float radius);
};



#endif //PROJECT_SCENE_OBJECT_FACTORY_H
