package ceng.ceng351.labdb;

import java.util.*;  

public class LabDB {

    
    Map<Integer,Bucket> binary_keeper=new HashMap<Integer,Bucket>();  
    public int gd;
    public int bucket_capacity;
    


    public LabDB(int bucketSize) {
        
        Bucket bucket1 = new Bucket();
        Bucket bucket2 = new Bucket();

        this.gd=1;
        this.binary_keeper.put(0,bucket1);
        this.binary_keeper.put(1,bucket2);
        
        this.bucket_capacity = bucketSize;
        
        
    }

    public void enter(String studentID) {
        if(search(studentID)=="-1"){
            String string_key = studentID.substring(1);
        int key_val = Integer.parseInt(string_key);
        int h = key_val & ((1<< this.gd) - 1);
        
        if(((binary_keeper.get(h)).bckt_vct).size() < this.bucket_capacity){
            
            ((binary_keeper.get(h)).bckt_vct).add(key_val);
       
        }
        
        else{
            split(h,binary_keeper.get(h),key_val);
            
            enter(studentID);
        }
        
        // System.out.println(key_val);
        
        }
        
        
    }

    public void leave(String studentID) {
        String string_key = studentID.substring(1);
        int key_val = Integer.parseInt(string_key);
       //System.out.println("buras1");
        eliminate(key_val);
         
        
    }

    public String search(String studentID) {
        String string_key = studentID.substring(1);
        int key_val = Integer.parseInt(string_key);
        
        
        for (Map.Entry<Integer, Bucket> entry : binary_keeper.entrySet()) {
                            Integer key = entry.getKey();
                            Bucket value = entry.getValue();                   
                            
                            for(int i=0;i<(value.bckt_vct).size();i++){
                                if((value.bckt_vct).get(i) == key_val){
                                    
                                    
                                    int lenstr = (Integer.toBinaryString(key)).length();
                                    int extrazero = this.gd - lenstr;

                                    String howmany ="";
                                    for(int m=0;m<extrazero;m++){
                                        howmany+="0";
                                    }

                                    String zeroadded = howmany + Integer.toBinaryString(key);

                                    return zeroadded;
                                    
                                    }
                            }
                            
                            
            
        }
        
        
        return "-1";
    }

    public void printLab() {
        
        System.out.println("Global depth : "+ this.gd);
        
        for (int i = 0; i < (1<<gd); i++)
	{
		/* fin */
                int lenstr = (Integer.toBinaryString(i)).length();
                int extrazero = this.gd - lenstr;
                
                String howmany ="";
                for(int m=0;m<extrazero;m++){
                    howmany+="0";
                }
                
                String zeroadded = howmany + Integer.toBinaryString(i);
                
                String s="";
                
                
                
		
		for(int j=0; j< ((binary_keeper.get(i)).bckt_vct).size() ; j++){
                    
			s+="<e"+((binary_keeper.get(i)).bckt_vct).get(j)+">";}
		System.out.println(zeroadded+" : "+"[Local depth:"+(binary_keeper.get(i)).locdep +"]"+s);
	}
        


    }
    
    /***/
    
    public void split(int bucket_no,Bucket sp_Bucket,int n) {
        
        int  directoryCount;
        int local_depth;
        int pair_locate;
        int indexOffset;
        
        //System.out.println("here1");
        (binary_keeper.get(bucket_no)).locdep = (binary_keeper.get(bucket_no)).locdep +1 ;
        //System.out.println("here2");
        local_depth = (binary_keeper.get(bucket_no)).locdep;
        //System.out.println("here3");
        if(local_depth > this.gd){
            enlarge();
        }
        //System.out.println("here4");
        pair_locate = bucket_no^(1<<(local_depth-1));
        Bucket newBucket =new Bucket();
        //System.out.println("here5");
        binary_keeper.put(pair_locate, newBucket);
         //System.out.println("here6");
        (binary_keeper.get(pair_locate)).locdep = local_depth;
          //System.out.println("here7");
        ArrayList<Integer> temp = new ArrayList<Integer>();
          //System.out.println("here8");
        for(int i = 0; i < ((binary_keeper.get(bucket_no)).bckt_vct).size(); i++) { 		      
          temp.add(((binary_keeper.get(bucket_no)).bckt_vct).get(i));
          
      } 
        
        ((binary_keeper.get(bucket_no)).bckt_vct).clear();
        
        
        indexOffset = 1 << local_depth;
        directoryCount = 1<< gd;
        
        for(int i=pair_locate-indexOffset ; i>=0;i-=indexOffset){
            binary_keeper.put(i,binary_keeper.get(pair_locate));
            
        }
        
        for(int i=pair_locate+indexOffset; i<directoryCount ; i+=indexOffset){
            binary_keeper.put(i, binary_keeper.get(pair_locate));
        }
        
        for(int i=0;i<temp.size();i++){
            String s="e"+ Integer.toString(temp.get(i));
            this.enter(s);
        }
        
        
        
    }
    public void merge(int bucket_no) {
       
        int local_depth,pair_locate;
        int flag=0;
        int indexOffset,directoryCount,i;
//System.out.println("asd1");
        local_depth = (binary_keeper.get(bucket_no)).locdep;
        //System.out.println("asd2");
        pair_locate = bucket_no^(1<<(local_depth-1));
        //System.out.println("asd3");
        indexOffset = 1<<local_depth;
        //System.out.println("asd4");
        directoryCount = 1<< this.gd;
//System.out.println("asd5");
    if( (binary_keeper.get(pair_locate)).locdep == local_depth && (binary_keeper.get(bucket_no).bckt_vct).isEmpty() && (binary_keeper.get(bucket_no)).locdep>1)
    {//System.out.println("asd6");
        flag=1;
        
        (binary_keeper.get(pair_locate)).locdep =(binary_keeper.get(pair_locate)).locdep -1;
        //System.out.println("asd7");
        binary_keeper.put(bucket_no,binary_keeper.get(pair_locate));
        //System.out.println("asd8");
        for( i=bucket_no-indexOffset ; i>=0 ; i-=indexOffset )
            binary_keeper.put(i, binary_keeper.get(pair_locate));
        for( i=bucket_no+indexOffset ; i<directoryCount ; i+=indexOffset )
            binary_keeper.put(i, binary_keeper.get(pair_locate));
    //System.out.println("asd9");
        
   //System.out.println("asd10");
   
   shrink();
   //printLab();
    }
    
    
    
    if(flag==1){merge(bucket_no);}
   
        
       /*int flag=0;
        for(int i =0;i < (1 << (this.gd-1));i++){
        
            int h = i  & ((1<< this.gd) - 1);
            
            Bucket curr = this.binary_keeper.get(i);
            //System.out.println("buras4");
            
            
            Bucket matched = this.binary_keeper.get(i ^ (1<<((this.binary_keeper.get(i)).locdep)-1))  ;
            //System.out.println(curr.locdep);
            if(curr!=matched){
                //System.out.println("buras10");
                int sz = (curr.bckt_vct).size() + (matched.bckt_vct).size();
                
                /*if(sz <= this.bucket_capacity && (curr.bckt_vct).size() ==0 && curr.locdep>1 && curr.locdep == matched.locdep)*/
               /* if( (curr.bckt_vct).size() ==0 && curr.locdep>1 && curr.locdep == matched.locdep)
		    {
                        flag=1;
                        for(int j=0;j<(matched.bckt_vct).size();j++){
                             //System.out.println("added "+ (matched.bckt_vct).get(j) );
                            (curr.bckt_vct).add((matched.bckt_vct).get(j));
                            
                            
                        
                        }
                        
                        ArrayList<Integer> inds = new ArrayList<Integer>();
                        
                        for (Map.Entry<Integer, Bucket> entry : this.binary_keeper.entrySet()) {
                            Integer key = entry.getKey();
                            Bucket value = entry.getValue();
                            
                            if(value == matched){
                                inds.add(key);
                                    
                                }
                            
                            
                            
                        }                        
                    for(int k=0;k< inds.size();k++){
                        (this.binary_keeper).put(inds.get(k),binary_keeper.get(i));
                        }
                    
                    (binary_keeper.get(i)).locdep = (binary_keeper.get(i)).locdep -1; 
                    
                     shrink();
                    }
                
                
            
            }
            
            
            
            
        }
        //shrink();
        //System.out.println("merge ended");
      if(flag==1){
          merge();
          
      }*/
    }
    
    
    
    public void shrink(){
        int flag=0;
        for (int i = 0; i < (1<<gd); i++)
	{
            if ( (this.gd <= (binary_keeper.get(i)).locdep )){
			flag=1;}
        }
    
    
            if(flag==0){this.gd = this.gd -1;}
            
        /*ArrayList<Bucket> buckets = new ArrayList<Bucket>();
        
        
        
        
        for (Map.Entry<Integer, Bucket> entry : binary_keeper.entrySet()) {
                            /*Integer key = entry.getKey();*/
                            /*Bucket value = entry.getValue();                   
                            buckets.add(value);
        }
        
        int flag=0;
        
        int sz = buckets.size();
        
        for(int i=0;i<sz ;i++){
            if ( (this.gd <= (buckets.get(i)).locdep )){
			flag=1;}
        }
        
        if(flag==0){this.gd = this.gd -1;}
        
        
    */
    }
    
    
    public void eliminate(int n){
        int bucket_ind = 0;
        boolean found;
        found = false;
        String ax="e"+ Integer.toString(n);
        
        if(search(ax) != ""){
            
            Iterator<Map.Entry<Integer, Bucket>> iterator = (this.binary_keeper).entrySet().iterator();
    //System.out.println("buras2");
    while(iterator.hasNext()){
                Map.Entry<Integer, Bucket> curr = iterator.next();
                
                /*if(((curr.getValue().bckt_vct).contains(n))){
                    (curr.getValue().bckt_vct).
                    
                }
    }*/
                Iterator<Integer> it = (curr.getValue().bckt_vct).iterator();
                while (it.hasNext()) {
                    if (it.next() == n) {
                        it.remove();
                        found=true;
                        bucket_ind = curr.getKey();
                        break;
                    }
                }
                if(found){break;}
    
            //System.out.println("buras3");
            
            
                 
            
            //shrink();
        }
    if( ((binary_keeper.get(bucket_ind)).bckt_vct).isEmpty() && ((binary_keeper.get(bucket_ind)).locdep>1)){
                 merge(bucket_ind);
    
    }
    }
        
    }
    
    public void enlarge()
        {
        for(int i = 0 ; i < 1<< this.gd; i++ )
                binary_keeper.put((1<<gd)+i,binary_keeper.get(i));
            gd++;
        }
    
    
}
