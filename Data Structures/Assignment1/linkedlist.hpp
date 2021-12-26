#ifndef LINKEDLIST_HPP
#define	LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
private:
    Node<T>* head; 
    int length;  
public:

    LinkedList();
    LinkedList(const LinkedList<T>& ll);
    LinkedList<T>& operator=(const LinkedList<T>& ll);
    ~LinkedList();


    Node<T>* getHead() const;
    Node<T>* first() const;
    Node<T>* findPrev(const T& data) const;
    Node<T>* findNode(const T& data) const;
    void insertNode(Node<T>* prev, const T& data); 
    void deleteNode(Node<T>* prevNode);
    void clear();
    size_t getLength() const;
    void print() const;
    void swap(int index1, int index2);
};

template <class T>
void LinkedList<T>::print() const {
    const Node<T>* node = first();
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout << std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */

template <class T> 
LinkedList<T>:: LinkedList()
{
head = new Node<T>(T());
 


}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll){
    
 head = new Node<T>(T()); 
 length=0;

  *this = ll;                        //


}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll){
    if (this != &ll) {
    clear();     
    length=0;       // clear the former content
    const Node<T>* r = ll.first(); // r = the zeroth of rhs
    Node<T>* p = getHead();    // p starts from the first of lhs
    
    while (r) {         
       insertNode(p,r->getData()); //implement et
       r = r->getNext();              // move r forward
       p = p->getNext();              // move p forward
    }
  }
  return *this;     // return lhs for casca


}


template <class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data)
{
    // Now p never be NULL as a list has at least one node (dummy). 
// That is, we have only Case-1. 
// To insert to the start of the list, p should point to the dummy node.

  Node<T>* newNode = new Node<T>(data);
  newNode->setNext(prev->getNext());
  prev->setNext(newNode);
  length++;

}

template <class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode)
{
  //Node<T>* p = findPrev(data);
    

  if (prevNode) {  // if p is not NULL ? keeps the address of the prev.   
    Node<T>* tmp = prevNode->getNext();  // address of the node to delete
    prevNode->setNext(tmp->getNext()); // p’s next will be next of to-be-deleted
    delete tmp;
    length--;     // delete it
  }
}



template <class T>

Node<T>* LinkedList<T>::findPrev(const T& data)const {
  Node<T>* p = getHead();
  // start from zeroth: if we delete the first, its prev. will be zeroth
  while (p->getNext()) {  // while node pointed by p is not the last  one
    // if data in the next node of the one pointed by p, 
    // then return p: it points to prev. of what we will delete 
    if ((p->getNext())->getData() == data)
      return p;
    p = p->getNext();
  }
  return NULL;       // if we are here, not found…! 
}


template <class T>
Node<T>* LinkedList<T>::findNode(const T& data)const {
  Node<T>* p = first();  // start with the first (after dummy)

  while (p) {   // while p shows some node
  // if data in the node pointed by p is what we search for, 
  // then return its address
    if (p->getData()== data)
      return p; 
    p = p->getNext();
  }
  return NULL;  // if we are here, not found…! 
}




template <class T>
void LinkedList<T>::clear()
{
  while(first() != NULL)    // while list is not empty
    deleteNode(getHead()); 
    
    length=0;   // delete the first nod
}

template <class T>
size_t LinkedList<T>::getLength() const{
    return length;
}

template <class T>
Node<T>* LinkedList<T>::getHead() const {return head;}

template <class T>
Node<T>* LinkedList<T>::first() const{return head->getNext();}


template <class T>
LinkedList<T>::~LinkedList()
{
   clear();       // delete all the elements, except dummy
  delete head;  // delete dumm

}


template <class T>
void LinkedList<T>::swap(int index1, int index2){
  
  int pos=-1;
  Node<T>* prev1=NULL;
  Node<T>* prev2=NULL;
  Node<T>* node1=NULL;
  Node<T>* node2=NULL;
  Node<T>* p=getHead();
  Node<T>* temp=NULL;
/***********************************************/
  if(index1<length && index2<length){
  
  while(p){
 
    
    if(pos==index1-1)
    {
      prev1=p;
      break;
    }
    p=p->getNext();
    pos++;
  }

  if(prev1->getNext()){
    node1=prev1->getNext();
  }
  /******************************************/
  p=getHead();
  int pos2=-1;
  while(p){
    
   
    if(pos2==index2-1)
    {
      prev2=p;
      break;
    }
     p=p->getNext();
    pos2++;
  }
  
  if(prev2->getNext()){
    node2=prev2->getNext();
  }

  prev1->setNext(node2);
  prev2->setNext(node1);

  temp= node1->getNext();
  node1->setNext(node2->getNext());
  node2->setNext(temp);


  }
}


  /********************************************/




/* end of your implementations*/
#endif	


