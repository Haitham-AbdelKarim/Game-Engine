#include "world.hpp"
#include <iostream>
using namespace std;

namespace our {

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json& data, Entity* parent){
        if(!data.is_array()) return;

        // Iterate over each entity data in the JSON array
        for(const auto& entityData : data){
            //TODO: (Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".
            Entity* newEntity = World::add(); // Create a new entity and add it to the world
            newEntity->parent = parent; // Set the parent of the new entity to the given parent, if not null
            newEntity->deserialize(entityData); // Deserialize the entity using its JSON data

            // If the entity has children, recursively deserialize them using this function
            if(entityData.contains("children")){
                //TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                this->deserialize(entityData["children"], newEntity);
            }
        }
    }

}