#include <stdio.h>

#define INT_MIN -1

#define ROW 100
#define COLUMN 100
int sol(int field[100][100],int x,int y,int lvl,int forbidden){
    if(field[x-1][y]==lvl )
      return 1;
      else {
      return 0;}
}

int sag(int field[100][100],int x,int y,int lvl,int forbidden){
  if(field[x+1][y]==lvl )
    return 1;
  else {
    return 0;}
}

int ust(int field[100][100],int x,int y,int lvl,int forbidden){
    if(field[x][y-1]==lvl )
      return 1;
      else {
      return 0;}
}

int alt(int field[100][100],int x,int y,int lvl,int forbidden){
  if(field[x][y+1]==lvl )
    return 1;
  else {
  return 0;}
}

int komsu(int field[100][100],int i,int j,int n,int lvl,int forbidden){
  int modified[ROW][COLUMN]={0};
  int toplam=1;
if(i<0 || j<0 || j>n-1 || i>n-1 || field[i][j]!=lvl )
  toplam=0;


if (ust(field,i,j,lvl,0) && forbidden!=0 ){
  if (j!=0 && field[i][j]!=0){
  /*printf("ust: (%d %d)",i,j-1);*/
  toplam+=komsu(field,i,j-1,n,lvl,1);/*+komsu(field,i,j+1,n,lvl,0)+komsu(field,i-1,j,n,lvl,3)+komsu(field,i+1,j,n,lvl,2);}*/
}}
if (alt(field,i,j,lvl,0) && forbidden!=1){
  if (j!=n-1 && field[i][j]!=0){
    /*printf("alt: (%d %d)\n",i,j+1);*/
  toplam+=komsu(field,i,j+1,n,lvl,0);/*+komsu(field,i,j-1,n,lvl,1)+komsu(field,i-1,j,n,lvl,3)+komsu(field,i+1,j,n,lvl,2);*/
}
}
if (sol(field,i,j,lvl,0) && forbidden!=2){
  if (i!=0 && field[i][j]!=0){
    /*printf("sol: (%d %d)\n",i-1,j);*/
  toplam+=komsu(field,i-1,j,n,lvl,3);/*+komsu(field,i,j-1,n,lvl,1)+komsu(field,i,j+1,n,lvl,0)+komsu(field,i+1,j,n,lvl,2);*/
}
}
if (sag(field,i,j,lvl,0) && forbidden!=3){
  if (i!=n-1 && field[i][j]!=0){
    /*printf("sag: (%d %d)\n",i+1,j);*/
  toplam+=komsu(field,i+1,j,n,lvl,2);/*+komsu(field,i,j-1,n,lvl,1)+komsu(field,i,j+1,n,lvl,0)+komsu(field,i-1,j,n,lvl,3);*/
}
}


return toplam;
}




int neigh(int field[100][100],int neighbors[100][100],int i,int j,int n,int lvl,int forbidden){

  int toplam=1;
if(i<0 || j<0 || j>n-1 || i>n-1 || field[i][j]!=lvl )
  toplam=0;


if (ust(field,i,j,lvl,0) && forbidden!=0 ){
  if (j!=0 && field[i][j]!=0){
  /*printf("ust: (%d %d)\n",i,j-1);*/
  neighbors[i][j-1]=-1;
  toplam+=neigh(field,neighbors,i,j-1,n,lvl,1);/*+komsu(field,i,j+1,n,lvl,0)+komsu(field,i-1,j,n,lvl,3)+komsu(field,i+1,j,n,lvl,2);}*/
}}
if (alt(field,i,j,lvl,0) && forbidden!=1){
  if (j!=n-1 && field[i][j]!=0){
    /*printf("alt: (%d %d)\n",i,j+1);*/
    neighbors[i][j+1]=-1;
  toplam+=neigh(field,neighbors,i,j+1,n,lvl,0);/*+komsu(field,i,j-1,n,lvl,1)+komsu(field,i-1,j,n,lvl,3)+komsu(field,i+1,j,n,lvl,2);*/
}
}
if (sol(field,i,j,lvl,0) && forbidden!=2){
  if (i!=0 && field[i][j]!=0){
    /*printf("sol: (%d %d)\n",i-1,j);*/
    neighbors[i-1][j]=-1;
  toplam+=neigh(field,neighbors,i-1,j,n,lvl,3);/*+komsu(field,i,j-1,n,lvl,1)+komsu(field,i,j+1,n,lvl,0)+komsu(field,i+1,j,n,lvl,2);*/
}
}
if (sag(field,i,j,lvl,0) && forbidden!=3){
  if (i!=n-1 && field[i][j]!=0){
    /*printf("sag: (%d %d)\n",i+1,j);*/
    neighbors[i+1][j]=-1;
  toplam+=neigh(field,neighbors,i+1,j,n,lvl,2);/*+komsu(field,i,j-1,n,lvl,1)+komsu(field,i,j+1,n,lvl,0)+komsu(field,i-1,j,n,lvl,3);*/
}
}


return toplam;
}

int komsuhelper(int field[100][100],int neighbors[100][100],int x,int y,int n,int level,int forbidden){
  int sayac=komsu(field,x,y,n,level,forbidden);

  /*printf("komsusay: %d\n",sayac);*/
  if(sayac>2){
    int i1;
    int j1;
      neigh(field,neighbors,x,y,n,level,forbidden);
      field[x][y]=level+1;


      for(j1=0;j1<n;j1++){
          for(i1=0;i1<n;i1++){
            if (neighbors[i1][j1]==-1){
              field[i1][j1]=0;
              neighbors[i1][j1]=0;
            }}}

  return komsuhelper(field,neighbors,x,y,n,level+1,forbidden);

  }
}


int max(int x, int y,int nx1,int ny1,int nx2,int ny2);


int yol(int cost[100][100], int m, int n,int step)

{
	/*printf("call %d ",step);*/
if (n < 0 || m < 0)
	return INT_MIN;
else if (m == 0 && n == 0){

	return cost[m][n];
}
else{
	/*printf("row:%d col:%d\n",m,n);*/
		return cost[m][n] + max(yol(cost, m-1, n,step+1),yol(cost, m, n-1,step+1),m-1,n,m,n-1);
}}

int max(int x, int y,int nx1,int ny1,int nx2,int ny2)
{
if (x >= y){
	/*printf("burdan devam et %d %d\n",nx1,ny1 );*/
	/*printf("E\n");*/
	return x;}
else{

		/*printf("burdan devam et %d %d\n",nx2,ny2);*/
	/*printf("S\n");*/
	return y;
}}

int path(int x,int y,int array[100][100],int n,int yonsay,char direct[198])

{

  if(x==0 && y!=0 ){
  /*printf(" asagi  ");*/
  direct[yonsay]='S';
  return path(x,y-1,array,n,yonsay-1,direct);}

  else if(y==0 && x!=0 ){
  /*printf(" sag ");*/
  direct[yonsay]='E';
  return path(x-1,y,array,n,yonsay-1,direct);}

  else if(x!=0 && y!=0){
	if(array[x][y-1]>=array[x-1][y] ){

		/*printf(" asagi ");*/
    direct[yonsay]='S';
		return path(x,y-1,array,n,yonsay-1,direct);}
	else if(array[x][y-1]<array[x-1][y] ){
		/*printf(" sag ");*/
    direct[yonsay]='E';
		/*printf("%d %d",x,y);*/
		return path(x-1,y,array,n,yonsay-1,direct);
}
}}


int main(){
  int n,level,y,x;
  int field [ROW][COLUMN] = {0};

  int neighbors [ROW][COLUMN] = {0};
  int arr[ROW][COLUMN]={0};
  int total;
  int i2;
  int j2;

  char direct[198] = {0};

  scanf("%d",&n);

  while(scanf("%d %d %d",&level,&x,&y) != EOF){
  field[x][y]=level;
  komsuhelper(field,neighbors,x,y,n,level,-1);


  }
/*  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      neighbors*/


  for(j2=0;j2<n;j2++){
    for(i2=0;i2<n;i2++){
        printf("%d ",field[i2][j2]);}
  printf("\n");}

/*printf("----------------\n");*/
/*
for(int j=0;j<n;j++){
    for(int i=0;i<n;i++){

      printf("%d ",neighbors[i][j]);
    }
    printf("\n");
}
}
*/
/*
for (int i=0;i<n;i++){
	for (int j=0;j<n;j++){
		arr[i][j]=yol(field,i,j,0);

	//printf("%d ", yol(cost, i, j,0));}
	//printf("\n");
}
}*/

/*for (int i=0;i<5;i++){
	for (int j=0;j<5;j++){
		printf("%d ",arr[i][j]);}
	printf("\n");}
*/
/*for (int j=0;j<n;j++){
	for (int i=0;i<n;i++){
		printf("%d ",arr[i][j]);}
	printf("\n");}
	printf("max sum:%d\n",yol(field,n-1,n-1,0)*40);
printf("path: ");path(n-1,n-1,arr,n,197,direct);
for (int i=0;i<198;i++){
  if (direct[i]=='E' ||direct[i]=='S' )
  printf("%c ",direct[i]);

}*/

/*
printf("array[0][1]=%d\n",arr[0][1]);
printf("array[0][1]=%d\n",arr[1][0]);
*/




}
