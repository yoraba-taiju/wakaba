/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once

#define DEF_RW(typ, name, read, write) \
  private: \
    typ name##_; \
  read: \
    [[nodiscard]] typ const& name() const { return this->name##_; }; \
  write: \
    [[nodiscard]] typ& name() { return this->name##_; };
#define DEF(typ, name) DEF_RW(typ, name, public, private)

#define VDEF_RW(typ, name, read, write) DEF_RW(::taiju::<typ>, name, read, write)
#define VDEF(typ, name) VDEF_RW(typ, name, public, private)
