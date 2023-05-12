
#include "spawner.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"


namespace our {

void SpawnerComponent::deserialize(const nlohmann::json &data) {
  if (!data.is_object())
    return;

            entity = World::addspawner();
            entity->parent = nullptr;
            entity->deserialize(data);
}
void SpawnerComponent::spawn(World *world)
  {
    Entity *newEntity=world->add(); 
  }
} // namespace our