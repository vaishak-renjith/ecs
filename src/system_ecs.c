#include <SDL2/SDL.h>
#include "system_ecs.h"
#include "entity_ecs.h"
#include "component_ecs.h"

void updateTransforms(struct EntityManager* mgr, struct ComponentRegistry* reg) {
  for (int i = 0; i < mgr->entity_count_; i++) {
    Entity entity = mgr->idx_to_entity_[i];
    size_t idx = mgr->entity_to_idx_[entity];

    if (!(bitat(mgr->component_tracker_[idx], trans))) continue;

    reg->transforms_[idx].pos_y += reg->transforms_[idx].vel_y*0.1;
    reg->transforms_[idx].pos_x += reg->transforms_[idx].vel_x*0.1;

    //printf("Entity %zu, Idx %zu: [%f,%f]\n", entity, idx, reg->transforms_[idx].pos_x, reg->transforms_[idx].pos_y);
  }
}

void renderColors(struct EntityManager* mgr, struct ComponentRegistry* reg, SDL_Renderer* renderer) {
  for (int i = 0; i < mgr->entity_count_; i++) {
    Entity entity = mgr->idx_to_entity_[i];
    size_t idx = mgr->entity_to_idx_[entity];

    if (!(bitat(mgr->component_tracker_[idx], trans))) continue;
    if (!(bitat(mgr->component_tracker_[idx], color))) continue;

    SDL_SetRenderDrawColor(renderer, reg->colors_[idx].r, reg->colors_[idx].g, reg->colors_[idx].b, reg->colors_[idx].a);
    SDL_Rect rect = {reg->transforms_[idx].pos_x, reg->transforms_[idx].pos_y, 5, 5};
    SDL_RenderDrawRect(renderer, &rect);

    //printf("Entity %zu, Idx %zu: [%d,%d,%d]\n", entity, idx, reg->colors_[idx].r, reg->colors_[idx].g, reg->colors_[idx].b);
  }

  SDL_RenderPresent(renderer);
}

void updateAnimations(struct EntityManager* mgr, struct ComponentRegistry* reg) {
  // do nothing
}

void updateCollisions(struct EntityManager* mgr, struct ComponentRegistry* reg) {
  for (int i = 0; i < mgr->entity_count_; i++) {
    Entity entity = mgr->idx_to_entity_[i];
    size_t idx = mgr->entity_to_idx_[entity];

    if (!(bitat(mgr->component_tracker_[idx], trans))) continue;

    if (reg->transforms_[idx].pos_y <= 0) reg->transforms_[idx].vel_y*=-1;
    else if (reg->transforms_[idx].pos_y >= 500) reg->transforms_[idx].vel_y*=-1;

    if (reg->transforms_[idx].pos_x <= 0) reg->transforms_[idx].vel_x*=-1;
    if (reg->transforms_[idx].pos_x >= 500) reg->transforms_[idx].vel_x*=-1;
  }
}
