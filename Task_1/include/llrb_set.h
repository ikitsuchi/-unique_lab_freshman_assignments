#ifndef LLRB_SET_H
#define LLRB_SET_H

#include <algorithm>

// 基于左倾红黑树 (下简称 LLRB) 实现的 set 类
template <typename Key>
class Set {
 private:
  enum NodeColor { kBlack = 0, kRed = 1 };

  struct Node {
    NodeColor color{kBlack};
    Node *lc{nullptr};
    Node *rc{nullptr};
    Key key;
    size_t size{0};

    Node() = default;
    Node(NodeColor color, Key key, size_t size)
        : color(color), key(key), size(size){};
  };

  Node *root_{nullptr};

  bool isRed(const Node *node) const {
    return node != nullptr && node->color == kRed;
  }
  NodeColor oppositeColor(NodeColor color) const {
    return color == kRed ? kBlack : kRed;
  }

  Node *rotateLeft(Node *node) const;
  Node *rotateRight(Node *node) const;
  Node *insert(Node *node, Key key) const;
  Node *deleteMin(Node *node) const;
  Node *deleteArbitrary(Node *node, Key key) const;
  Node *fixLlrb(Node *node) const;
  Node *spreadRedLeft(Node *node) const;
  Node *spreadRedRight(Node *node) const;
  Key getMin(Node *node) const;
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
typename Set<Key>::Node *Set<Key>::insert(Node *node, Key key) const {
  if (node == nullptr) return new Node(kRed, key, 1);
  if (std::less<Key>()(node->key, key)) {
    node->rc = insert(node->rc, key);
  } else if (std::less<Key>()(key, node->key)) {
    node->lc = insert(node->lc, key);
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
      node->key = getMin(node->rc);
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

// 返回 node 子树中最小值
template <typename Key>
Key Set<Key>::getMin(Node *node) const {
  for (; node->lc != nullptr; node = node->lc)
    ;
  return node->key;
}

template <typename Key>
void Set<Key>::insert(const Key key) {
  root_ = insert(root_, key);
  root_->color = kBlack;
}

#endif