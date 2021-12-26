/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "2236891",              /* Student ID */

    "İlter Taha Aktolga",     /* full name */
    "ilter.aktolga@metu.edu.tr",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * Sobel KERNEL
 ***************/

/******************************************************
 * Your different versions of the sobel functions  go here
 ******************************************************/

/* 
 * naive_sobel - The naive baseline version of Sobel 
 */
char naive_sobel_descr[] = "sobel: Naive baseline implementation";
void naive_sobel(int dim,int *src, int *dst) {
    int i,j,k,l;
    int ker[3][3] = {{-1, 0, 1}, 
                     {-2, 0, 2}, 
                     {-1, 0, 1}};  

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {	
	   dst[j*dim+i]=0;
            if(!((i == 0) || (i == dim-1) || (j == 0) || (j == dim-1))){
            for(k = -1; k <= 1; k++)
                for(l = -1; l <= 1; l++) {
                 dst[j*dim+i]=dst[j*dim+i]+src[(j + l)*dim+(i + k)] * ker[(l+1)][(k+1)]; 
                }
		       

      }
      
}
}




char mysobel_unroll16_descr[] = "sobel: accum unroll 16";
void mysobel_unroll16(int dim,int *src, int *dst){


int i,j;
	int slide = dim -2 ;

	int mult;
	
	/*int ker[9] = {-1,0,1,-2,0,2,-1,0,1};*/

	int k0 = -1;
	int k1 =  0;
	int k2 =  1;
	int k3 = -2;
	int k4 =  0;
	int k5 =  2;
	int k6 = -1;
	int k7 =  0;
	int k8 =  1;

	
	int *dest_ptr,*src_ptr;
	int *rowwise; 	
	
	int *srciter;
	srciter = src;

	/*int bottom = (dim-1) * dim;*/
	
	int mult_motion = (dim-1)*dim;
	


	/*for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			if( i==0 || j ==0 || i==dim-1 || j ==dim-1){
				dst[i*dim+j] = 0;
			}
		}
	}*/
	/*for(i=0;i<dim;i++){
		//dst[0][i] = 0;
		//dst[i][dim-1] = 0;

		//dst[i][0] = 0;
		//dst[dim-1][i] =0;
		dst[i] = 0;
		dst[i*dim+dim-1] = 0;

		dst[i*dim] = 0;
		dst[mult_motion+i] =0;
				
}	*/

	int* temp_dst;
	temp_dst = dst;
	dst +=dim+1;
	
	int acc1,acc2,acc3,acc4,acc5,acc6,acc7,acc8,acc9,acc10,acc11,acc12,acc13,acc14,acc15,acc16;
	for(i = 0; i < slide; i++){
		src_ptr = srciter;
		dest_ptr = dst;

		for(j = 0; j< slide;j+=16){
			acc1=acc2=acc3=acc4=acc5=acc6=acc7=acc8=acc9=acc10=acc11=acc12=acc13=acc14=acc15=acc16=0;
	
			rowwise = src_ptr;	
			int val1 = rowwise[0];
			int val2 = rowwise[1];
			int val3 = rowwise[2];

			int val4 = rowwise[3];
			int val5 = rowwise[4];
			int val6 = rowwise[5];
			int val66 =rowwise[6];
			int val666 = rowwise[7];
			int val6666 =rowwise[8];
			int val66666=rowwise[9];
			
			int val666666 =rowwise[10]; /*unroll 9*/
			int val6666666 =rowwise[11]; /*//unroll 10*/
			int val66666666 =rowwise[12]; /*//unroll -*/
			int val666666666 =rowwise[13]; /*//unroll -*/
			int val6666666666 =rowwise[14];/* //unroll -*/
			int val66666666666 =rowwise[15];/* //unroll -*/
			int val666666666666 =rowwise[16];/* //unroll -*/
			int val6666666666666 =rowwise[17]; /*//unroll 16*/
				

			rowwise += dim;	

			int val7 = rowwise[0];
			int val8 = rowwise[1];
			int val9 = rowwise[2];

			int val10 = rowwise[3];
			int val11 = rowwise[4];
			int val12 = rowwise[5];	

			int val122 =rowwise[6];
			int val1222 = rowwise[7];
			int val12222 =rowwise[8];
			int val122222=rowwise[9];


			int val1222222 =rowwise[10]; /*//unroll 9*/
			int val12222222 =rowwise[11]; /*//unroll 10*/
			int val122222222 =rowwise[12]; /*//unroll -*/
			int val1222222222 =rowwise[13]; /*//unroll -*/
			int val12222222222 =rowwise[14]; /*//unroll -*/
			int val122222222222 =rowwise[15]; /*//unroll -*/
			int val1222222222222 =rowwise[16]; /*//unroll -*/
			int val12222222222222 =rowwise[17]; /*//unroll 16*/
			
			rowwise += dim;			
	
			int val13 = rowwise[0];
			int val14 = rowwise[1];
			int val15 = rowwise[2];

			int val16 = rowwise[3];
			int val17 = rowwise[4];
			int val18 = rowwise[5];	

			int val188 =rowwise[6];
			int val1888 = rowwise[7];
			int val18888 =rowwise[8];
			int val188888=rowwise[9];

			int val1888888 =rowwise[10]; /* //unroll 9*/
			int val18888888 =rowwise[11]; /*//unroll 10*/
			int val188888888 =rowwise[12]; /*//unroll -*/
			int val1888888888 =rowwise[13]; /*//unroll -*/
			int val18888888888 =rowwise[14]; /*//unroll -*/
			int val188888888888 =rowwise[15]; /*//unroll -*/
			int val1888888888888 =rowwise[16]; /*//unroll -*/
			int val18888888888888 =rowwise[17]; /*//unroll 16*/


			acc1+=val1*k0;
			acc1+=val2*k1;
			acc1+=val3*k2;
			acc1+=val7*k3;
			acc1+=val8*k4;
			acc1+=val9*k5;
			acc1+=val13*k6;
			acc1+=val14*k7;
			acc1+=val15*k8;
			
			acc2+=val2 *k0;
			acc2+=val3 *k1;
			acc2+=val4 *k2;
			acc2+=val8 *k3;
			acc2+=val9 *k4;
			acc2+=val10 *k5;
			acc2+=val14 *k6;
			acc2+=val15 *k7;
			acc2+=val16 *k8;
			
			acc3+=val3 *k0;
			acc3+=val4 *k1;
			acc3+=val5 *k2;
			acc3+=val9 *k3;
			acc3+=val10 *k4;
			acc3+=val11 *k5;
			acc3+=val15 *k6;
			acc3+=val16 *k7;
			acc3+=val17 *k8;
					
			acc4+=val4 *k0;
			acc4+=val5 *k1;
			acc4+=val6 *k2;
			acc4+=val10 *k3;
			acc4+=val11 *k4;
			acc4+=val12 *k5;
			acc4+=val16 *k6;
			acc4+=val17 *k7;
			acc4+=val18 *k8;
			
			/**/
			acc5+=val5*k0;
			acc5+=val6*k1;
			acc5+=val66*k2;
			acc5+=val11*k3;
			acc5+=val12*k4;
			acc5+=val122*k5;
			acc5+=val17*k6;
			acc5+=val18*k7;
			acc5+=val188*k8;
			
			acc6+=val6 *k0;
			acc6+=val66 *k1;
			acc6+=val666 *k2;
			acc6+=val12 *k3;
			acc6+=val122 *k4;
			acc6+=val1222 *k5;
			acc6+=val18 *k6;
			acc6+=val188 *k7;
			acc6+=val1888 *k8;
			
			acc7+=val66 *k0;
			acc7+=val666 *k1;
			acc7+=val6666 *k2;
			acc7+=val122 *k3;
			acc7+=val1222 *k4;
			acc7+=val12222 *k5;
			acc7+=val188 *k6;
			acc7+=val1888 *k7;
			acc7+=val18888 *k8;
					
			acc8+=val666 *k0;
			acc8+=val6666 *k1;
			acc8+=val66666 *k2;
			acc8+=val1222 *k3;
			acc8+=val12222 *k4;
			acc8+=val122222 *k5;
			acc8+=val1888 *k6;
			acc8+=val18888 *k7;
			acc8+=val188888 *k8;

			/*unroll 16*/
			acc9+=val6666*k0;
			acc9+=val66666*k1;
			acc9+=val666666*k2;
			acc9+=val12222*k3;
			acc9+=val122222*k4;
			acc9+=val1222222*k5;
			acc9+=val18888*k6;
			acc9+=val188888*k7;
			acc9+=val1888888*k8;
			
			acc10+=val66666 *k0;
			acc10+=val666666 *k1;
			acc10+=val6666666 *k2;
			acc10+=val122222 *k3;
			acc10+=val1222222 *k4;
			acc10+=val12222222 *k5;
			acc10+=val188888 *k6;
			acc10+=val1888888 *k7;
			acc10+=val18888888 *k8;
			
			acc11+=val666666 *k0;
			acc11+=val6666666 *k1;
			acc11+=val66666666 *k2;
			acc11+=val1222222 *k3;
			acc11+=val12222222 *k4;
			acc11+=val122222222 *k5;
			acc11+=val1888888 *k6;
			acc11+=val18888888 *k7;
			acc11+=val188888888 *k8;
					
			acc12+=val6666666 *k0;
			acc12+=val66666666 *k1;
			acc12+=val666666666 *k2;
			acc12+=val12222222 *k3;
			acc12+=val122222222 *k4;
			acc12+=val1222222222 *k5;
			acc12+=val18888888 *k6;
			acc12+=val188888888 *k7;
			acc12+=val1888888888 *k8;
			
			/**/
			acc13+=val66666666*k0;
			acc13+=val666666666*k1;
			acc13+=val6666666666*k2;
			acc13+=val122222222*k3;
			acc13+=val1222222222*k4;
			acc13+=val12222222222*k5;
			acc13+=val188888888*k6;
			acc13+=val1888888888*k7;
			acc13+=val18888888888*k8;
			
			acc14+=val666666666 *k0;
			acc14+=val6666666666 *k1;
			acc14+=val66666666666 *k2;
			acc14+=val1222222222 *k3;
			acc14+=val12222222222 *k4;
			acc14+=val122222222222 *k5;
			acc14+=val1888888888 *k6;
			acc14+=val18888888888 *k7;
			acc14+=val188888888888 *k8;
			
			acc15+=val6666666666 *k0;
			acc15+=val66666666666 *k1;
			acc15+=val666666666666 *k2;
			acc15+=val12222222222 *k3;
			acc15+=val122222222222 *k4;
			acc15+=val1222222222222 *k5;
			acc15+=val18888888888 *k6;
			acc15+=val188888888888 *k7;
			acc15+=val1888888888888 *k8;
					
			acc16+=val66666666666*k0;
			acc16+=val666666666666 *k1;
			acc16+=val6666666666666 *k2;
			acc16+=val122222222222 *k3;
			acc16+=val1222222222222 *k4;
			acc16+=val12222222222222 *k5;
			acc16+=val188888888888 *k6;
			acc16+=val1888888888888 *k7;
			acc16+=val18888888888888 *k8;
			/*unroll 16 ends*/



                        
			*(dest_ptr) = acc1;
			*(dest_ptr+1)=acc2;
			*(dest_ptr+2)=acc3;
			*(dest_ptr+3)=acc4;	
			*(dest_ptr+4) = acc5;
			*(dest_ptr+5)=acc6;
			*(dest_ptr+6)=acc7;
			*(dest_ptr+7)=acc8;
			*(dest_ptr+8)=acc9;
			*(dest_ptr+9)=acc10;
			*(dest_ptr+10)=acc11;
			*(dest_ptr+11)=acc12;
			*(dest_ptr+12)=acc13;
			*(dest_ptr+13)=acc14;
			*(dest_ptr+14)=acc15;
			*(dest_ptr+15)=acc16;
			src_ptr+=16;
			dest_ptr+=16;
}
	
		srciter += dim;
		dst += dim;


		/*loop saber*/
		/*dst[i] = 0; //up
		dst[mult_motion+i] =0; //down
		dst[slide*dim+dim-1] = 0; //right
		dst[slide*dim] = 0; //left
		*/
		
		int idim = i*dim;

		temp_dst[i] = 0; 
		temp_dst[idim+dim-1] = 0; 

		temp_dst[idim] = 0;	
		temp_dst[mult_motion+i] =0; 

	}
		
		int idim = i*dim;
		temp_dst[i] = 0; 
		temp_dst[idim+dim-1] = 0; 

		temp_dst[idim] = 0;	
		temp_dst[mult_motion+i] =0; 

		i++;
		idim =i *dim;
		temp_dst[i] = 0; 
		temp_dst[idim+dim-1] = 0; 

		temp_dst[idim] = 0;	
		temp_dst[mult_motion+i] =0; 
		/*note for me*/
		
		/*dst[i] = 0;
		dst[i*dim+dim-1] = 0;

		dst[i*dim] = 0;
		dst[mult_motion+i] =0;*/
		
}

/**/
/****/

/* 
 * sobel - Your current working version of sobel
 * IMPORTANT: This is the version you will be graded on
 */

char sobel_descr[] = "Dot product: Current working version";
void sobel(int dim,int *src,int *dst) 
{

       /*naive_sobel(dim,src,dst);*/
	mysobel_unroll16(dim,src,dst);

}

/*********************************************************************
 * register_sobel_functions - Register all of your different versions
 *     of the sobel functions  with the driver by calling the
 *     add_sobel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_sobel_functions() {
    add_sobel_function(&naive_sobel, naive_sobel_descr);   
    add_sobel_function(&sobel, sobel_descr);   
    /* ... Register additional test functions here */
}




/***************
 * MIRROR KERNEL
 ***************/

/******************************************************
 * Your different versions of the mirror func  go here
 ******************************************************/

/* 
 * naive_mirror - The naive baseline version of mirror 
 */
char naive_mirror_descr[] = "Naive_mirror: Naive baseline implementation";
void naive_mirror(int dim,int *src,int *dst) {
    
 	 int i,j;

  for(j = 0; j < dim; j++)
        for(i = 0; i < dim; i++) {
            dst[RIDX(j,i,dim)]=src[RIDX(j,dim-1-i,dim)];

        }

}




char mymirror8ex2_descr[] = "my_mirror8ex2:My implementation";
void mymirror8ex2(int dim,int *src,int *dst) {
    
 	int i,j;
	
	int *src_ptr = src;
	/*int *dst_ptr = dst;*/

	/*int* src_devam;*/

	src_ptr = src_ptr+ dim-1;
	for(i = dim; i >0;--i){
		/*src_devam= src_ptr;*/
        	for( j= dim; j > 0; j-=32) {

			*dst = *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);
src_ptr -=4;
dst +=4;
			*dst = *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);	
src_ptr -=4;
dst +=4;
			*dst= *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);
src_ptr -=4;
dst +=4;
			*dst= *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);
src_ptr -=4;
dst +=4;
   			*dst= *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);
src_ptr -=4;
dst +=4;
			*dst= *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);
src_ptr -=4;
dst +=4;
			*dst = *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);
src_ptr -=4;
dst +=4;
			*dst = *src_ptr;
			*(dst+1) = *(src_ptr-1);
			*(dst+2) = *(src_ptr-2);
			*(dst+3) = *(src_ptr-3);
src_ptr -=4;
dst +=4;

		   	/**dst++ = *src_ptr;
			*dst++ = *(src_ptr-1);
			*dst++ = *(src_ptr-2);
			*dst++ = *(src_ptr-3);

			*dst++ = *(src_ptr-4);
			*dst++ = *(src_ptr-5);
			*dst++ = *(src_ptr-6);
			*dst++ = *(src_ptr-7);		   	

			*dst++ = *(src_ptr-8);
			*dst++ = *(src_ptr-9);
			*dst++ = *(src_ptr-10);
			*dst++ = *(src_ptr-11);

			*dst++ = *(src_ptr-12);
			*dst++ = *(src_ptr-13);
			*dst++ = *(src_ptr-14);
			*dst++ = *(src_ptr-15);		

   			*dst++ = *(src_ptr-16);
			*dst++ = *(src_ptr-17);
			*dst++ = *(src_ptr-18);
			*dst++ = *(src_ptr-19);

			*dst++ = *(src_ptr-20);
			*dst++ = *(src_ptr-21);
			*dst++ = *(src_ptr-22);
			*dst++ = *(src_ptr-23);		   	

			*dst++ = *(src_ptr-24);
			*dst++ = *(src_ptr-25);
			*dst++ = *(src_ptr-26);
			*dst++ = *(src_ptr-27);

			*dst++ = *(src_ptr-28);
			*dst++ = *(src_ptr-29);
			*dst++ = *(src_ptr-30);
			*dst++ = *(src_ptr-31);	

			src_ptr -=32;*/
        	}
	/*src_ptr++;*/
	src_ptr+=dim<<1;

			/**/

			/**/
	

}

}


/* 
 * mirror - Your current working version of mirror
 * IMPORTANT: This is the version you will be graded on
 */
char mirror_descr[] = "Mirror: Current working version";
void mirror(int dim,int *src,int *dst) 
{

        
 	/*naive_mirror(dim,src,dst);
*/

	mymirror8ex2(dim,src,dst);
}

/*********************************************************************
 * register_mirror_functions - Register all of your different versions
 *     of the mirror functions  with the driver by calling the
 *     add_mirror_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_mirror_functions() {
    add_mirror_function(&naive_mirror, naive_mirror_descr);   
    add_mirror_function(&mirror, mirror_descr);   
    /* ... Register additional test functions here */
}

