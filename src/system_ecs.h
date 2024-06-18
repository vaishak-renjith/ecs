#pragma once
#include <SDL2/SDL.h>
#include "entity_ecs.h"
#include "component_ecs.h"

void updateTransforms(struct EntityManager* mgr, struct ComponentRegistry* reg);

void renderColors(struct EntityManager* mgr, struct ComponentRegistry* reg, SDL_Renderer* renderer);

void updateAnimations(struct EntityManager* mgr, struct ComponentRegistry* reg);

void updateCollisions(struct EntityManager* mgr, struct ComponentRegistry* reg);
