/* coding: utf-8 */
/**
 * wakaba
 *
 * Copyright 2020-, Kaede Fujisaki
 */
#pragma once
#include <type_traits>

#define DEF_RW(typ, name, read, write) \
  private: \
    typ name##_; \
  read: \
    [[nodiscard]] std::remove_reference<typ>::type const& name() const { \
      return this->name##_; \
    }; \
  write: \
    [[nodiscard]] std::remove_reference<typ>::type& name() { \
      return this->name##_; \
    };

#define DEF(typ, name) \
  private: \
    typ name##_; \
  public: \
    [[nodiscard]] std::remove_reference<typ>::type const& name() const { \
      return this->name##_; \
    };

#define VDEF_RW(typ, name, read, write) \
  private: \
    ::taiju::Value<std::remove_reference<std::remove_cv<typ>::type>::type> name##_; \
  read: \
    [[nodiscard]] std::remove_reference<std::remove_cv<typ>::type>::type const& name() const { return this->name##_.get().value(); }; \
  write: \
    [[nodiscard]] std::remove_reference<std::remove_cv<typ>::type>::type& name() { return this->name##_.get().value(); };

#define VDEF(typ, name) \
  private: \
    typename ::taiju::Value<typ> name##_; \
  public: \
    [[nodiscard]] std::remove_reference<typ>::type const& name() const { \
      return this->name##_.get().value(); \
    };
