#include "simulation/factory/scene_object_factory.h"

#include <game/components/render/render_component.h>
#include <graphics/model/material.h>

#include <graphics/shaders/loaders/program_loader.h>
#include <graphics/shaders/program.h>
#include <resources/resources.h>
#include <graphics/model/mesh.h>
#include <graphics/model/model.h>

SceneObjectFactory::SceneObjectFactory(){}

SceneObjectFactory::~SceneObjectFactory(){}

std::shared_ptr<ifx::RenderComponent> SceneObjectFactory::CreateLine(
        const glm::vec3& p1,
        const glm::vec3& p2,
        const glm::vec3& color){
    auto material = std::shared_ptr<ifx::Material>(new ifx::Material());

    material->diffuse = CreateSolidColorTexture(
            color, ifx::TextureTypes::DIFFUSE);
    material->specular = CreateSolidColorTexture(
            color, ifx::TextureTypes::SPECULAR);

    auto model = CreateLineModel(p1, p2);
    model->getMesh(0)->material(material);
    auto program = LoadMainProgram();

    auto render_object
            = std::shared_ptr<ifx::RenderComponent>(
                    new ifx::RenderComponent(model));
    render_object->addProgram(program);

    render_object->SetBeforeRender([](const Program* program){
        glLineWidth(6);
    });
    render_object->SetAfterRender([](const Program* program){
        glLineWidth(1);
    });

    return render_object;
}

std::shared_ptr<ifx::RenderComponent> SceneObjectFactory::CreateCircle(
        float radius){
    auto model = LoadCircleModel(radius);

    auto material = std::shared_ptr<ifx::Material>(new ifx::Material());

    auto color = glm::vec3(0,255,0);
    material->diffuse = CreateSolidColorTexture(
            color, ifx::TextureTypes::DIFFUSE);
    material->specular = CreateSolidColorTexture(
            color, ifx::TextureTypes::SPECULAR);
    model->getMesh(0)->material(material);

    auto render_object
            = std::shared_ptr<ifx::RenderComponent>(
                    new ifx::RenderComponent(model));
    auto program = LoadMainProgram();
    render_object->addProgram(program);

    return render_object;
}

std::shared_ptr<ifx::Texture2D> SceneObjectFactory::CreateSolidColorTexture(
        const glm::vec3& color,
        ifx::TextureTypes texture_type,
        int n, int m){
    auto texture = ifx::Texture2D::MakeTexture2DEmpty(ifx::NO_FILEPATH,
                                                 texture_type,
                                                      ifx::TextureInternalFormat::RGB,
                                                      ifx::TexturePixelType::UNSIGNED_BYTE,
                                                 n, m);
    int data_size = n*m*3;
    std::vector<unsigned char> data;
    data.resize(data_size);
    int i = 0;
    while(i < data_size){
        data[i++] = color.x;
        data[i++] = color.y;
        data[i++] = color.z;
    }
    texture->InitData((void*)data.data(), n, m);

    return texture;
}


std::shared_ptr<Program> SceneObjectFactory::LoadMainProgram(){
    ifx::Resources& resources = ifx::Resources::GetInstance();

    std::string vertex_path =
            resources.GetResourcePath("main/main.vs",
                                      ifx::ResourceType::SHADER);
    std::string fragment_path =
            resources.GetResourcePath("main/main.fs",
                                      ifx::ResourceType::SHADER);

    return ProgramLoader().CreateProgram(vertex_path, fragment_path);
}

std::shared_ptr<ifx::Model> SceneObjectFactory::CreateLineModel(
        const glm::vec3& p1, const glm::vec3& p2){
    std::vector<std::unique_ptr<ifx::Mesh>> meshes;
    meshes.push_back(std::move(CreateLineMesh(p1,p2)));

    return ifx::Model::MakeModel(ifx::NO_FILEPATH, std::move(meshes));
}

std::unique_ptr<ifx::Mesh> SceneObjectFactory::CreateLineMesh(
        const glm::vec3& p1, const glm::vec3& p2){
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    vertices.push_back(Vertex{
            p1, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
    vertices.push_back(Vertex{
            p2, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
    indices.push_back(0);
    indices.push_back(1);

    std::unique_ptr<ifx::Mesh> mesh(new ifx::Mesh(vertices, indices));
    mesh->primitive_draw_mode(ifx::PrimitiveDrawMode::LINES);

    return mesh;
}

std::unique_ptr<ifx::Mesh> SceneObjectFactory::LoadCircleMesh(float radius) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    /// ------
    double latitudeBands = 20;
    double longitudeBands = 20;

    for (double latNumber = 0; latNumber <= (latitudeBands); latNumber++) {
        double theta = latNumber * M_PI / latitudeBands;
        //theta /= 2.0f;

        double sinTheta = sin(theta);
        double cosTheta = cos(theta);

        for (double longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            double phi = longNumber * 2 * M_PI / longitudeBands;
            double sinPhi = sin(phi);
            double cosPhi = cos(phi);

            Vertex vs;
            vs.Normal = glm::vec3(cosPhi * sinTheta,
                                  cosTheta,
                                  sinPhi * sinTheta);
            vs.TexCoords = glm::vec2(1 - (longNumber / longitudeBands),
                                     1 - (latNumber / latitudeBands));

            vs.Position = glm::vec3(radius * vs.Normal[0],
                                    radius * vs.Normal[1],
                                    radius * vs.Normal[2]);
            vs.Normal = -vs.Normal;

            vertices.push_back(vs);
        }

        for (int latNumber = 0; latNumber < latitudeBands; latNumber++) {
            for (int longNumber = 0; longNumber < longitudeBands; longNumber++) {
                int first = (latNumber * (longitudeBands + 1)) + longNumber;
                int second = first + longitudeBands + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);

            }
        }
    }

    auto mesh = std::unique_ptr<ifx::Mesh>(new ifx::Mesh(vertices, indices));
    //mesh->setPolygonMode(GL_LINE);

    return mesh;
}

std::shared_ptr<ifx::Model> SceneObjectFactory::LoadCircleModel(float radius){
    std::vector<std::unique_ptr<ifx::Mesh>> meshes;
    meshes.push_back(std::move(LoadCircleMesh(radius)));

    return ifx::Model::MakeModel(ifx::NO_FILEPATH, std::move(meshes));
}
