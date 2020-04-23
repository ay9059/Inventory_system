#include <stdio.h>
#include "inventory.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


//returns a 0 if the id of the first part comes before
//the second, alphabetically. Returns 1 otherwise.
int part_compare(const void * first , const void * second ){
   part_t * first_item = *(part_t **) first;
   part_t * second_item =  *(part_t **) second;
   return strcmp(first_item->id,second_item->id);
}


//returns a 0 if the id of the first assembly comes before
//the second, alphabetically. Returns 1 otherwise.
int assembly_compare(const void * first, const void * second){
   assembly_t * first_assembly = *(assembly_t  **) first;
   assembly_t * second_assembly = *(assembly_t **) second;
   return strcmp(first_assembly->id,second_assembly->id);
}


//returns a 0 if the id of the first item comes before
//the second, alphabetically. Returns 1 otherwise.
int item_compare(const void * first, const void * second){
   item_t * first_item = *(item_t **) first;
   item_t * second_item = *(item_t **) second;
   return strcmp(first_item->id,second_item->id);
}

part_t ** to_part_array(int count, part_t * part_list){
   part_t ** part_array = malloc(sizeof(part_t *)*count);
   int counter = 0;
   while(counter!=count){
      part_array[counter]=part_list;
      part_list = part_list->next; 
      counter++;
   }
   qsort(part_array, count, sizeof(part_t *), part_compare);
   return part_array;      

}



item_t ** to_item_array(int count, item_t * item_list){
    item_t ** item_array = calloc(count,sizeof(item_t *));
    int counter=0;
   while(counter!=count){
      item_array[counter]=item_list;
      item_list = item_list->next; 
      counter++;
   }
   
   qsort(item_array, count, sizeof(item_t *), item_compare);
   counter = counter+count;
   return item_array;      

}

//Allocates space for an assembly_t pointer array and populates it fields
//and later calls sort on it.
assembly_t ** to_assembly_array(int count, assembly_t * assembly_list){
   assembly_t ** assembly_array = calloc(count, sizeof(assembly_t *));
   int counter = 0;
   while(assembly_list!=NULL){
      assembly_array[counter]=assembly_list;
      assembly_list = assembly_list->next; 
      counter++;
   }
   qsort(assembly_array, count, sizeof(assembly_t *), assembly_compare);

   return assembly_array;      
   
}
