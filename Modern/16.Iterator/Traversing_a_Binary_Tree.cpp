#include <iostream>
#include <memory>
using namespace std;

template <typename T>
struct BinaryTree;

template <typename T>
struct Node : public enable_shared_from_this<Node<T>> {
  shared_ptr<Node<T>> left{nullptr}, right{nullptr};
  weak_ptr<Node<T>> parent{};
  weak_ptr<BinaryTree<T>> tree{};
  T value;

  explicit Node(const T& value) : value{value} {}
  Node(const T& value, Node<T>* const left, Node<T>* const right)
      : left{left->shared_from_this()}, right{right->shared_from_this()}, value{value} {}

  static shared_ptr<Node<T>> make_node(const T& value, Node<T>* const left, Node<T>* const right) {
    shared_ptr<Node<T>> node = make_shared<Node<T>>(value, left, right);
    left->parent = right->parent = node;
    return node;
  }

  void set_tree(BinaryTree<T>* const tree) {
    this->tree = tree->shared_from_this();
    if (left)
      left->set_tree(tree);
    if (right)
      right->set_tree(tree);
  }
};

template <typename T>
struct BinaryTree : public enable_shared_from_this<BinaryTree<T>> {
  shared_ptr<Node<T>> root{nullptr};
  explicit BinaryTree(Node<T>* const root) : root{root->shared_from_this()} {}

  static shared_ptr<BinaryTree<T>> make_tree(Node<T>* const root) {
    shared_ptr<BinaryTree<T>> tree = make_shared<BinaryTree<T>>(root);
    root->set_tree(tree.get());
    return tree;
  }

  struct PreOrderIterator {
    Node<T>* current;
    explicit PreOrderIterator(Node<T>* current) : current{current} {}

    bool operator!=(const PreOrderIterator& other) const { return current != other.current; }

    Node<T>& operator*() const { return *current; }
    Node<T>* operator->() const { return &this->operator*(); }

    PreOrderIterator& operator++() {
      if (current->right) {
        current = current->right.get();
        while (current->left)
          current = current->left.get();
      } else {
        auto p = current->parent.lock().get();
        while (p && current == p->right.get()) {
          current = p;
          p = p->parent.lock().get();
        }
        current = p;
      }
      return *this;
    }
    template <typename U>
    static PreOrderIterator begin(const BinaryTree<U>& tr) {
      Node<T>* node = tr.root.get();
      if (node)
        while (node->left)
          node = node->left.get();
      return PreOrderIterator{node};
    }
    template <typename U>
    static PreOrderIterator end(const BinaryTree<U>&) {
      return PreOrderIterator{nullptr};
    }
  };
  using iterator = PreOrderIterator;

  iterator begin() { return iterator::begin(*this); }
  iterator end() { return iterator::end(*this); }
};

void test() {
  auto b4 = make_shared<Node<int>>(4);
  auto b3 = make_shared<Node<int>>(3);
  auto b2 = make_shared<Node<int>>(2);
  auto b1 = Node<int>::make_node(1, b3.get(), b4.get());
  auto b0 = Node<int>::make_node(0, b1.get(), b2.get());
  auto tree = BinaryTree<int>::make_tree(b0.get());
  for (auto& e : *tree)
    cout << e.value << endl;
}

int main() {
  test();
  return 0;
}
