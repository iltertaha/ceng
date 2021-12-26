#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

void VideoShare::printAllVideos() {
    videos.print();
}

void VideoShare::printAllUsers() {
    users.print();
}

/* TO-DO: method implementations below */
VideoShare::VideoShare(){}

VideoShare::~VideoShare(){
    
}

void VideoShare::createUser(const string & userName, const string & name , const string & surname){
    User yeni_kullanici(userName,name,surname);
    users.insertNode(users.getHead(),yeni_kullanici);    
}

void VideoShare::loadUsers(const string & fileName){
    ifstream f(fileName.c_str());
	string line;
	while (getline(f, line))
	{
		size_t n1, n2;
		n1 = line.find(";");
		 if(line.substr(0,n1).empty()){
            continue;
        }
		n2 = line.rfind(";");
		User temp_user(line.substr(0, n1), line.substr(n1 + 1, n2 - n1 - 1), line.substr(n2 + 1, line.size() - n2 - 1));
		users.insertNode(users.getHead(), temp_user);
	}
	f.close();
}

void VideoShare::createVideo(const string & title, const string & genre){
    Video yeni_video(title,genre);
    videos.insertNode(videos.getHead(),yeni_video);
}

void VideoShare::loadVideos(const string & fileName){
    ifstream f(fileName.c_str());
	string line;
	while (getline(f, line))
	{
		size_t n1, n2;
		n1 = line.find(";");
        
		
		Video temp_video(line.substr(0, n1), line.substr(n1 + 1,line.size()-n1-1));
		videos.insertNode(videos.getHead(), temp_video);
	}
	f.close();
}

void VideoShare::addFriendship(const string & userName1, const string & userName2){
    Node<User>*p1=users.findNode(userName1);
    Node<User>*p2=users.findNode(userName2);
    if(p1 && p2){
         p1->getDataPtr()->addFriend(p2->getDataPtr());
        p2->getDataPtr()->addFriend(p1->getDataPtr());
    }

   
}

void VideoShare::removeFriendship(const string & userName1, const string & userName2){
    Node<User>*p1=users.findNode(userName1);
    Node<User>*p2=users.findNode(userName2);

    p1->getDataPtr()->removeFriend(p2->getDataPtr());
    p2->getDataPtr()->removeFriend(p1->getDataPtr());
}

void VideoShare::updateUserStatus(const string & userName, Status newStatus){
    Node<User>*userptr=users.findNode(userName);
    userptr->getDataPtr()->updateStatus(newStatus);
}

void VideoShare::subscribe(const string & userName, const string & videoTitle){
    Node<User>*userptr=users.findNode(userName);
    if(userptr->getDataPtr()->getStatus()==ACTIVE){
        Node<Video>*videoptr=videos.findNode(videoTitle);
        userptr->getDataPtr()->subscribe(videoptr->getDataPtr());
    }
    

}

void VideoShare::unSubscribe(const string & userName, const string & videoTitle){
    Node<User>*userptr=users.findNode(userName);
    if(userptr->getDataPtr()->getStatus()==ACTIVE){
    Node<Video>*videoptr=videos.findNode(videoTitle);
    userptr->getDataPtr()->unSubscribe(videoptr->getDataPtr());
    }
    
}

void VideoShare::deleteUser(const string & userName){
Node<User>* q = users.findNode(userName);
Node<User>* gez = users.first();
User* userptr=q->getDataPtr();

while(gez){
    if(gez->getDataPtr()!=userptr){
        if(gez->getDataPtr()->getFriends()->findNode(userptr)){
            Node<User*>*silinecek=gez->getDataPtr()->getFriends()->findPrev(userptr);
            gez->getDataPtr()->getFriends()->deleteNode(silinecek);
        }
    
    }
    gez=gez->getNext();
}

	if (q)
	{
		Node<User> *p = users.findPrev(userName);
		users.deleteNode(p);
	}


}


void VideoShare::sortUserSubscriptions(const string & userName){
    LinkedList<Video*>*kullanici=users.findNode(userName)->getDataPtr()->getSubscriptions();
    Node<Video*>*gez=users.findNode(userName)->getDataPtr()->getSubscriptions()->first();
	


    Node<Video*>*ptr;
    Node<Video*>*ptr2;
    Node<Video*>*cur;
    Node<Video*>*sonraki;
    
    ptr=kullanici->first();

    
    Node<Video*>*counter;
    int boyut=0;
    
    Node<Video*>* min;
	Node<Video*>* p;
	Node<Video*>* q;
    int pa=0;
    int qa=0;
    int mina=0;
    int say=0;
    
	
while(gez){
    gez=gez->getNext();
    say++;
    }
    
int n=kullanici->getLength();


        

bool sorted = false ;
for (int i = 0; (i < n-1) && !sorted; i++) {
    sorted = true;
    ptr=kullanici->first();
    for (int j=1; j <= n-i-1; j++){
        if (ptr->getData()->getTitle() > ptr->getNext()->getData()->getTitle()) { //
            
            kullanici->swap(j-1, j); // Swap these two  
            
            sorted = false ; // Mark exchange
            }
        else{
            ptr=ptr->getNext();
        }
        }

    }
}
	
   





void VideoShare::printUserSubscriptions(const string & userName){
    users.findNode(userName)->getDataPtr()->printSubscriptions();
}

void VideoShare::printFriendsOfUser(const string & userName){
    Node<User>*p=users.findNode(userName);
    if(p){
        p->getDataPtr()->printFriends();

    }
}

void VideoShare::printCommonSubscriptions(const string & userName1, const string & userName2){
    User ortak;
    //Node<User>*ortak=users.first();
    Node<User>* usr1= users.findNode(userName1);
    Node<User>* usr2= users.findNode(userName2);
    Node<Video*> *p=usr1->getDataPtr()->getSubscriptions()->first();
    Node<Video*> *q=usr2->getDataPtr()->getSubscriptions()->first();
    Node<Video*>*temp;

    Node<Video*>* sonaekle;
    sonaekle=ortak.getSubscriptions()->getHead();//ortak->getDataPtr()->getSubscriptions()->getHead();
    Node<Video*> *say1=usr1->getDataPtr()->getSubscriptions()->first();
    Node<Video*> *say2=usr2->getDataPtr()->getSubscriptions()->first();
    int count1=0;
    int count2=0;

    while(say1){say1=say1->getNext();
    count1++;}

    while(say2){say2=say2->getNext();
    count2++;}

     /*
  
    while(p){
        if(usr2->getDataPtr()->getSubscriptions()->findNode(p->getData()) && !ortak.getSubscriptions()->findNode(p->getData()))
        {   ortak.getSubscriptions()->insertNode(sonaekle,p->getData());
            sonaekle=sonaekle->getNext();}

            p=p->getNext();

        
    }

    while(q){
        if(usr1->getDataPtr()->getSubscriptions()->findNode(q->getData()) && !ortak.getSubscriptions()->findNode(q->getData()))
        {   ortak.getSubscriptions()->insertNode(sonaekle,q->getData());
            sonaekle=sonaekle->getNext();}

            q=q->getNext();
            
        
    }
ortak.printSubscriptions();
    */

    if(count2>count1){
        temp=q;
        q=p;
        p=temp;
    }
   
     while(p){
         q=usr2->getDataPtr()->getSubscriptions()->first();
        while(q){
            if(p->getData()==q->getData()){
                ortak.getSubscriptions()->insertNode(sonaekle,p->getData());
                sonaekle=sonaekle->getNext();
                }
                 q=q->getNext();
        }
        p=p->getNext();
    }

    ortak.printSubscriptions();



    /*


    while(p){
        while(q){
            if(p->getData()==q->getData()){
                ortak.getSubscriptions()->insertNode(sonaekle,p->getData());
                sonaekle=sonaekle->getNext();}
                 q=q->getNext();
        }
        p=p->getNext();
    }

    ortak.printSubscriptions();*/
    //deleteUser("mutual");
    

}


void VideoShare::printFriendSubscriptions(const string & userName){
    createUser("distink");
    Node<User>* dis=users.first();
    //Node<User>*ortak=users.first();
    Node<Video*>* sonaekle;
    sonaekle=dis->getDataPtr()->getSubscriptions()->getHead();
    Node<User>* usr1= users.findNode(userName);
    Node<Video*>* q;

    Node<User*>*friendptr=usr1->getDataPtr()->getFriends()->first();

   

   
     while(friendptr){
         q=friendptr->getData()->getSubscriptions()->first();
        while(q){
            if(!dis->getDataPtr()->getSubscriptions()->findNode(q->getData())){
            
                dis->getDataPtr()->getSubscriptions()->insertNode(sonaekle,q->getData());
                sonaekle=sonaekle->getNext();}
                
                 q=q->getNext();
        }
        friendptr=friendptr->getNext();
    }
    sortUserSubscriptions(dis->getDataPtr()->getUsername());
    dis->getDataPtr()->printSubscriptions();
    users.deleteNode(users.getHead());


}



bool VideoShare::isConnected(const string & userName1, const string & userName2){
    LinkedList<User*> todo_list;
    LinkedList<User*> done_list;
    Node<User>* usr1= users.findNode(userName1);
    Node<User>* usr2= users.findNode(userName2);

    User* usr1_ptr=usr1->getDataPtr();
    User* usr2_ptr=usr2->getDataPtr();

    todo_list.insertNode(todo_list.getHead(),usr1_ptr);

    User* start=usr1->getDataPtr();
    User* finish=usr2->getDataPtr();

    
    while(todo_list.first()){
        User* erisim=todo_list.first()->getData();
        add_end(done_list,todo_list.first());
        remove_first(todo_list);
        Node<User*>* reach=erisim->getFriends()->first();
        while(reach){
            if(reach->getData()==usr2_ptr){
                return true;
            }
            if(!done_list.findNode(reach->getData()) && !(todo_list.findNode(reach->getData()))){
                add_end(todo_list,reach);
            }

            reach=reach->getNext();
        }

    }
    return false;
}


    /*Node<User*>* gez=start->getFriends()->first();



    LinkedList<User*> visited;
    LinkedList<User*> queue;
    Node<User*>* i;
    i=queue.first();

    visited.insertNode(visited.getHead(),start);
    
    while(queue.first()!=NULL){


    }
    */
    



    void VideoShare::remove_first(LinkedList<User*> & liste){
        liste.deleteNode(liste.getHead());
    }

    void VideoShare::add_end(LinkedList<User*> & liste,Node<User*>* eklenecek_node_ptr){
        Node<User*>* traverse=liste.getHead();
        
        while(traverse->getNext()){
            traverse=traverse->getNext();
        }
        liste.insertNode(traverse,eklenecek_node_ptr->getData());

    }



