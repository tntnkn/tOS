#ifndef ENTITY_H_SENTRY
#define ENTITY_H_SENTRY


#include "utils.h"

/* 
 *  this is a most common anscestor for things in this framework.
 *  every thing provided by the framework has it as a virtual base.
 *  it is not explicitly mentioned in a member initializer list
 *    of some of them as it has a default construcor.
 */
class Entity {
  protected:
    Entity() {};  
};

class Drawable : virtual protected Entity {
  protected:
    using coord_t = Utils::coord_t;
    
    coord_t m_top_left, m_bot_right;
    
    Drawable() {}; 
    Drawable(coord_t top_left, coord_t bot_right) : m_top_left(top_left), m_bot_right(bot_right) {};
    
  public:
    virtual void redrawScreen() = 0;
    virtual void redrawBox(coord_t topleft, coord_t botright) = 0;
    //virtual void redraw() = 0;  
};


class Executable : virtual protected Entity {
  protected:
    Executable() {}; 
    //can be put as a foreground or background process into tOS?   
};

class Settable : virtual protected Entity {
  protected:
    Settable() {};    
};

class HasParent : virtual protected Entity {
  protected:
    Entity *m_parent = nullptr;

    HasParent() {};
    HasParent(Entity *parent) : m_parent(parent) {};
    
    Entity *       getParent()       { return                            m_parent; };
    const Entity * getParent() const { return const_cast<const Entity *>(m_parent); };   
};

template<template<typename T, typename A> class C>
class HasChildren : virtual protected Entity {
  protected:
    C<typename C::value_type, typename C::allocator_type> m_children;
    HasChildren() {};
    HasChildren(T *m_children, size_t len) {};
};



class SystemService : virtual protected Entity {
  protected:
    SystemService() {};
    //kinda handles an access to/from tOS
};


#endif
