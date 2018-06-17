#ifndef FUSION_XML_KEY_H
#define FUSION_XML_KEY_H

#include <string>

//#include <functional>

template <typename S, typename C>
class Key {
public:
  Key() = default;
  Key(const S& _name) : name(_name) {};
  Key(const S _name, const S& _id, bool _unify) : name(_name), id(_id), unify(_unify) {};
  ~Key() = default;
  bool operator<(const Key& k) const { return name < k.name || (name == k.name && id < k.id); };
  bool operator==(const Key& k) const { return name == k.name && id == k.id && unify == k.unify; };
  const S& Name() const { return name; };
  const S& ID() const { return id; };
  bool Unify() const { return unify; };

private:
  S name;
  S id;
  bool unify{ false };
};

namespace std {
  template <typename S, typename  C>
  struct hash<Key<S, C>> : public unary_function<Key<S, C>, size_t> {
    size_t operator()(const Key<S, C>& value) const {
      return std::hash<S>{}(value.Name() + value.ID());
    }
  };

  template <typename S, typename  C>
  struct equal_to<Key<S, C>> : public unary_function<Key<S, C>, bool> {
    bool operator()(const Key<S, C>& x, const Key<S, C>& y) const {
      return x == y;
    }
  };
}

#endif