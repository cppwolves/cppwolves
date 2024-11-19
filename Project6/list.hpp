#ifndef LIST_HPP
#define LIST_HPP

template <typename NodeType> class List {
public:
  List<NodeType>() = default;
  virtual ~List<NodeType>() = default;

  virtual NodeType *head() = 0;
  virtual NodeType *tail() = 0;
};

#endif // LIST_HPP
