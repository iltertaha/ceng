#ifndef _HW3_H
#define _HW3_H
#include <iostream>
#include <string>
#include <vector>
#include <utility>  // use just for std::pair< >
#include <fstream>

#define MAX_LOAD_FACTOR 0.65
#define EMPTY "EMPTY"
#define DELETED "DELETED"


class AccessControl
{
public:
	AccessControl(int table1Size, int table2Size);
	~AccessControl();

	int addUser(std::string username, std::string pass);
	int addUsers(std::string filePath);
	int delUser(std::string username, std::vector<std::string>& oldPasswords);
	int changePass(std::string username, std::string oldpass, std::string newpass);

	int login(std::string username, std::string pass);
	int logout(std::string username);

	float printActiveUsers();
	float printPasswords();
private:
	//<<your hash class>> activeUsers;	// hash(username) -> username
	//<<your hash class>> regUsers;  		// hash(username) -> username,password
	
		
		
	std::pair<std::string,std::string>* table1; //req users
	std::string * table2; //active users

	int n_1; // Current number of the existing entries in hash table1
	int n_2; // Current number of the existing entries in hash table2

	int current_size_table1; //to track after expanding
	int current_size_table2; //to  track after expanding

	int h1_param;
	int h2_param;


	int h1(int newK,int tableSize)const;
	int h2(int newK,int tableSize)const;
	

	void expand_table1();
	void expand_table2();

	double getLoadFactor_table1() const;
	double getLoadFactor_table2() const;
	


	int hashFunction(std::string key, int tableSize, int i);

	int is_registered(std::string username);
	int is_active(std::string username);
	std::string get_latest_password(std::string username);
	bool isPrime( int n ) const;
	int nextPrimeAfter( int n ) const;
	int add_with_order(std::pair<std::string, std::string> *& myArray,int old_table_size,std::string username, std::string pass);
	int get_pass_degree(std::string username,std::string pass);
	int get_pass_degree_old(std::pair<std::string, std::string> *& myArray,int old_table_size,std::string username,std::string pass);
	int add_with_username(std::string username);
	int add_order(std::string username, std::string pass);
};

#endif
