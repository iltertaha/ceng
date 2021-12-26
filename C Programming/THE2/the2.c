#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*****************************************************/
struct node {
	int data;
	struct node *next;
};

/********************************************************/
struct message_node{
	int sender_id;
	int topic_id;
	int message_id;
	struct message_node *next;
};
/************************************************************/
struct topic_node{
	char *topic_name;
	int topic_id;
	struct node *follower_list; /*hold user ID's in data field*/
	struct topic_node *next;

	/* Add additional fields as needed*/
};
/**************************************************************/
struct user_node{
	char *user_name;
	int user_id;
	struct node *followed_topics; /*hold topic ID's in data field*/
	struct message_node *inbox;
  struct user_node *next;

	/* Add additional fields as needed*/
};

/********************************************************************/

struct node* createfollowedtopicnode(int topicid){
    struct node *newNode = malloc(sizeof(struct node));
    if(newNode)
    {
      newNode->data=topicid;
      newNode->next = NULL;

    }
    return newNode;
}

/***********************************************************************/
/***********************************************************************/
void addfollowedtopicnode(struct node **head, int topicid){

    struct node *newNode = createfollowedtopicnode(topicid);
    struct node *curr;

    if((*head)==NULL)
    {
        *head=newNode;
        (*head)->next=NULL;
    }

    else
    {
 curr = *head;
 while((curr->next)!=NULL){
     curr=curr->next;}
curr->next=newNode;
    }
}
/******************************************************************/




/********************************************************************/
/*
struct node* createinboxnode(int userid){
    struct message_node *newNode = malloc(sizeof(struct message_node));
    if(newNode)
    {
      newNode->data=userid;
      newNode->next = NULL;

    }
    return newNode;
}
*/
/***********************************************************************/
/***********************************************************************/

/*
void addtoinbox(struct message_node **head, int userid){

    struct node *newNode = createfollowernode(userid);
    struct node *curr;

    if((*head)==NULL)
    {
        *head=newNode;
        (*head)->next=NULL;
    }

    else
    {
 curr = *head;
 while((curr->next)!=NULL)
     curr=curr->next;
     curr->next=newNode;
    }
}*/
/******************************************************************/




/********************************************************************/

struct message_node* createmessagenode(int sender,int topic,int counter){
    struct message_node *newNode = malloc(sizeof(struct message_node));
    if(newNode)
    {

newNode->sender_id=sender;
newNode->topic_id=topic;
newNode->message_id=counter;


 newNode->next = NULL;
    }
    return newNode;
}

/***********************************************************************/
void addmessagenode(struct message_node **head,int sender,int topicid,int counter){

    struct message_node *newNode = createmessagenode(sender,topicid,counter);
    struct message_node *curr;

    if((*head)==NULL)
    {
        *head=newNode;
        (*head)->next=NULL;
    }

    else
    {
 curr = *head;
 while((curr->next)!=NULL){
     curr=curr->next;}
     curr->next=newNode;
    }
}
/******************************************************************/




/********************************************************************/

struct topic_node* createtopicnode( char* name,int counter){
    struct topic_node *newNode = malloc(sizeof(struct topic_node));
    if(newNode)
    {
newNode->topic_name=malloc(sizeof(char)*21);
strcpy(newNode->topic_name,name);
newNode->topic_id=counter;
newNode->follower_list=NULL;


 newNode->next = NULL;
    }
    return newNode;
}

/***********************************************************************/
void addtopicnode(struct topic_node **head, char* val,int counter){

    struct topic_node *newNode = createtopicnode(val,counter);
    struct topic_node *curr;

    if((*head)==NULL)
    {
        *head=newNode;
        (*head)->next=NULL;
    }

    else
    {
 curr = *head;
 while((curr->next)!=NULL){
     curr=curr->next;}
     curr->next=newNode;
    }
}
/******************************************************************/




/********************************************************************/

struct node* createfollowernode(int userid){
    struct node *newNode = malloc(sizeof(struct node));
    if(newNode)
    {
      newNode->data=userid;
      newNode->next = NULL;

    }
    return newNode;
}

/***********************************************************************/
/***********************************************************************/
void addfollowernode(struct node **head, int userid){

    struct node *newNode = createfollowernode(userid);
    struct node *curr;

    if((*head)==NULL)
    {
        *head=newNode;
        (*head)->next=NULL;
    }

    else
    {
 curr = *head;
 while((curr->next)!=NULL){
     curr=curr->next;}
     curr->next=newNode;
    }
}
/******************************************************************/



/********************************************************************/

struct user_node* createusernode( char* name,int num){
    struct user_node *newNode = malloc(sizeof(struct user_node));
    if(newNode)
    {
newNode->user_name=malloc(sizeof(char)*21);
strcpy(newNode->user_name,name);
newNode->user_id=num;
newNode->inbox=NULL;
newNode->followed_topics=NULL;

 newNode->next = NULL;
    }
    return newNode;
}

/***********************************************************************/
void addusernode(struct user_node **head, char* val,int num){

    struct user_node *newNode = createusernode(val,num);
    struct user_node *curr;

    if((*head)==NULL)
    {
        *head=newNode;
        (*head)->next=NULL;
    }

    else
    {
 curr = *head;
 while((curr->next)!=NULL){
     curr=curr->next;}
     curr->next=newNode;
    }
}
/******************************************************************/


 int sizeofList(struct user_node *head)
 {
     int count;
     struct user_node *curr;
     if(head==NULL)
         return 0;
     if(head->next==NULL)
         return 1;

     count=1;
     curr=head;
     while ( (curr = curr->next) != NULL )
         count++;

     return count;
 }
/************************************************************************/

 int printList(struct user_node *head)
 {
     int listSize=sizeofList(head);

     int i;
     struct user_node *curr;
     if(listSize==0)
     {

 	return 0;
     }

     curr=head;
     for(i=1;i<=listSize;i++)
     {
         printf("Node %d: %s\n",i, curr->user_name);
 	curr=curr->next;
     }
		 return 0;
 }
/*************************************************************************/

/******************************************************************/


 int sizeofList2(struct topic_node *head)
 {
     int count;
     struct topic_node *curr;
     if(head==NULL)
         return 0;
     if(head->next==NULL)
         return 1;

     count=1;
     curr=head;
     while ( (curr = curr->next) != NULL )
         count++;

     return count;
 }
/************************************************************************/

 void printList2(struct topic_node *head)
 {
     int listSize=sizeofList2(head);

     int i;
     struct topic_node *curr;
     /*if(listSize==0)
     {
 	printf("Nothing to print!\n");
 	return;
}*/

     curr=head;
     for(i=1;i<=listSize;i++)
     {
         printf("Node %d: %s\n",i, curr->topic_name);

 	curr=curr->next;

     }
 }
/*************************************************************************/




int useridbulucu(struct user_node *head,char* username)
{
struct user_node* tmp=head;
while(tmp!=NULL){
  if(strcmp(tmp->user_name,username)==0){
    return tmp->user_id;}

  tmp=tmp->next;
}
return 0;
}
/****************************************************************************/


/*************************************************************************/




int topicidbulucu(struct topic_node *head,char* topicname)
{
struct topic_node* tmp=head;
while(tmp!=NULL){
  if(strcmp(tmp->topic_name,topicname)==0){
    return tmp->topic_id;}

  tmp=tmp->next;
}
return 0;
}
/****************************************************************************/
/****************************************************************************/

void deleteusernode(struct user_node **head_ref, int key)
{

    struct user_node* temp = *head_ref, *prev;


    if (temp != NULL && temp->user_id == key)
    {
        *head_ref = temp->next;
        free(temp);
        return;
    }


    while (temp != NULL && temp->user_id != key)
    {
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL) return;


    prev->next = temp->next;

    free(temp);
}
/*********************************************************/




void deletefollowernode(struct node** head_ref, int key)
{

    struct node* temp = *head_ref, *prev;


    if (temp != NULL && temp->data == key)
    {
        *head_ref = temp->next;
        free(temp);
        return;
    }


    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL) return;


    prev->next = temp->next;

    free(temp);
}
/*********************************************************/

/*********************************************************/

/****************************************************************************/

void deletemessagenode(struct message_node** head_ref, int key)
{

    struct message_node* temp = *head_ref, *prev;


    if (temp != NULL && temp->topic_id == key)
    {
        *head_ref = temp->next;
        free(temp);
        return;
    }


    while (temp != NULL && temp->topic_id != key)
    {
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL) return;


    prev->next = temp->next;

    free(temp);
}
/*********************************************************/


/****************************************************************************/

void delete_message_from_inbox_of_others(struct message_node** head_ref, int key)
{

    struct message_node* temp = *head_ref, *prev;


    if (temp != NULL && temp->sender_id == key)
    {
        *head_ref = temp->next;
        free(temp);
        return;
    }


    while (temp != NULL && temp->sender_id != key)
    {
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL) return;


    prev->next = temp->next;

    free(temp);
}
/*********************************************************/

/*****************************************************************************/

int main() {
  struct user_node* userhead;
  struct topic_node* topichead;
  struct message_node* messagehead;
  int user_id_counter=1;
  int message_id_counter=1;
  int topic_id_counter=1;


  /*struct topic_node* gez=topichead;*/

  /*struct user_node* temp;*/

  /*struct user_node* final;*/
  /*struct node* inboxptr;*/

	struct user_node* userbas;
	struct message_node* inboxbas;


  char input[100];
	userhead=NULL;topichead=NULL;messagehead=NULL;

    while(  fgets( input , sizeof(input) , stdin )) {
      char *pch;
    /*printf("%s",input);*/
    pch = strtok (input," \n");

    if(strcmp(pch,"ADD")==0){
      char *username;
      username=strtok (NULL, " \n"); /*username oku*/
      addusernode(&userhead,username,user_id_counter);

      user_id_counter++;
  }

    if(strcmp(pch,"SUBSCRIBE")==0){
      int foundtopic=0;
      char *username;
      char *topicname;
      struct topic_node* topicptr=topichead;
			struct user_node* userptr=userhead;

      username=strtok (NULL, " \n");
      topicname=strtok(NULL, " \n");

      while(topicptr!=NULL){
        if(strcmp(topicptr->topic_name,topicname)==0){
          foundtopic=1;break;}
          topicptr=topicptr->next;
        }

      if(foundtopic==1){
        topicptr=topichead;
        while(topicptr!=NULL){
          if(strcmp(topicptr->topic_name,topicname)==0){
            addfollowernode(&(topicptr->follower_list),useridbulucu(userhead,username));}
          topicptr=topicptr->next;
        }
      }

      else if(foundtopic==0){
        addtopicnode(&topichead,topicname,topic_id_counter);
        topicptr=topichead;
        while(topicptr!=NULL){
          if(strcmp(topicptr->topic_name,topicname)==0){
            addfollowernode(&(topicptr->follower_list),useridbulucu(userhead,username));
            topic_id_counter++;break;}
          topicptr=topicptr->next;
        }
      }

			while(userptr!=NULL){
				if(strcmp(userptr->user_name,username)==0){
					addfollowedtopicnode(&(userptr->followed_topics),topicidbulucu(topichead,topicname));
				}
				userptr=userptr->next;
			}



      }

      if(strcmp(pch,"SEND")==0){
        char *username;
        char *topicname;
				struct node *followed;

        struct user_node* userptr=userhead;
				addmessagenode(&messagehead,useridbulucu(userhead,username),topicidbulucu(topichead,topicname),message_id_counter);

				username=strtok (NULL, " \n");
				topicname=strtok(NULL, " \n");

        while(userptr!=NULL){
					/*printf("check1\n");*/
          if((strcmp(userptr->user_name,username)!=0)){ /*to avoid double add the message to senders inbox*/
						/*printf(" check2\n");*/
						followed=userptr->followed_topics;

            while(followed!=NULL){
							/*printf("check3");*/
              if(followed->data==topicidbulucu(topichead,topicname)){
                addmessagenode(&(userptr->inbox),useridbulucu(userhead,username),topicidbulucu(topichead,topicname),message_id_counter);}

                followed=followed->next;
            }

          }
        userptr=userptr->next;}

        userptr=userhead;
        while(userptr!=NULL){
          if(strcmp(userptr->user_name,username)==0){
            addmessagenode(&(userptr->inbox),useridbulucu(userhead,username),topicidbulucu(topichead,topicname),message_id_counter);}
          userptr=userptr->next;
          }


					message_id_counter++;
        }

        if(strcmp(pch,"UNSUBSCRIBE")==0){
          char *username;
          char *topicname;
          struct topic_node * tptr;
          struct message_node *usermestemp;
          struct user_node *usertemp;
          struct node *cikar;

					struct node *user_takip_gez;

          username=strtok (NULL, " \n");
          topicname=strtok(NULL, " \n");

          tptr=topichead;

          while(tptr!=NULL){
            if(strcmp(tptr->topic_name,topicname)==0){
              break;}
            tptr=tptr->next;
          }

          cikar=tptr->follower_list;

					while(cikar!=NULL){
						if(cikar->data==useridbulucu(userhead,username)){
							deletefollowernode(&(tptr->follower_list),useridbulucu(userhead,username));
						}
						cikar=cikar->next;
					}

          usertemp=userhead;

          while(usertemp!=NULL){
            if(strcmp(usertemp->user_name,username)==0){
              break;
            }
            usertemp=usertemp->next;
          }

					user_takip_gez=usertemp->followed_topics;
					while(user_takip_gez!=NULL){
						if(user_takip_gez->data==topicidbulucu(topichead,topicname)){
							deletefollowernode(&(usertemp->followed_topics),topicidbulucu(topichead,topicname));
						}

						user_takip_gez=user_takip_gez->next;
					}

          usermestemp=usertemp->inbox;
          while(usermestemp!=NULL){
            if(usermestemp->topic_id==topicidbulucu(topichead,topicname)){

              deletemessagenode(&(usertemp->inbox),usermestemp->topic_id);
            }
            usermestemp=usermestemp->next;
          }



        }

        if(strcmp(pch,"DELETEUSER")==0){

          char *username;
          struct user_node* userptr;
          struct message_node *msg;
					int usertut=useridbulucu(userhead,username);
          username=strtok (NULL, " \n");


          deleteusernode(&userhead,usertut);
          userptr=userhead;

          while(userptr!=NULL){
            msg=userptr->inbox;
            while(msg!=NULL){
              if(msg->sender_id==usertut){
                delete_message_from_inbox_of_others(&(userptr->inbox),usertut);
              }
              msg=msg->next;
            }
            userptr=userptr->next;
          }
        }







/*
    printList(userhead);*/

    /*printf("%s",userhead->user_name);
    printf("%d",useridbulucu(userhead,"ilter"));
    printf("%d",topichead->follower_list->next->data);
*/

}


/*printf("%d ",userhead->inbox->sender_id);*/

/*printf("%d\n",userhead->inbox->message_id);*/




userbas=userhead;
while(userbas!=NULL){
	printf("%d:",userbas->user_id);

	inboxbas=userbas->inbox;
	if(inboxbas!=NULL){printf(" ");}
	while(inboxbas!=NULL){

		printf("%d",inboxbas->message_id);
		if(inboxbas->next!=NULL){printf(" ");}
		inboxbas=inboxbas->next;
	}
	printf("\n");
	userbas=userbas->next;

	}



    return 0;

}
