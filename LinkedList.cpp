#include "LinkedList.h"
#include <sstream>
#include <iostream> // for debug

LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

LinkedList::~LinkedList() {
    clear();
}

// Copy constructor
LinkedList::LinkedList(const LinkedList& other)
    : head(nullptr), tail(nullptr), size(0) {
  Node* current = other.head;
  while (current != nullptr) {
    addBack(current->getTile());
    current = current->getNext();
  }
}

// Copy assignment operator
LinkedList& LinkedList::operator=(const LinkedList& other) {
  if (this != &other) {
    clear();
    Node* current = other.head;
    while (current != nullptr) {
      addBack(current->getTile());
      current = current->getNext();
    }
  }
  return *this;
}

// Move constructor
LinkedList::LinkedList(LinkedList&& other)
    : head(other.head), tail(other.tail), size(other.size) {
  other.head = nullptr;
  other.tail = nullptr;
  other.size = 0;
}

// Move assignment operator
LinkedList& LinkedList::operator=(LinkedList&& other) {
  if (this != &other) {
    // Clean up the current state
    clear();  
    head = other.head;
    tail = other.tail;
    size = other.size;
    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
  }
  return *this;
}

void LinkedList::addBack(Tile* tile) {
    Node* newNode = new Node(tile);
    newNode->setNext(nullptr);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->setNext(newNode);
        tail = newNode;
    }
    size++;
}

void LinkedList::addFront(Tile* tile) {
    Node* newNode = new Node(tile);
    newNode->setNext(head);
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }
    size++;
}

Tile* LinkedList::remove(Tile* tile) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr && *(current->getTile()) != *tile) {
        previous = current;
        current = current->getNext();
    }

    if (current == nullptr) {
        return nullptr;
    }

    if (previous == nullptr) {
        head = current->getNext();
    } else {
        previous->setNext(current->getNext());
    }

    if (current == tail) {
        tail = previous;
    }

    Tile* removedTile = current->getTile();
    current->setTile(nullptr);
    delete current;

    size--;

    return removedTile;
}

Tile* LinkedList::removeFront() {
    if (head == nullptr) {
        return nullptr;
    }

    Node* oldHead = head;
    head = head->getNext();
    if (head == nullptr) {
        tail = nullptr;
    }

    Tile* removedTile = oldHead->getTile();
    oldHead->setTile(nullptr);
    delete oldHead;

    size--;

    return removedTile;
}

Tile* LinkedList::removeEnd() {
    if (head == nullptr) {

        throw std::underflow_error("List is empty");
        // return nullptr;
    }

    if (head == tail) {
        Tile* removedTile = head->getTile();
        delete head;
        head = nullptr;
        tail = nullptr;
        size--;
        return removedTile;
    }

    Node* current = head;
    while (current->getNext() != tail) {
        current = current->getNext();
    }

    Tile* removedTile = tail->getTile();
    delete tail;
    tail = current;
    tail->setNext(nullptr);

    size--;

    return removedTile;
}

void LinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->getNext();
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

Node* LinkedList::getHead() const {
    return head;
}

int LinkedList::getLength() const {
    return size;
}

std::string LinkedList::toString() const {
    std::ostringstream oss;
    Node* current = head;
    while (current != nullptr) {
        oss << *(current->getTile());
        if (current->getNext() != nullptr) {
            oss << ", ";
        }
        current = current->getNext();
    }
    return oss.str();
}

bool LinkedList::isEmpty() const {
    return head == nullptr;
}

Tile* LinkedList::get(int index) const {
  if (index < 0 || index >= size) {
    throw std::out_of_range("Index out of range");
  }
  Node* current = head;
  for (int i = 0; i < index; ++i) {
    current = current->getNext();
  }
  return current->getTile();
}

void LinkedList::deleteFront() { removeFront(); }

void LinkedList::deleteBack() { removeEnd(); }

void LinkedList::addAt(Tile* tile, int index) {
  if (index < 0 || index > size) {
    throw std::out_of_range("Index out of range");
  }

  if (index == 0) {
    addFront(tile);
    return;
  }

  if (index == size) {
    addBack(tile);
    return;
  }

  Node* newNode = new Node(tile);
  Node* current = head;
  for (int i = 0; i < index - 1; ++i) {
    current = current->getNext();
  }
  newNode->setNext(current->getNext());
  current->setNext(newNode);
  size++;
}

void LinkedList::deleteAt(int index) {
  if (index < 0 || index >= size) {
    throw std::out_of_range("Index out of range");
  }

  if (index == 0) {
    removeFront();
    return;
  }

  if (index == size - 1) {
    removeEnd();
    return;
  }

  Node* current = head;
  for (int i = 0; i < index - 1; ++i) {
    current = current->getNext();
  }
  Node* toDelete = current->getNext();
  current->setNext(toDelete->getNext());
  toDelete->setTile(nullptr);
  delete toDelete;
  size--;
}
