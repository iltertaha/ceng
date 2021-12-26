#include "fs.h"
#include "ext2_fs/ext2.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int init_fs(const char *image_path) {
  current_fs = initialize_ext2(image_path);
  current_sb = current_fs->get_superblock(current_fs);
  return !(current_fs && current_sb);
}

struct file *openfile(char *path) {
  struct file *f = malloc(sizeof(struct file));
  f->f_path = malloc(strlen(path) + 1);
  strcpy(f->f_path, path);
  struct dentry *dir = pathwalk(path);
  if (!dir) {
    return NULL;
  }
  f->f_inode = dir->d_inode;
  free(dir);
  if (f->f_inode->f_op->open(f->f_inode, f)) {
    return NULL;
  }
  return f;
}

int closefile(struct file *f) {
  if (f->f_op->release(f->f_inode, f)) {
    printf("Error closing file\n");
  }
  free(f);
  f = NULL;
  return 0;
}

int readfile(struct file *f, char *buf, int size, loffset_t *offset) {
  if (*offset >= f->f_inode->i_size) {
    return 0;
  }
  if (*offset + size >= f->f_inode->i_size) {
    size = f->f_inode->i_size - *offset;
  }
  // May add llseek call
  return f->f_op->read(f, buf, size, offset);
}

struct dentry *pathwalk(char *path) {
  /* Allocates and returns a new dentry for a given path */
  
 // printf("pathwalk 1 \n");

  /* copy string  */
  char* pathcopy= (char*)malloc(sizeof(char)*strlen(path));
  char* prevcopy= (char*)malloc(sizeof(char)*strlen(path));
  strcpy(pathcopy,path);
  strcpy(prevcopy,path);

  //printf("pathwalk 2 \n");
  
	

  /* if given path is dot */
  if(strcmp(path, ".") == 0){
   // printf("pathwalk dot \n");

      return current_sb->s_root;
    }

    
  /* if not dot*/
  else{
     //   printf("pathwalk path \n");


    struct inode* current_inode = current_sb->s_root->d_inode;
    struct dentry* current_dentry = malloc(sizeof(struct dentry));
    struct dentry* prev;


    int count = 0;
    int count2= 0;
    char* token = strtok(pathcopy, "/"); 
    
     //printf("pathwalk path 2 \n");
     //printf("pathwalk path 2 token : %s \n",token);

    while (token != NULL) { 
		  //printf("%s\n", token); 
      count++;
      current_dentry->d_name= token;
      
      prev = current_dentry;

     // printf("lookupa verilen aranacak deger : %s \n",token);
      
      current_dentry = current_inode->i_op->lookup(current_inode,current_dentry);
    
      //printf("prev: %s \n",prev->d_inode->i_ino);
      //printf("tik \n");
      
      if(current_dentry == NULL){return NULL;}
      //printf("current: %s \n",current_dentry->d_name);
      //printf("tok \n");
      
      current_inode = current_dentry->d_inode;

		  token = strtok(NULL, "/");     
  

      //printf("pathwalk bitimi: %s \n",current_dentry->d_name);

    

  }
   current_dentry->d_parent = prev;
    return current_dentry;

}

}

int statfile(struct dentry *d_entry, struct kstat *k_stat) {
  return d_entry->d_inode->i_op->getattr(d_entry, k_stat);
}
