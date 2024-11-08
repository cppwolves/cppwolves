#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP

template <typename NodeType> class ListNode {
public:
  ListNode() = default;
  virtual ~ListNode() = default;

  virtual ListNode *link(ListNode *node);
  virtual ListNode *unlink();

  virtual NodeType *next() { return (NodeType *)_next; };
  virtual NodeType *previous() { return (NodeType *)_previous; };

protected:
  ListNode *_next{nullptr};
  ListNode *_previous{nullptr};
};

#endif // !LIST_NODE_HPP

template <typename NodeType>
ListNode<NodeType> *ListNode<NodeType>::link(ListNode *node) {
  if (!node) {
    this->_next = nullptr;
    return nullptr;
  }

  // Disconnect the list to insert the new list
  ListNode *end = this->_next;

  // Connect the end of the old and start of the new list
  this->_next = node;
  node->_previous = this;

  ListNode *next = node->_next;

  // Move to the last node of the new list.
  while (next && next->_next) {
    next = next->_next;
  }
  // Connect the end of the new to the start of the old list
  if (next) {
    next->_next = end;
  }
  if (end) {
    end->_previous = next;
  }

  return node;
}

template <typename NodeType> ListNode<NodeType> *ListNode<NodeType>::unlink() {
  ListNode *previous = this->_previous;
  ListNode *next = this->_next;
  if (previous) {
    next = previous->link(next);
  } else if (next) {
    next->_previous = nullptr;
  }
  return next;
}
