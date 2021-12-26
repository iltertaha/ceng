#include "the3.h"




void floydWarshall (std::vector<std::vector<int> > &graph,std::vector<std::vector<int> > &howfar) 
{ 

	int i, j, k; 

   int VV =howfar.size();
	for (i = 0; i < VV; i++) 
		for (j = 0; j < VV; j++) 
			howfar[i][j] = graph[i][j]; 


	for (k = 0; k < VV; k++) 
	{ 
	
		for (i = 0; i < VV; i++) 
		{ 

			for (j = 0; j < VV; j++) 
			{ 

				if(howfar[i][k]==-1 || howfar[k][j] == -1)
               continue;
               int total = howfar[i][k] + howfar[k][j];
            
            if (howfar[i][j] != -1) {
               if(howfar[i][j]<total)
                  howfar[i][j] ;
               else{
                  howfar[i][j] =total;
                }
            } 
            else {
                howfar[i][j] = total;
			} 
		} 
	} 

	// Print the shortest distance matrix 
	//printSolution(dist); 
}
} 





// INPUT :
//            n         : number of nodes in the graph
//            edgeList  : edges in the graph
//            scores    : importance scores
// return value : 
//                 number of disconnected components


//will be deleted
//void dfs(int n,int v,bool visited[],int**& edgeList); /*,vector<int> &vect*/
//int findConnected(int V,int**& edgeList); /*, vector<vector<int> > &general*/
//will be deleted 






void dfs(int n,int v,bool visited[],int**& edgeList){ /*,vector<int> &vect*/
    visited[v] = true; 
    

    //cout << v << " ";
    //vect.push_back(v); 
    
  
    
    for (int i = 0; i < n; i++) { 
  

        if ((!visited[i]) && edgeList[v][i]!=0) { 
            dfs(n,i,visited,edgeList);  /*,vect*/
        } 
    } 
}

int findConnected(int V,int**& edgeList ){ /*,vector<vector<int> > &general*/
   bool* visited = new bool[V];
   //vector<vector<int> > general;
  

   for (int v = 0; v < V; v++) 
      visited[v] = false; 

   int say = 0;
    for(int v=0;v <V;v++){
      visited[v] = false;
   } 
   for(int v=0;v<V;v++){
      if(visited[v] == false){
         //vector<int> temp;
         dfs(V,v,visited,edgeList); /*,temp*/
         say++;

         //cout << "\n";
         //general.push_back(temp);
      }

      
   
   }

  

   return say;
}


int Important (int n, int**& edgeList, double*& scores)
{ 
   
   std::vector<std::vector<int> > components;

   std::vector<std::vector<int> > edgeList2 (n, std::vector <int> (n, 0));


   int disconnected= findConnected(n,edgeList); /*components*/

   //int comp_num =components.size();

   std::vector<std::vector<int> > rat(n, std::vector <int> (n, 0));

   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         if(i!=j && edgeList[i][j]==0){
            edgeList2[i][j]=-1;
         }
         else{
            edgeList2[i][j] = edgeList[i][j];
         }
      }
   }

  
   

   floydWarshall(edgeList2,rat);




   // for(int g=0;g<comp_num;g++){ //for each connected group
   //    int comp_size = components[g].size();

   //    for(int i=0;i<comp_size;i++){ //between each 2 vertex
   //       for(int j=i;j<comp_size;j++){
            
   //          //calculates shortest path between 2 vertex in component.
   //          //compute shortest path between components[g][i] components[g][j]  




   //       }
   //    }//end of pair calculation

   //    //select shortest from s to v
   //    // +
   //    //select shortest from v to t
   //    // divide to
   //    // shortest from s to t



   //    //move to next component if exists
   // }   



   //component print check

    /* for(int i=0;i<rat.size();i++){
         for(int j=0;j<rat[i].size();j++){
            
            if (rat[i][j] == -1) 
				   cout<<"-1"<<"	 "; 
			   else
				cout<<rat[i][j]<<"	 "; 
         }
         
         cout<<endl;
      }

      cout<<"size  :"<<components.size()<<std::endl ; */

   //component print check


   for(int g=0; g< n;g++){ //for each vertex
      double sum=0;
      for(int i=0;i<n;i++){    //iterate over distances
         for(int j=0;j<n;j++){  //
               int pay=0;
               int payda=0;
               int val1=0;
               

               double ratio=0;

               if(rat[i][g] !=-1 && rat[g][j] != -1 ){

                  

                  if(g==i || g==j){ //if s==v or t==v path containing v is zero
                     val1=0;
                  }

                  

                  else{
                     val1= rat[i][g] +rat[g][j]; 
                  }

                  if(i==j){ //if s == t then shortest path is 1.
                     payda=1;
                  }
                  else{
                     payda = rat[i][j];
                  }
                  pay= val1;
                  //cout<<"sayi1  : "<< i <<endl;
                  //cout<<"sayi2 : "<< j <<endl;
                  //cout<<"this pay : "<< pay <<endl;
                  //cout<<"this payda : "<< payda <<endl;
                  //cout<<"--rat--"<<endl;
                  ratio = pay  / (double) payda;
                  //cout<<"this ratio "<<ratio<<endl<<endl;

                  sum+=ratio;
               }
                  //sum+=0;
            }
         }
         scores[g]=sum;
         //cout<<"--end of comp  ------------------------------------"<<g<<"--"<<endl;
      }
      //cout<<endl;
   //cout<<"scores test \n";
   //for(int i=0;i<n;i++){
      //cout<<scores[i]<<" ";
   //}


   return disconnected; 
}

 int main(){
   
   int **arr;
   arr = new int*[5];
   int **arr2;
   arr2 = new int*[6];

   int **arr3;
   arr3 = new int*[3];

 int **arr4;
   arr4 = new int*[5];

   std::vector<std::vector<int> > dene;

   int a1[5] = {0,0,2,5,3};
   int a2[5] = {0,0,0,2,3};
   int a3[5] = {2,0,0,1,0};
   int a4[5] = {5,2,1,0,4};
   int a5[5] = {3,3,0,4,0};

   int b1[6] = {0,0,0,3,0,2};
   int b2[6] = {0,0,3,0,2,0};
   int b3[6] = {0,3,0,0,0,0};
   int b4[6] = {3,0,0,0,0,4};
   int b5[6] = {0,2,0,0,0,0};
   int b6[6] = {2,0,0,4,0,0};
  



   arr[0]= a1;
   arr[1]= a2;
   arr[2]= a3;
   arr[3]= a4;
   arr[4]= a5;
   
   arr2[0]= b1;
   arr2[1]= b2;
   arr2[2]= b3;
   arr2[3]= b4;
   arr2[4]= b5;
   arr2[5]= b6;

   double *score;
   score = new double[6];
   score[0]=0;
   score[1]=0;
   score[2]=0;
   score[3]=0;
   score[4]=0;
   score[5]=0;
   score[6]=0;
   
   double result =0;

   int c1[3] = {0,0,15};
   int c2[3] = {0,0,15};
   int c3[3] = {15,15,0};
   
   arr3[0]= c1;
   arr3[1]= c2;
   arr3[2]= c3;

   int d1[5] = {0,8,0,0,10};
   int d2[5] = {8,0,0,0,0};
   int d3[5] = {0,0,0,0,0};
   int d4[5] = {0,0,0,0,16};
   int d5[5] = {10,0,0,16,0};
   
   arr4[0]= d1;
   arr4[1]= d2;
   arr4[2]= d3;
   arr4[3]= d4;
   arr4[4]= d5;
   result = Important(5,arr4,score);
   std::cout<<result<<std::endl;;

     for(int i=0;i<5;i++){
   	std::cout<< score[i]<<" ";
   }

 

} 
   /* 
   5
0 8 0 0 10
8 0 0 0 0
0 0 0 0 0
0 0 0 0 16
10 0 0 16 0
    */

   /* std::cout<<"returned value : "<<result<<std::endl;


    for(int i=0;i<dene.size();i++){
         for(int j=0;j<dene[i].size();j++){
            cout<< dene[i][j]<<" ";
         }
         
         cout<<endl;
      }

      cout<<"size  :"<<dene.size()<<std::endl ;
 */


/* 
0 0 2 5 3
0 0 0 2 3
2 0 0 1 0
5 2 1 0 4
3 3 0 4 0
 */


/* 
0 0 0 3 0 2
0 0 3 0 2 0
0 3 0 0 0 0
3 0 0 0 0 4
0 2 0 0 0 0
2 0 0 4 0 0
 */ 
