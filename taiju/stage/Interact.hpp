/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#include <memory>

namespace taiju {

// Witches x Bullets
class Sora;
class Chitose;
class Momiji;
class Kaede;
class Bullet;

void interact(std::shared_ptr<Sora> sora, std::shared_ptr<Bullet> bullet);
void interact(std::shared_ptr<Chitose> chitose, std::shared_ptr<Bullet> bullet);
void interact(std::shared_ptr<Momiji> kaede, std::shared_ptr<Bullet> bullet);
void interact(std::shared_ptr<Kaede> momiji, std::shared_ptr<Bullet> bullet);

// Witches x Objects

// Bullets x Objects
}
