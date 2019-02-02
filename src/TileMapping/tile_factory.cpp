#include "tile_factory.hpp"

TileFactory::TileFactory()
{
}

TileFactory::~TileFactory()
{
}

Entity TileFactory::build(int type, glm::vec2 position, glm::vec2 scale)
{
  Program *program = new Program(shader_path("sprite.vert"), shader_path("sprite.frag"));
  SpriteComponent *sprite = new SpriteComponent(program, new Texture());
  TransformComponent *transform = new TransformComponent(position, scale, 0.0f);

  ColorComponent *colour;
  if (type == 0) {
    colour = new ColorComponent(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }
  else if (type == 1) {
    colour = new ColorComponent(glm::vec4(0.8f, 0.8f, 0.4f, 1.0f));
  }

  Entity e;
  e.setComponent<SpriteComponent>(sprite);
  e.setComponent<TransformComponent>(transform);
  e.setComponent<ColorComponent>(colour);
  return e;
}
