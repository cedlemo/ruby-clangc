/*
 * Ressources:
 * https://en.wikipedia.org/wiki/Virtual_inheritance
 * http://stackoverflow.com/questions/21558/in-c-what-is-a-virtual-base-class
 * */
class Animal {
 public:
  virtual void eat();
};

// Two classes virtually inheriting Animal:
class Mammal : public virtual Animal {
 public:
  virtual void breathe();
};

class WingedAnimal : public virtual Animal {
 public:
  virtual void flap();
};

// A bat is still a winged mammal
class Bat : public Mammal, public WingedAnimal {
};
