#ifndef FIBONACCI_PRIORITY_QUEUE_H
#define FIBONACCI_PRIORITY_QUEUE_H

#include <algorithm>

// 利用 fibonacci heap 构造大根堆，从而实现 std::priority_queue 的基本功能
template <typename Key>
class PriorityQueue {
 private:
  struct Node {
    Node *prev{nullptr};
    Node *nxt{nullptr};
    Node *child{nullptr};
    Key key;
    int degree{0};

    Node() = default;
    Node(Key key) : key(key) {}
  };

  Node *maximum_{nullptr};
  size_t size_{0};

  void insertListNode(Node *target, Node *node) const;
  Node *merge(Node *x, Node *y) const;
  void consolidate();
  void clear(Node *node);
  void extractNode(Node *node) const;

 public:
  Key top() const { return maximum_->key; }
  bool empty() const { return maximum_ == nullptr; }
  size_t size() const { return size_; }

  PriorityQueue() = default;
  ~PriorityQueue() { clear(maximum_); };

  void push(Key key);
  void pop();
};

// 把 node 插入为链表中 target 的后继
template <typename Key>
void PriorityQueue<Key>::insertListNode(Node *target, Node *node) const {
  target->nxt->prev = node;
  node->nxt = target->nxt;
  target->nxt = node;
  node->prev = target;
}

// 将两个度数相同的堆合并
template <typename Key>
typename PriorityQueue<Key>::Node *PriorityQueue<Key>::merge(Node *x,
                                                             Node *y) const {
  if (std::greater<Key>()(y->key, x->key))
    std::swap(x, y);  //  确保 x 是 key 更大的那一个
  if (x->child != nullptr) {
    insertListNode(x->child, y);
  } else {
    x->child = y;
    y->prev = y->nxt = y;
  }
  ++x->degree;
  return x;
}

template <typename Key>
void PriorityQueue<Key>::push(Key key) {
  auto node = new Node(key);
  node->prev = node->nxt = node;
  if (maximum_ == nullptr) {
    maximum_ = node;
  } else {
    insertListNode(maximum_, node);
    maximum_ = std::greater<Key>()(key, maximum_->key) ? node : maximum_;
  }
  ++size_;
}

// 把一个堆从链表里拆出来
template <typename Key>
void PriorityQueue<Key>::extractNode(Node *node) const {
  node->prev->nxt = node->nxt;
  node->nxt->prev = node->prev;
  node->nxt = node->prev = node;
}

template <typename Key>
void PriorityQueue<Key>::pop() {
  if (maximum_ == nullptr) return;
  if (maximum_->child != nullptr) {  // 需要把孩子加到根节点链表里
    Node *node = maximum_->child;
    if (maximum_->prev != maximum_) {
      node->prev->nxt = maximum_->nxt;
      node->prev = maximum_->prev;
    }
    delete maximum_;
    maximum_ = node;
  } else {
    if (maximum_->prev != maximum_) {
      auto x = maximum_->prev;
      extractNode(maximum_);
      delete maximum_;
      maximum_ = x;
    } else {
      delete maximum_;
      maximum_ = nullptr;
    }
  }
  consolidate();
  --size_;
}

// 维护没有相同度数的堆的性质
template <typename Key>
void PriorityQueue<Key>::consolidate() {
  // 把所有堆拆出来，放到数组里存起来
  Node *degree[128] = {};
  Node *now;
  // 当最大值是空指针时说明所有子树都被拆出来、合并完了
  // 在这里最大值指针被用作一个不断向后扫的遍历的工具
  while (maximum_ != nullptr) {
    now = maximum_;
    maximum_ = (maximum_->nxt == maximum_) ? (nullptr) : (maximum_->nxt);  //
    extractNode(now);
    // 只要存在度数相同的堆就不断合并
    while (degree[now->degree] != nullptr) {
      Node *same = degree[now->degree];
      degree[now->degree] = nullptr;
      now = merge(now, same);
    }
    degree[now->degree] = now;
  }
  // 把所有堆再塞回链表，顺便更新最大值指针
  for (auto x : degree) {
    if (x == nullptr) continue;
    if (maximum_ == nullptr) {
      maximum_ = x;
    } else {
      insertListNode(maximum_, x);
      maximum_ = std::greater<Key>()(x->key, maximum_->key) ? x : maximum_;
    }
  }
}

// 用来释放 new 的内存空间
template <typename Key>
void PriorityQueue<Key>::clear(Node *node) {
  // 把循环链表断开
  // 小心野指针！！！
  if (node != nullptr) node->prev->nxt = nullptr;
  while (node != nullptr) {
    auto x = node->nxt;
    clear(node->child);
    delete node;
    node = x;
  }
}

#endif