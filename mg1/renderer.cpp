#include "renderer.h"

glm::vec4 AdaptiveRenderer::computeColor(int centerX, int centerY, const EllipsoidProperties& properties)
{
    const float normalizedX = (static_cast<float>(centerX) / windowWidth - 0.5f);
    const float normalizedY = (0.5f - static_cast<float>(centerY) / windowHeight);

    glm::mat4 dPrim = calculateDPrim(
        properties.position,
        glm::vec3(properties.scaleFactor, properties.scaleFactor, properties.scaleFactor),
        properties.rotation,
        properties.a, properties.b, properties.c);

    float z = elipsoidZ(normalizedX, normalizedY, dPrim);

    if (std::isnan(z))
    {
        return backgroundColor;
    }

    glm::vec3 point = { normalizedX, normalizedY, z };
    glm::vec3 normal = normalVector(point, dPrim);

    //glm::vec3 ambient = lightColor * ambientStrength;

    glm::vec3 lightDir = normalize(lightPos - point);
    float diff = std::pow(std::max(dot(normal, lightDir), 0.f), properties.m);
    glm::vec3 diffuse = diff * lightColor;

    //glm::vec3 viewDir = normalize(viewPos - point);
    //glm::vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(std::max(dot(viewDir, reflectDir), 0.f), properties.m);
    //glm::vec3 specular = spec * glm::vec3(1.0f, 1.0f, 1.0f);

    //glm::vec3 finalColor = ambient + diffuse + specular;
    glm::vec3 finalColor = diffuse;
    finalColor.x = std::clamp(finalColor.x, 0.f, 1.f);
    finalColor.y = std::clamp(finalColor.y, 0.f, 1.f);
    finalColor.z = std::clamp(finalColor.z, 0.f, 1.f);

    return { finalColor.x, finalColor.y, finalColor.z, 1.f };
}

void AdaptiveRenderer::drawElipsoid(SDL_Texture* texture, int accuracy, const EllipsoidProperties& properties)
{
    int i = properties.accuracy - accuracy;

    Uint32* pixels;
    int pitch;

    assert(SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch) == 0);

    int blockCountX = windowWidth / blockSize;
    int blockCountY = windowHeight / blockSize;

    glm::vec4 color{};
    Uint32 rawColor{};

    for (int blockY = 0; blockY < blockCountY; blockY++)
    {
        for (int blockX = 0; blockX < blockCountX; blockX++)
        {
            bool fillRest{};

            int x = blockX * blockSize;
            int y = blockY * blockSize;

            int iBlock = 0;

            for (int fillY = y; fillY < y + blockSize && fillY < windowHeight; ++fillY)
            {
                for (int fillX = x; fillX < x + blockSize && fillX < windowWidth; ++fillX)
                {
                    if (iBlock == i)
                    {
                        color = computeColor(fillX, fillY, properties);
                        rawColor = SDL_MapRGBA(format,
                            static_cast<Uint8>(color.x * 255), static_cast<Uint8>(color.y * 255),
                            static_cast<Uint8>(color.z * 255), static_cast<Uint8>(color.w * 255));
                        fillRest = true;
                    }

                    if (fillRest)
                    {
                        pixels[fillY * windowWidth + fillX] = rawColor;
                    }

                    iBlock++;
                }
            }
        }
    }

    SDL_UnlockTexture(texture);
}