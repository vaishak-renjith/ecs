#include "entity_ecs.h"
#include "component_ecs.h"

void initEntityManager(struct EntityManager* mgr) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    mgr->free_usigs_[i] = i;
    mgr->idx_to_entity_[i] = -1;
    mgr->entity_to_idx_[i] = -1;
  }
}

void debug(struct EntityManager* mgr) {
  printf("Entity Count\n%zu\n", mgr->entity_count_);

  printf("Free universal signatures\n");
  for (int i = 0; i < MAX_ENTITIES; i++) { printf("%02zu ", mgr->free_usigs_[i]); }
  printf("\n");

  printf("Idx -> Entity\n");
  for (int i = 0; i < MAX_ENTITIES; i++) { printf("%02ld ", (mgr->idx_to_entity_[i] == (size_t)(-1) ? (long)-1 : (long)mgr->idx_to_entity_[i])); }
  printf("\n");

  printf("Entity -> Idx\n");
  for (int i = 0; i < MAX_ENTITIES; i++) { printf("%02ld ", (mgr->entity_to_idx_[i] == (size_t)(-1) ? (long)-1 : (long)mgr->entity_to_idx_[i])); }
  printf("\n");

  printf("Component trackers\n");
  for (int i = 0; i < MAX_ENTITIES; i++) { printf("%02lu ", mgr->component_tracker_[i]); }
  printf("\n\n");
}

Entity addEntity(struct EntityManager* mgr, long component_tracker) {
  // stop if entity cap filled
  assert(mgr->entity_count_ != MAX_ENTITIES);

  // get available universal signature
  Entity usig = mgr->free_usigs_[0];
  // update signature array to reflect the now unavailable signature
  mgr->free_usigs_[0] = mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1];
  mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1] = usig;

  // set values
  mgr->component_tracker_[mgr->entity_count_] = component_tracker;
  mgr->idx_to_entity_[mgr->entity_count_] = usig;
  mgr->entity_to_idx_[usig] = mgr->entity_count_;

  // update entity count
  mgr->entity_count_++;
  return usig;
}

// NOTE: removing an entity DOES NOT update the component data.
// PackComponentData MUST BE EXPLICITLY CALLED, or else BAD THINGS HAPPEN!!!
void removeEntity(struct EntityManager* mgr, Entity entity) {
  // stop if entity is not in world
  assert(mgr->entity_count_ != 0 && mgr->entity_to_idx_[entity] != -1);

  // printf("removing %zu\n", entity);
  mgr->entity_count_--;


  // pack array data
  size_t pack_idx = mgr->entity_to_idx_[entity];
  mgr->entity_to_idx_[mgr->idx_to_entity_[mgr->entity_count_]] = pack_idx;
  mgr->idx_to_entity_[pack_idx] = mgr->idx_to_entity_[mgr->entity_count_];
  mgr->component_tracker_[pack_idx] = mgr->component_tracker_[mgr->entity_count_];


  // remove duplicated data
  mgr->entity_to_idx_[entity] = -1;
  mgr->idx_to_entity_[mgr->entity_count_] = -1;
  mgr->component_tracker_[mgr->entity_count_] = 0;


  // update universal signatures to reflect freed signature
  int entity_usig_idx = -1;
  for (int i = MAX_ENTITIES; i >= 0; i--) {
    if (mgr->free_usigs_[i] != entity) continue;
    entity_usig_idx = i;
  }
  Entity last_used_usig = mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1];
  mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1] = mgr->free_usigs_[entity_usig_idx];
  mgr->free_usigs_[entity_usig_idx] = last_used_usig;
}

void addComponents(struct EntityManager* mgr, Entity entity, long components) {
  size_t index = mgr->entity_to_idx_[entity];
  mgr->component_tracker_[index] |= components;
}

void removeComponents(struct EntityManager* mgr, Entity entity, long components) {
  size_t index = mgr->entity_to_idx_[entity];
  mgr->component_tracker_[index] &= (~components);
}

// <<<<<<< TESTING >>>>>>>
/*
int main() {
  struct EntityManager mgr = {0, {0}, {0}, {0}, {0}};
  initEntityManager(&mgr);
  debug(&mgr);

  Entity a = addEntity(&mgr, 0);
  Entity b = addEntity(&mgr, 0);
  Entity c = addEntity(&mgr, 0);
  Entity d = addEntity(&mgr, 0);

  addComponents(&mgr, a, TRANS);
  addComponents(&mgr, b, 0);
  addComponents(&mgr, c, COLOR|ANIMA);
  addComponents(&mgr, d, TRANS|COLOR|ANIMA);
  debug(&mgr);


  for (int i = 0; i < 3; i++) {
    addEntity(&mgr, (long)mgr.free_usigs_[0]*10);
    debug(&mgr);
  }

  removeEntity(&mgr, 0);
  debug(&mgr);

  removeEntity(&mgr, 9);
  debug(&mgr);

  addEntity(&mgr, (long)mgr.free_usigs_[0]*10);
  debug(&mgr);

  removeEntity(&mgr, 8);
  debug(&mgr);

  removeEntity(&mgr, 7);
  debug(&mgr);
}
*/
