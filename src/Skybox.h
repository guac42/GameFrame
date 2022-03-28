#ifndef GAMEFRAME_SKYBOX_H
#define GAMEFRAME_SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "objects/ShaderProgram.h"
#include "objects/VertexArray.h"
#include "objects/Texture.h"
#include "Image.h"

class Skybox {
private:
    VertexArray vao;
    Texture texture;
    Program program;

public:
    explicit Skybox(const std::string& texturePath)
    : program({*new Shader(Shader::Vertex, "#version 430 core\n"
                                           "layout (location = 0) in vec3 aPosition;\n"
                                           "uniform mat4 uProjection, uView;\n"
                                           "out vec3 eyeDir;\n"
                                           "void main() {\n"
                                           "    mat4 inverseProjection = inverse(uProjection);\n"
                                           "    mat3 inverseModelview = transpose(mat3(inverse(uView)));\n"
                                           "    vec3 unprojected = (inverseProjection * vec4(aPosition, 1.0f)).xyz;\n"
                                           "    eyeDir = inverseModelview * unprojected;\n"
                                           "    gl_Position = vec4(aPosition, 1.0f);\n"
                                           "}", false),
               *new Shader(Shader::Fragment, "#version 430 core\n"
                                             "uniform samplerCube uTexture;\n"
                                             "in vec3 eyeDir;\n"
                                             "out vec4 frag_color;\n"
                                             "void main() {\n"
                                             "    frag_color = texture(uTexture, eyeDir);\n"
                                             "}", false)}),
        texture(GL_TEXTURE_CUBE_MAP), vao() {
        int width = 0, height = 0, comps = 0;
        unsigned char *data = Image::load(texturePath.c_str(), &width, &height, &comps);

        if (!data)
            printf("Cubemap texture failed to load at path: %s\n", texturePath.c_str());
        int xStride = width/4, yStride = height/3;
        int type = comps == 4 ? GL_RGBA : GL_RGB;

        this->texture.ImmutableAllocate(xStride, yStride, 1, GL_SRGB8_ALPHA8);

        glTextureSubImage3D(this->texture.getId(), 0, 2*xStride,   yStride, 0, xStride, yStride, 1, type, GL_UNSIGNED_BYTE, data); //right
        glTextureSubImage3D(this->texture.getId(), 0,         0,   yStride, 1, xStride, yStride, 1, type, GL_UNSIGNED_BYTE, data); //left
        glTextureSubImage3D(this->texture.getId(), 0,   xStride,         0, 2, xStride, yStride, 1, type, GL_UNSIGNED_BYTE, data); //bottom
        glTextureSubImage3D(this->texture.getId(), 0,   xStride, 2*yStride, 3, xStride, yStride, 1, type, GL_UNSIGNED_BYTE, data); //top
        glTextureSubImage3D(this->texture.getId(), 0,   xStride,   yStride, 4, xStride, yStride, 1, type, GL_UNSIGNED_BYTE, data); //front
        glTextureSubImage3D(this->texture.getId(), 0, 3*xStride,   yStride, 5, xStride, yStride, 1, type, GL_UNSIGNED_BYTE, data); //back

        /*this->texture.Bind();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);*/

        Image::free(data);

        this->texture.TextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        this->texture.TextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        this->texture.TextureParam(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Buffer buffer;
        buffer.Allocate(sizeof(vertices), (void*)vertices, GL_DYNAMIC_STORAGE_BIT);

        this->vao.Bind();
        this->vao.AddSourceBuffer(buffer, 0, 3*sizeof(float));
        this->vao.SetAttribFormat(0, 3, GL_FLOAT);

        this->program.Use();
        this->program.Upload("uTexture", 0);
    }

    void Render(const glm::mat4& projection, const glm::mat4& view) const {
        glDepthFunc(GL_LEQUAL);
        this->program.Use();
        this->program.Upload("uProjection", projection);
        this->program.Upload("uView", view);
        this->vao.Bind();
        Texture::Activate(GL_TEXTURE0);
        this->texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
    }

private:
    constexpr static const float vertices[] {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
    };
};

#endif //GAMEFRAME_SKYBOX_H
