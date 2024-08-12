#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
 public:
  LinkedList();
  ~LinkedList();
  LinkedList(const LinkedList& other);
  LinkedList& operator=(const LinkedList& other);
  LinkedList(LinkedList&& other);
  LinkedList& operator=(LinkedList&& other);

  void addBack(Tile* tile);
  void addFront(Tile* tile);
  Tile* remove(Tile* tile);
  Tile* removeFront();
  Tile* removeEnd();
  void clear();

  Node* getHead() const;
  int getLength() const;
  bool isEmpty() const;
  // std::string toString();
  std::string toString() const;

  Tile* get(int index) const;
  void deleteFront();
  void deleteBack();
  void addAt(Tile* tile, int index);
  void deleteAt(int index);

 private:
  Node* head;
  Node* tail;
  int size;
};

#endif // ASSIGN2_LINKEDLIST_H
