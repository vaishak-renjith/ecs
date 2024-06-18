#pragma once
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

// ENTITY STUFF
typedef size_t Entity;
#define MAX_ENTITIES 10001

struct EntityManager {
  Entity entity_count_;
  Entity free_usigs_[MAX_ENTITIES];

  uint64_t component_tracker_[MAX_ENTITIES]; // 64 possible components

  size_t idx_to_entity_[MAX_ENTITIES];
  size_t entity_to_idx_[MAX_ENTITIES];
};

// FUNCTION DEFINITIONS
void initEntityManager(struct EntityManager* mgr);
void debug(struct EntityManager* mgr);

Entity addEntity(struct EntityManager* mgr, long component_tracker);
void removeEntity(struct EntityManager* mgr, Entity entity);

void addComponents(struct EntityManager* mgr, Entity entity, long components);
void removeComponents(struct EntityManager* mgr, Entity entity, long components);
