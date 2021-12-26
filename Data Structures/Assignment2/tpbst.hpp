#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.
    TwoPhaseBST();
    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);
    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);
    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");
    T *find(const std::string &primaryKey, const std::string &secondaryKey);

private: // you may add your own utility member functions here.
    void destructNode(PrimaryNode * &root);
    void destructNode(SecondaryNode * &root);
    /************************/
    struct PrimaryNode* findprimer(const std::string &primaryKey,struct PrimaryNode *t);
    struct SecondaryNode* findsecon(const std::string &secondaryKey,struct SecondaryNode *a);
    void insertprimary(const std::string &primaryKey, PrimaryNode * & t) const;
    void insertsecondary(const T& x, SecondaryNode * & t,const std::string &secondaryKey) const;
    SecondaryNode* removehelper(const std::string &secondaryKey, SecondaryNode * & t);
    SecondaryNode* findMin(SecondaryNode *t) const;
    void printprimary(PrimaryNode* t);
    void printsecondary(SecondaryNode* t);
    SecondaryNode* findPrevOfMin(SecondaryNode* t);


private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(const TwoPhaseBST &);
    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template <class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template <class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template <class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template <class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {
    /* IMPLEMENT THIS */
    if(root==NULL){

        insertprimary(primaryKey,root);
        SecondaryNode *q=new SecondaryNode(secondaryKey,data,NULL,NULL);
        root->rootSecondaryNode=q;
        
    }
     else if(root!=NULL){

        if(findprimer(primaryKey,root)){
            
            PrimaryNode* bulunan=findprimer(primaryKey,root);
            insertsecondary(data,bulunan->rootSecondaryNode,secondaryKey);
        }
        else{
            
            insertprimary(primaryKey,root);
            PrimaryNode* bulunan=findprimer(primaryKey,root);
            SecondaryNode *s=new SecondaryNode(secondaryKey,data,NULL,NULL);
            bulunan->rootSecondaryNode=s;


        } }

        return *this;
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode *pri=findprimer(primaryKey,root);
    if(pri){
        SecondaryNode *sec= findsecon(secondaryKey,pri->rootSecondaryNode);
        if(sec){
            pri->rootSecondaryNode=removehelper(secondaryKey,pri->rootSecondaryNode);
        }
    }     
    return *this;
    }

template <class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    if(primaryKey=="" && secondaryKey==""){ //FIRST CASE
        if(root==NULL){
            std::cout<<"{}";
        }
        else{
            std::cout<<"{";
           printprimary(root); //kendisi diger fonksiyonu da cagıracak elleme
            std::cout<<"}";
  
        }
        

    }

    if(primaryKey=="" && secondaryKey!=""){//dont do anything SECOND CASE
    }

    if(primaryKey!="" && secondaryKey==""){//print info about all items indexed with the given primary key THIRD CASE
        if(findprimer(primaryKey,root)){
            PrimaryNode* bulunan_pri=findprimer(primaryKey,root);
            SecondaryNode* bulunan_sec=bulunan_pri->rootSecondaryNode;
            std::cout<<"{";
            std::cout<<"\""<<primaryKey<<"\""<<" : ";
            std::cout<<"{";
            printsecondary(bulunan_sec);
            std::cout<<"}";
            std::cout<<"}";
        }

        else if(!findprimer(primaryKey,root)){
            std::cout<<"{}";
        }

    }

    if(primaryKey!="" && secondaryKey!=""){
        //print info about that specific item FOURTH CASE
        PrimaryNode* bulunan_pri=findprimer(primaryKey,root);
        if(bulunan_pri){
            SecondaryNode* bulunan_sec=findsecon(secondaryKey,bulunan_pri->rootSecondaryNode);
            if(bulunan_sec!=NULL){
            std::cout<<"{"<<"\""<<primaryKey<<"\""<<" : "<<"{"<<"\""<<secondaryKey<<"\""<<" : "<<"\""<<bulunan_sec->data<<"\""<<"}}";
            }

            else{
            std::cout<<"{}";
        }
        }

        else{
            std::cout<<"{}";
        }
        

    }

/*
    if(primaryKey!="" && secondaryKey!=""){
        if(!find(primaryKey,secondaryKey)){std::cout<< "{}";}
        if(find(primaryKey,secondaryKey)){
            PrimaryNode* pri=findprimer(primaryKey,root);
            SecondaryNode* sec=findsecon(secondaryKey,pri->rootSecondaryNode);
            std::cout<<"{" << pri->key<<" : "<<"{" << sec->key << sec->data <<"}" <<"}";
    }
    }
    else if(primaryKey!="" && secondaryKey==""){
        if(findprimer()){

        }
        else{
            
        }
    }
    else if()

    std::cout<<"{";
    printprimary(root);
    std::cout<<"}";*/
    std::cout<<std::endl;
    return *this;
}

template <class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode* a=root;
    if(findprimer(primaryKey,a)!=NULL){
        PrimaryNode* bulunan=findprimer(primaryKey,a);

        if(findsecon(secondaryKey,bulunan->rootSecondaryNode)){
            SecondaryNode* sonuc=findsecon(secondaryKey,bulunan->rootSecondaryNode);
            return &sonuc->data;
        }
        else{return NULL;}
    

    }
    else{return NULL;}

     

}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}

/*******************************************************/
template <class T>
typename TwoPhaseBST<T>::PrimaryNode* TwoPhaseBST<T>::findprimer(const std::string &primaryKey,PrimaryNode *t) {
    /* IMPLEMENT THIS */

  if(t == NULL)
      return NULL;
  else if(primaryKey < t->key)
      return findprimer(primaryKey, t->left);
  else if(t->key < primaryKey)
      return findprimer(primaryKey, t->right);
  else
      return t;    // Match*/
}
/*******************************************************************/
template <class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findsecon(const std::string &secondaryKey,SecondaryNode *a) {
    /* IMPLEMENT THIS */
SecondaryNode* t= a;
  if(t == NULL)
      return NULL;
  else if(secondaryKey < t->key)
      return findsecon(secondaryKey, t->left);
  else if(t->key < secondaryKey)
      return findsecon(secondaryKey, t->right);
  else
      return t;    // Match
}

/************************************************************************/

template <class T>
void TwoPhaseBST<T>::insertprimary(const std::string &primaryKey, PrimaryNode * & t) const {
   if(t == NULL)
      t = new PrimaryNode(primaryKey,NULL, NULL,NULL);
   else if(primaryKey < t->key)
      insertprimary(primaryKey, t->left);
   else if(t->key < primaryKey)
      insertprimary(primaryKey, t->right);
   else
      ;  // Duplicate; do nothing
}

/*************************************************************************/
template <class T>
void TwoPhaseBST<T>::insertsecondary(const T& x, SecondaryNode * & t,const std::string &secondaryKey) const {
   if(t == NULL)
      t = new SecondaryNode(secondaryKey,x, NULL, NULL);
   else if(secondaryKey < t->key)
      insertsecondary(x, t->left,secondaryKey);
   else if(t->key < secondaryKey)
      insertsecondary(x, t->right,secondaryKey);
   else
      ;  // Duplicate; do nothing
}

/***********************************************************************/
template <class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::removehelper(const std::string &secondaryKey, SecondaryNode * & t) { 
    // base case 
    if (t == NULL) return t; 
  
    // If the key to be deleted is smaller than the root's key, 
    // then it lies in left subtree 
    if (secondaryKey < t->key) 
        t->left = removehelper(secondaryKey,t->left); 
  
    // If the key to be deleted is greater than the root's key, 
    // then it lies in right subtree 
    else if (secondaryKey > t->key) 
        t->right = removehelper(secondaryKey,t->right); 
  
    // if key is same as root's key, then This is the node 
    // to be deleted 
    else
    { 
        // node with only one child or no child 
        if (t->left == NULL) 
        { 
            SecondaryNode *temp = t->right; 
            delete t; 
            return temp; 
        } 
        else if (t->right == NULL) 
        { 
            SecondaryNode *temp = t->left; 
            delete t; 
            return temp; 
        } 
  
        // node with two children
       else{
        SecondaryNode* prev=findPrevOfMin(t->right);
        

        if(prev->left){
            SecondaryNode* successor=prev->left;
            prev->left=successor->right;
            successor->left=t->left;
            successor->right=t->right;
            delete t;
            t=successor;
            }
        else{
            prev->left=t->left;
            delete t;
            t=prev;
        }
       }
        
        //SecondaryNode *newnode = new SecondaryNode(temp->key, temp->data, t->left, t->right);
        //t=newnode;
        
        // Delete the inorder successor 
        
    } 
    return t; 
} 








template <class T>
typename  TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findMin(SecondaryNode *t) const {
   if(t == NULL)
       return NULL;
   if(t->left == NULL)
       return t;
   return findMin(t->left);
}


template<class T>
void TwoPhaseBST<T>::printprimary(PrimaryNode* t){
   if (t == NULL) 
        return; 
  
    /* first recur on left child */
    
    printprimary(t->left); 
    if(t->left)std::cout<<", ";
  
    /* then print the data of node */
    std::cout <<"\""<< t->key <<"\""<<" : ";
    if(t->rootSecondaryNode){
        std::cout<<"{";
        printsecondary(t->rootSecondaryNode);
        std::cout<<"}";
    }
    else if(t->rootSecondaryNode==NULL){std::cout<<"{}";}
  
    /* now recur on right child */
    if(t->right)std::cout<<", ";
    printprimary(t->right); 
    
} 




template<class T>
void TwoPhaseBST<T>::printsecondary(SecondaryNode* t){
  if (t == NULL) 
        return; 
  
    /* first recur on left child */
    printsecondary(t->left); 
    if(t->left)std::cout<<", ";
  
    /* then print the data of node */
    std::cout <<"\""<< t->key <<"\""<< " : "<<"\""<< t->data<<"\""; 
    
  
    /* now recur on right child */
    if(t->right)std::cout<<", ";
    printsecondary(t->right); 
} 

template<class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findPrevOfMin(SecondaryNode* p)
{
	while (p && p -> left && p -> left -> left) 
    {p = p -> left;}
	return p;
}

#endif //TWOPHASEBST_H
