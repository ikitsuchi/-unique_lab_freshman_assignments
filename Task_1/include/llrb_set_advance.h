#ifndef LLRB_SET_H
#define LLRB_SET_H

#include <algorithm>

// 基于左倾红黑树 (下简称 LLRB) 实现的 set 类
template <typename Key>
class Set {
 private:
  enum NodeColor { kBlack = 0, kRed = 1 };

  // 红黑树的节点
  struct Node {
    NodeColor color{kBlack};
    Node *parent{nullptr};
    Node *lc{nullptr};
    Node *rc{nullptr};
    Key key;
    size_t size{0};

    Node() = default;
    Node(NodeColor color, Key key, Node *parent, size_t size)
        : color(color), key(key), parent(parent), size(size){};

    // 返回当前节点的前驱
    Node *getPrev() {
      if (lc != nullptr) {
        return getMax(lc);
      } else {
        auto node = this;
        while (node->parent != nullptr && node->parent->lc == node)
          node = node->parent;
        return node->parent;
      }
    }

    // 返回当前节点的后继
    Node *getNext() {
      if (rc != nullptr) {
        return getMin(rc);
      } else {
        auto node = this;
        while (node->parent != nullptr && node->parent->rc == node)
          node = node->parent;
        return node->parent;
      }
    }
  };

  // 实现一些简单的迭代器功能
  struct Iterator {
    Node *node{nullptr};

    Iterator() = default;
    Iterator(Node *node) : node(node) {}

    Key &operator*() const { return node->key; }

    Iterator &operator++() {
      node = node->getNext();
      return *this;
    }

    Iterator &operator--() {
      node = node->getPrev();
      return *this;
    }

    bool operator==(const Iterator &it) { return node == it.node; }
    bool operator!=(const Iterator &it) { return node != it.node; }
  };

  friend Iterator;

  Node *root_{nullptr};

  bool isRed(const Node *node) const {
    return node != nullptr && node->color == kRed;
  }
  NodeColor oppositeColor(NodeColor color) const {
    return color == kRed ? kBlack : kRed;
  }

  Node *rotateLeft(Node *node) const;
  Node *rotateRight(Node *node) const;
  Node *insert(Node *node, Key key, Node *parent) const;
  Node *deleteMin(Node *node) const;
  Node *deleteArbitrary(Node *node, Key key) const;
  Node *fixLlrb(Node *node) const;
  Node *spreadRedLeft(Node *node) const;
  Node *spreadRedRight(Node *node) const;
  static Node *getMin(Node *node);
  static Node *getMax(Node *node);
  Node *getMin() const;
  Node *lowerBound(Node *node, Key key) const;
  Node *upperBound(Node *node, Key key) const;
  void colorFlip(Node *node) const;
  void maintainSize(Node *node) const;
  void clear(Node *node) const;
  bool findKey(Key key) const;

 public:
  bool empty() const { return size() == 0; }
  size_t size() const { return root_ == nullptr ? 0 : root_->size; }
  size_t count(const Key key) { return findKey(key) ? 1 : 0; }

  Set() = default;
  ~Set() { clear(root_); }

  size_t erase(const Key key);
  void clear();
  void insert(const Key key);

 public:
  typedef Iterator iterator;
  iterator begin() { return Iterator(getMin()); }
  iterator end() { return Iterator(nullptr); }
  iterator find(Key key) const {
    Node *node = root_;
    while (node != nullptr) {
      if (std::less<Key>()(node->key, key)) {
        node = node->rc;
      } else if (std::less<Key>()(key, node->key)) {
        node = node->lc;
      } else {
        return Iterator(node);
      }
    }
    return Iterator(nullptr);
  }
  iterator lower_bound(const Key &key) const;
  iterator upper_bound(const Key &key) const;
};

// 删除以 node 为根的树
template <typename Key>
void Set<Key>::clear(Node *node) const {
  if (node != nullptr) {
    clear(node->lc);
    clear(node->rc);
    delete node;
  }
}

// 左旋右倾的边
template <typename Key>
typename Set<Key>::Node *Set<Key>::rotateLeft(Node *node) const {
  Node *x = node->rc;
  node->rc = x->lc;
  x->lc = node;
  x->parent = node->parent;
  node->parent = x;
  std::swap(node->color, x->color);
  x->size = node->size;
  maintainSize(node);
  return x;
}

// 右旋左倾的边
template <typename Key>
typename Set<Key>::Node *Set<Key>::rotateRight(Node *node) const {
  Node *x = node->lc;
  node->lc = x->rc;
  x->rc = node;
  x->parent = node->parent;
  node->parent = x;
  std::swap(node->color, x->color);
  x->size = node->size;
  maintainSize(node);
  return x;
}

// 将与点相连的所有边反色
template <typename Key>
void Set<Key>::colorFlip(Node *node) const {
  node->color = oppositeColor(node->color);
  if (node->lc != nullptr) node->lc->color = oppositeColor(node->lc->color);
  if (node->rc != nullptr) node->rc->color = oppositeColor(node->rc->color);
}

// 维护 set 的大小
template <typename Key>
void Set<Key>::maintainSize(Node *node) const {
  node->size = 1;
  if (node->lc != nullptr) node->size += node->lc->size;
  if (node->rc != nullptr) node->size += node->rc->size;
}

// 修复 LLRB 结构
template <typename Key>
typename Set<Key>::Node *Set<Key>::fixLlrb(Node *node) const {
  if (!isRed(node->lc) && isRed(node->rc)) node = rotateLeft(node);  // 修正右倾
  if (isRed(node->lc) && isRed(node->lc->lc))
    node = rotateRight(node);  // 修正连续红边
  if (isRed(node->lc) && isRed(node->rc)) colorFlip(node);  // 消除 4-node
  maintainSize(node);
  return node;
}

// 将红边向左子树传递
template <typename Key>
typename Set<Key>::Node *Set<Key>::spreadRedLeft(Node *node) const {
  colorFlip(node);
  // 一定要先判断有没有右儿子，小心空指针
  if (node->rc != nullptr && isRed(node->rc->lc)) {  // 右子树出现连续红边，修正
    node->rc = rotateRight(node->rc);
    node = rotateLeft(node);
    colorFlip(node);
  }
  return node;
}

// 将红边向右子树传递
template <typename Key>
typename Set<Key>::Node *Set<Key>::spreadRedRight(Node *node) const {
  colorFlip(node);
  // 一定要先判断有没有左儿子，小心空指针
  if (node->lc != nullptr && isRed(node->lc->lc)) {  // 左子树出现连续红边，修正
    node = rotateRight(node);
    colorFlip(node);
  }
  return node;
}

// 把一个 key 插入 set
template <typename Key>
typename Set<Key>::Node *Set<Key>::insert(Node *node, Key key,
                                          Node *parent) const {
  if (node == nullptr) return new Node(kRed, key, parent, 1);
  if (std::less<Key>()(node->key, key)) {
    node->rc = insert(node->rc, key, node);
  } else if (std::less<Key>()(key, node->key)) {
    node->lc = insert(node->lc, key, node);
  }
  return fixLlrb(node);
}

// 删除 set 的最小值
template <typename Key>
typename Set<Key>::Node *Set<Key>::deleteMin(Node *node) const {
  // 访问到了最小值叶子
  if (node->lc == nullptr) {
    delete node;
    return nullptr;
  }
  // 红边传递不下去了，向父亲借
  if (!isRed(node->lc) && !isRed(node->lc->lc)) {
    node = spreadRedLeft(node);
  }
  node->lc = deleteMin(node->lc);
  return fixLlrb(node);
}

// 删除 set 中任意值
template <typename Key>
typename Set<Key>::Node *Set<Key>::deleteArbitrary(Node *node, Key key) const {
  if (std::less<Key>()(key, node->key)) {  // key < node->key
    if (!isRed(node->lc) && !isRed(node->lc->lc)) node = spreadRedLeft(node);
    node->lc = deleteArbitrary(node->lc, key);
  } else {  // key >= node->key
    // 原本 > 和 = 是分开写的，结果发现大部分代码都一样就合起来了
    if (isRed(node->lc)) node = rotateRight(node);
    // 发现到叶子节点了，可以直接删
    if (node->lc == nullptr && node->rc == nullptr) {
      delete node;
      return nullptr;
    }
    if (!isRed(node->rc) && !isRed(node->rc->lc)) node = spreadRedRight(node);
    if (node->key == key) {
      node->key = getMin(node->rc)->key;
      node->rc = deleteMin(node->rc);
    } else {
      node->rc = deleteArbitrary(node->rc, key);
    }
  }
  return fixLlrb(node);
}

// 清空 set
template <typename Key>
void Set<Key>::clear() {
  clear(root_);
  root_ = nullptr;
}

// 找指定的 key
template <typename Key>
bool Set<Key>::findKey(Key key) const {
  Node *node = root_;
  while (node != nullptr) {
    if (std::less<Key>()(node->key, key)) {
      node = node->rc;
    } else if (std::less<Key>()(key, node->key)) {
      node = node->lc;
    } else {
      return true;
    }
  }
  return false;
}

// 删除任意值
template <typename Key>
size_t Set<Key>::erase(Key key) {
  if (findKey(key)) {
    root_ = deleteArbitrary(root_, key);
    if (root_ != nullptr) root_->color = kBlack;
    return 1;
  } else {
    return 0;
  }
}

// 插入新的值
template <typename Key>
void Set<Key>::insert(const Key key) {
  root_ = insert(root_, key, nullptr);
  root_->color = kBlack;
}

template <typename Key>
typename Set<Key>::Node *Set<Key>::getMin(Node *node) {
  if (node == nullptr) return nullptr;
  for (; node->lc != nullptr; node = node->lc)
    ;
  return node;
}

template <typename Key>
typename Set<Key>::Node *Set<Key>::getMax(Node *node) {
  for (; node->rc != nullptr; node = node->rc)
    ;
  return node;
}

template <typename Key>
typename Set<Key>::Node *Set<Key>::upperBound(Node *node, Key key) const {
  if (std::less<Key>()(key, node->key)) {
    if (node->lc == nullptr) return node;
    return upperBound(node->lc, key);
  } else if (std::less<Key>()(node->key, key)) {
    if (node->rc == nullptr) return nullptr;
    return upperBound(node->rc, key);
  } else {
    return node->getNext();
  }
}

template <typename Key>
typename Set<Key>::Node *Set<Key>::lowerBound(Node *node, Key key) const {
  if (std::less<Key>()(key, node->key)) {
    if (node->lc == nullptr) return node;
    return lowerBound(node->lc, key);
  } else if (std::less<Key>()(node->key, key)) {
    if (node->rc == nullptr) return nullptr;
    return lowerBound(node->rc, key);
  } else {
    return node;
  }
}

template <typename Key>
typename Set<Key>::iterator Set<Key>::upper_bound(const Key &key) const {
  auto node = upperBound(root_, key);
  if (node == nullptr)
    return Iterator(nullptr);
  else
    return Iterator(node);
}

template <typename Key>
typename Set<Key>::Node *Set<Key>::getMin() const {
  return getMin(root_);
}

template <typename Key>
typename Set<Key>::iterator Set<Key>::lower_bound(const Key &key) const {
  auto node = lowerBound(root_, key);
  if (node == nullptr)
    return Iterator(nullptr);
  else
    return Iterator(node);
}

#endif