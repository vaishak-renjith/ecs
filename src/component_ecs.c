#include "component_ecs.h"
#include "entity_ecs.h"

void packComponentData(struct EntityManager* mgr, struct ComponentRegistry* reg, size_t aliveIdx, size_t deadIdx) { // move alive data to dead's positions
  // pack aliveIdx data into deadIdx
  reg->transforms_[deadIdx] = reg->transforms_[aliveIdx];
  reg->colors_[deadIdx] = reg->colors_[aliveIdx];
  reg->animations_[deadIdx] = reg->animations_[aliveIdx];
  reg->collisions_[deadIdx] = reg->collisions_[aliveIdx];

  // remove now-packed data
  reg->transforms_[aliveIdx] = (struct TransformComponent){0};
  reg->colors_[aliveIdx] = (struct ColorComponent){0};
  reg->animations_[aliveIdx] = (struct AnimationComponent){0};
  reg->collisions_[aliveIdx] = (struct CollisionComponent){};
}
