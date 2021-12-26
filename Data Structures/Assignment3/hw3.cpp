#include "hw3.h"


AccessControl::AccessControl(int table1Size, int table2Size)
{
    table1 = new std::pair<std::string,std::string>[table1Size];
    table2 = new std::string[table2Size];
    n_1=0;
    n_2=0;

    this->current_size_table1=table1Size;
    this->current_size_table2=table2Size;

    for(int i=0; i < table1Size; i++){
        
        table1[i].first = EMPTY;
        table1[i].second = EMPTY;
    }

    for(int i=0; i < table2Size; i++){
        
        table2[i] = EMPTY;
        
    }
    
}




AccessControl::~AccessControl()
{
    delete [] table1;
    delete [] table2;
}


int AccessControl::addUser(std::string username, std::string pass){
    int index=0;
   


    if(!is_registered(username)){

        
            
        
        for (int i = 0; i < current_size_table1; i++) {
            index = hashFunction(username,current_size_table1,i);
            
            if (table1[index].first == EMPTY) {
                n_1++;
                if (getLoadFactor_table1() > MAX_LOAD_FACTOR){
                    expand_table1();
                table1[index].first = username;
                table1[index].second = pass;
                
                
        }
                return 1;
            } 
            /*else if(table1[index].first==DELETED){
                table1[index].first=username;
                table1[index].second=pass;
                n_1++;
                return 1;
            }*/

            else if (table1[index].first == username) {//not necessary,just extra control check
                return 0;
            }
            
        }
                return 0;
    }

    else{
        return 0;
    }

}

int AccessControl::add_with_order(std::pair<std::string, std::string> *& myArray,int old_table_size,std::string username, std::string pass){
    int index=0;
    
        
        
        for (int i = 0; i < current_size_table1; i++) {
            index = hashFunction(username,current_size_table1,i);
            
            if (table1[index].first == EMPTY) {
                table1[index].first = username;
                table1[index].second = pass;
                n_1++;
                return 1;
            } 

             else if (table1[index].first == DELETED) {
                table1[index].first = username;
                table1[index].second = pass;
                n_1++;
                return 1;
            } 


             else if (table1[index].first == username ) {
                 if(get_pass_degree_old(myArray,old_table_size, table1[index].first,table1[index].second) > get_pass_degree_old(myArray,old_table_size,username,pass)){
                std::string temp = table1[index].second;
                table1[index].second = pass;
                pass = temp;
                
                //std::cout<<table1[index].second<<" daha eskiymiş "<<pass<<" den "<<std::endl;
                 }
            }

        }
                return 0;
    }


 


int AccessControl::addUsers(std::string filePath){
    std::ifstream input_file;
    std::string username,password;
    int index = 1;
    int registered=0;
    input_file.open(filePath.c_str());
    while (input_file >> username>>password) {
        
        if(addUser(username,password)){
            registered++;
            }
    
  }
    input_file.close();
    return registered;
}



int AccessControl::delUser(std::string username, std::vector<std::string>& oldPasswords){
    if(is_registered(username)){
        //delete all users with this name
        //put all passwords to oldpasswords
        int index = 0;
        for (int i = 0; i < current_size_table1 ; i++) {
            index = hashFunction(username,current_size_table1, i);

            if (table1[index].first == username)
            {
                oldPasswords.push_back(table1[index].second);
                table1[index].first=DELETED;
                table1[index].second=DELETED;
            }

            
           
    }
        return 1;
    }
    else{
        return 0;}


}
int AccessControl::changePass(std::string username, std::string oldpass, std::string newpass){



    if(is_registered(username) && (get_latest_password(username)==oldpass) ){
        if (getLoadFactor_table1() > MAX_LOAD_FACTOR){
            expand_table1();}
        
        int index=0;
        for(int i=0;i<current_size_table1;i++){
            index=hashFunction(username,current_size_table1, i);
             if (table1[index].first == EMPTY) {
                table1[index].first = username;
                table1[index].second = newpass;
                n_1++;
                return 1;
            } 

            else if(table1[index].first==DELETED){
                table1[index].first=username;
                table1[index].second=newpass;
                n_1++;
                return 1;
            }

           

        }
                return 0; //check
        
        }
    else{
        return 0;
    }

}
int AccessControl::login(std::string username, std::string pass){
    int index=0;


    
    if(is_registered(username) && (get_latest_password(username)==pass)){
        if (getLoadFactor_table2() > MAX_LOAD_FACTOR){
            expand_table2();}

        for(int i=0;i<current_size_table2;i++){
            index = hashFunction(username,current_size_table2, i);
            if (table2[index] == EMPTY){
                table2[index]=username; //logged in;
                n_2++;
                return 1;
            }
            else if(table2[index]==DELETED){
                table2[index]=username;
                n_2++;
                return 1;
            }

            else if(table2[index]==username){
                return 0;
            }

        }
        //return 0; //is it necessary or not 
    }

    else{
        return 0; //not logged in
    }
}
int AccessControl::logout(std::string username){
    int index=0;
    if(is_active(username)){
        for(int i=0;current_size_table2;i++){
            index=hashFunction(username,current_size_table2,i);
            if(table2[index]==username){
                table2[index]=DELETED;
                return 1;
            }
        }
    }
    else{
        return 0;
    }
}
float AccessControl::printActiveUsers(){

    
	for (int i=0;i<current_size_table2;i++)
	{
		std::cout<<table2[i]<<std::endl;
    }

    return getLoadFactor_table2();
}

float AccessControl::printPasswords(){
    for(int i=0;i<current_size_table1;i++){
        std::cout<<table1[i].first<<" "<<table1[i].second<<std::endl;

    }

   return getLoadFactor_table1();
}


int AccessControl::hashFunction(std::string key, int tableSize, int i){
    
	   int length = key.length();
	   int newK = 0;
	   for (int i = 0; i < length; i++)
	     newK += (int) key[i];

		 // hash function 1 -> (newK % tableSize)
		 // hash function 2 -> (newK * tableSize - 1) % tableSize)

	   //return (h1(newK,tableSize) + i * h2(newK,tableSize)) % tableSize;
        return ((newK % tableSize) + i * ((newK * tableSize - 1) % tableSize)) % tableSize;
    }



int AccessControl::h1(int newK,int tableSize)const{
    return newK % tableSize;
}

int AccessControl::h2(int newK,int tableSize)const{
    return ((newK * tableSize - 1) % tableSize);
}

/*****************************************************************/
void AccessControl::expand_table1(){

    std::pair<std::string, std::string> *old_table = table1; //to keep the link of old hashtable1
    int old_table_size = current_size_table1;
    n_1 = 0;

    current_size_table1 = nextPrimeAfter(2 * old_table_size); //expanding table
    

    table1 = new std::pair<std::string,std::string>[current_size_table1];

    for (int i = 0; i < current_size_table1; i++) {
        table1[i].first = EMPTY;
        table1[i].second = EMPTY;
  } //initialized new table to transfer
    
    for (int i = 0; i < old_table_size; i++) {
        if (old_table[i].first != EMPTY && old_table[i].first!=DELETED) {
            add_with_order(old_table,old_table_size,old_table[i].first, old_table[i].second);
            
        }

  }
  
  delete[] old_table; //free the old table
  return;
}

/*****************************************************************/

void AccessControl::expand_table2(){
    std::string * old_table=table2;
    int old_table_size= current_size_table2;
    n_2=0;

    current_size_table2= nextPrimeAfter(2*old_table_size); //exoanding table

    table2=new std::string[current_size_table2];

     for (int i = 0; i < current_size_table2; i++) {
        table2[i]= EMPTY;

  } //initialized new table to transfer

    for (int i = 0; i < old_table_size; i++) {
        if (old_table[i]!= EMPTY && old_table[i]!=DELETED) {
            add_with_username(old_table[i]);
            //login() //will be implemented
        }

  }


}

/***************************************************************/
double AccessControl::getLoadFactor_table1() const{
    return ((double) n_1+1) / ((double) current_size_table1);
}
/***************************************************************/
double AccessControl::getLoadFactor_table2() const{
    return ((double) n_2+1) / ((double) current_size_table2);
}


/*****************************************************************************************************************/
int AccessControl::is_registered(std::string username){ //get function to verify user is in the table1
    int index = 0;
    for (int i = 0; i < current_size_table1 ; i++) {
        index = hashFunction(username,current_size_table1, i);

        if (table1[index].first == EMPTY && table1[index].second == EMPTY){
            return 0;} //not necessary to look after because if there is at least one.

        else if (table1[index].first == username){
            //std::cout<<"is registered :"<<username<<std::endl;
            return 1;} //found
    }
    return 0; //not found

}
/****************************************************************************************************************/
int AccessControl::is_active(std::string username){ //get function to verify user is in the table2

int index=0;

for(int i=0; i<current_size_table2; i++){
    index=hashFunction(username,current_size_table2,i);

    if (table2[index]==username){
        return 1;} //found
}

    return 0; //not found

   
}
/******************************************************************************************************************/
std::string AccessControl::get_latest_password(std::string username){
    int index = 0;
    std::string lastpass;

    for (int i = 0; i < current_size_table1 ; i++) {
        index = hashFunction(username,current_size_table1, i);

        if (table1[index].first == username){
            lastpass=table1[index].second;} //if there is a user take this into 
            
    }
    //std::cout<<"lastpass : "<<lastpass<<std::endl;
    return lastpass; //we return the latest one we found


}

/******************************************************************************************************************/
bool AccessControl::isPrime( int n ) const
{
    if( n%2 == 0 ) return false;
    for( int i=3; i*i<=n; i+=2 )
        if( n%i == 0 )
            return false;
    return true;
}

/*********************************************************************************************************************/
int AccessControl::nextPrimeAfter( int n ) const
{
    for( int i=n+1; ; i++ )
        if( isPrime(i) )
            return i;
}





 int AccessControl::get_pass_degree(std::string username,std::string pass){

     int index = 0;
     int degree_counter=0;
    std::string lastpass;

    for (int i = 0; i < current_size_table1 ; i++) {
        index = hashFunction(username,current_size_table1, i);

        if (table1[index].first == username && table1[index].second!=pass){
            degree_counter++;
            }

        else if(table1[index].first== username && table1[index].second==pass){
            
            return degree_counter;
        }
            
    }
 
    return degree_counter; //we return degree


 }


int AccessControl::add_with_username(std::string username){
    int index=0;

    for(int i=0;i<current_size_table2;i++){
            index = hashFunction(username,current_size_table2, i);
            if (table2[index] == EMPTY){
                table2[index]=username; //logged in;
                n_2++;
                return 1;
            }
            

        }
        return 0;
}


/*************************************/
int AccessControl::get_pass_degree_old(std::pair<std::string, std::string> *& myArray,int old_table_size,std::string username,std::string pass){

     int index = 0;
     int degree_counter=0;
    std::string lastpass;

    for (int i = 0; i < current_size_table1 ; i++) {
        index = hashFunction(username,old_table_size, i);

        if (myArray[index].first == username && myArray[index].second!=pass){
            degree_counter+=1;
            }

        else if(myArray[index].first== username && myArray[index].second==pass){
            
            return degree_counter;
        }
            
    }
 
    return degree_counter; //we return degree


 }

/***************************************/




/*****************************/
int main(){
         AccessControl ac1(1, 1);
    std::cout << "User Add: Sarah " << ac1.addUser("Sarah", "pass123") << std::endl;
    std::cout << "User Add: Sarah " << ac1.addUser("Sarah", "pass123") << std::endl;
    std::cout << "User Add: Sarah " << ac1.addUser("Sarah", "pass1234") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac1.printPasswords();

    std::cout << "User Add: Morgan " << ac1.addUser("Morgan", "morP4ssw0rd") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac1.printPasswords();

    std::cout << "User Add: BigMike " << ac1.addUser("BigMike", "mikesPassword") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac1.printPasswords();

    std::cout << "User Add: Jeff " << ac1.addUser("Jeff", "jeffsspassword") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac1.printPasswords();

    std::cout << "User Add: Lester " << ac1.addUser("Lester", "Lestersspassword") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac1.printPasswords();


    AccessControl ac2(4, 4);
    std::cout << "User Add: Sarah " << ac2.addUser("Sarah", "Sarahspass") << std::endl;
    std::cout << "User Add: Sarah " << ac2.addUser("Sarah", "pass1234") << std::endl;
    std::cout << "User Add: Morgan " << ac2.addUser("Morgan", "morganspass") << std::endl;
    std::cout << "User Add: Ellie " << ac2.addUser("Ellie", "elspassword") << std::endl;
    std::cout << "User Add: Vicky " << ac2.addUser("Vicky", "vickypass") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac2.printPasswords();

    std::cout << "User Add: Casey " << ac2.addUser("Casey", "caseysspassword") << std::endl;
    std::cout << "User Add: Casey " << ac2.addUser("Casey", "caseyssp4ssw0rd") << std::endl;
    std::cout << "User Add: Vale " << ac2.addUser("Vale", "valespass") << std::endl;
    std::cout << "User Add: Devon " << ac2.addUser("Devon", "devonspass") << std::endl;
    std::cout << "User Add: Chuck " << ac2.addUser("Chuck", "chuckspass") << std::endl;
    std::cout << "User Add: Orion " << ac2.addUser("Orion", "orionspass") << std::endl;
    std::cout << "User Add: Chuck " << ac2.addUser("Chuck", "chuckspass") << std::endl;
    std::cout << "User Add: Chuck " << ac2.addUser("Chuck", "chuckspass") << std::endl;
    std::cout << "User Add: Devon " << ac2.addUser("Devon", "devonspass") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac2.printPasswords();

    AccessControl ac3(3, 1);
    std::cout << "User Add: Sarah " << ac3.addUser("Sarah", "Sarahspass") << std::endl;
    std::cout << "User Add: Sarah " << ac3.addUser("Sarah", "sarspass") << std::endl;
    std::cout << "---------" << std::endl << "Passwords" << std::endl;
    ac3.printPasswords();
}
