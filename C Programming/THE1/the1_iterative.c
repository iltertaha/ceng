#include <stdio.h>

#include<limits.h>

#define ROW 100
#define COLUMN 100

int komsu(int field[100][100],int valx,int valy,int level,int n){
  int x1=valx;
  int y1=valy;
  int x2=valx;
  int y2=valy;
  int x3=valx;
  int y3=valy;
  int x4=valx;
  int y4=valy;

  int count=1;/*
  if(x1>0 && y1>0 && field[valx-1][valy-1]==level){
  printf("test1");
  count++;}
  if(x1>0 && y1<=n-1 && field[valx-1][valy+1]==level){
  printf("test2");
  count++;}
  if(x1<n-1 && y1<n-1 && field[valx+1][valy+1]==level){
  printf("test3");
  count++;}
  if(x1<n-1 && y1>0 && field[valx+1][valy-1]==level){
  printf("test4");
  count++;}*/

  for(x1;x1<n;x1++){
    if(x1+1<=n-1 && field[x1+1][y1]==level){
      /*printf("testloop1\n");*/
      count++;
      if(field[x1+1][y1+1]==level){
      count++;break;}
      if(field[x1+1][y1-1]==level){
      count++;break;}
    }
      else
      break;
  }

  for(x2;x2>0;x2--){
    if(x2-1>=0 && field[x2-1][y2]==level){
      /*printf("testloop2\n");*/
      count++;
      if(field[x2-1][y2+1]==level){
      count++;break;}
      if(field[x2-1][y2-1]==level){
      count++;break;}
      }

      else
      break;}

  for(y3;y3<n;y3++){
    if(y3+1<=n-1 && field[x3][y3+1]==level){
      /*printf("testloop3\n");*/
    count++;
    if(field[x3+1][y3+1]==level){
    count++;break;}
    if(field[x3-1][y3+1]==level){
    count++;break;}
  }
    else
    break;}

  for(y4;y4>=0;y4--){
    if(y4-1>=0 && field[x4][y4-1]==level){
      /*printf("testloop4\n");*/
    count++;
    if(field[x4+1][y4-1]==level){
    count++;break;}
    if(field[x4-1][y4-1]==level){
    count++;break;}
  }
    else
    break;
  }

  return count;
}


int zeroassigner(int field[100][100],int valx,int valy,int level,int n){
  int x1=valx;
  int y1=valy;
  int x2=valx;
  int y2=valy;
  int x3=valx;
  int y3=valy;
  int x4=valx;
  int y4=valy;

  int count=1;
  field[valx][valy]=field[valx][valy]+1;
/*
  if(x1>0 && y1>0 && field[valx-1][valy-1]==level){
  field[valx-1][valy-1]=0;
  count++;}
  if(x1>0 && y1<n-1 && field[valx-1][valy+1]==level){
  field[valx-1][valy+1]=0;
  count++;}
  if(x1<n-1 && y1<n-1 && field[valx+1][valy+1]==level){
  field[valx+1][valy+1]=0;
  count++;}
  if(x1<n-1 && y1>0 && field[valx+1][valy-1]==level){
  field[valx+1][valy-1]=0;
  count++;}*/

  for(x1;x1<n;x1++){
    if(x1+1<=n-1 && field[x1+1][y1]==level){
      field[x1+1][y1]=0;
      count++;
      if(field[x1+1][y1+1]==level){
      field[x1+1][y1+1]=0;break;}
      if(field[x1+1][y1-1]==level){
      field[x1+1][y1-1]=0;break;}
    }
      else
      break;
  }

  for(x2;x2>0;x2--){
    if(x2-1>=0 && field[x2-1][y2]==level){
      field[x2-1][y2]=0;
      count++;
      if(field[x2-1][y2+1]==level){
      field[x2-1][y2+1]=0;break;}
      if(field[x2-1][y2-1]==level){
      field[x2-1][y2-1]=0;break;}
    }
      else
      break;}

  for(y3;y3<n;y3++){
    if(y3+1<=n-1 && field[x3][y3+1]==level){
      field[x3][y3+1]=0;
    count++;
    if(field[x3+1][y3+1]==level){
    field[x3+1][y3+1]=0;break;}
    if(field[x3-1][y3+1]==level){
    field[x3-1][y3+1]=0;break;}}
    else
    break;}

  for(y4;y4>=0;y4--){
    if(y4-1>=0 && field[x4][y4-1]==level){
        field[x4][y4-1]=0;
    count++;
    if(field[x4+1][y4-1]==level){
    field[x4+1][y4-1]=0;break;}
    if(field[x4-1][y4-1]==level){
    field[x4-1][y4-1]=0;break;}}
    else
    break;
  }

  return count;
}






int main(){
  int n,level,y,x;
  int field [ROW][COLUMN] = {0};
  int neighbors [ROW][COLUMN] = {0};
  int total;
  int komsusay;
  int j3;
  int i3;
  scanf("%d",&n);
while(scanf("%d %d %d",&level,&x,&y) != EOF){
  field[x][y]=level;
  komsusay=komsu(field,x,y,level,n);
  /*printf("%d\n",komsusay);*/

  while (1){
    if (komsusay>2){
      /*printf("zagor\n");*/
      zeroassigner(field,x,y,level,n);
      level++;
      komsusay=komsu(field,x,y,level,n);}
    if (komsusay<3)
      break;

  }

  }




for(j3=0;j3<n;j3++){
  for(i3=0;i3<n;i3++){
      printf("%d ",field[i3][j3]);}
printf("\n");}
}
