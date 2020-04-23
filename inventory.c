//Inventory.c implements an interactive inventory system for a company 
//that makes assemblies from parts and sells them to a customer. The 
//system also keeps track of the parts which were used to manufacture 
//the products. The program also allows to restock when items are too 
//low and reporting the state of inventory.
//
//@author: Abhishek Yadav <ay9059@rit.edu>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "trimit.h"
#include "inventory.h"

//lookup_assembly searches for an assembly with the specified id
//and returns a pointer to that assemly if it exists otherwise
//it returns a NULL pointer.
//@param (assembly_t *) ap: a linked list of assemblies
//@param (char *) id: the name of the assembly to lookup
//@return: an assembly_t pointer if it exists
assembly_t * lookup_assembly(assembly_t * ap, char * id){
  assembly_t * iterator = ap;
   while(iterator!=NULL){
     if(strcmp(iterator->id,id)==0){
        return iterator;
     }
     if(iterator->next!=NULL){
        iterator = iterator->next;
     }else{
        break;
     }
   } 
   return NULL;

}

//lookup_part searches for a part with the specified id
//and returns a pointer to that part if it exists otherwise
//it returns a NULL pointer.
//@param (part_t *) pp: a linked list of parts
//@param (char *) id: the name of the part to lookup
//@return: a part_t pointer if it exists
part_t * lookup_part(part_t * pp, char * id){
   part_t * iterator = pp;
   while(iterator!=NULL){
     if(strcmp(iterator->id,id)==0){
        return iterator;
     }
     if(iterator->next!=NULL){
        iterator = iterator->next;
     }else{
        break;
     }
   } 
   return NULL;
}



//lookup_part searches for an item with the specified id
//and returns a pointer to that item if it exists otherwise
//it returns a NULL pointer.
//@param (item_t *) ip: a linked list of items
//@param (char *) id: the name of the item to lookup
//@return: an item_t pointer if it exists
item_t * item_lookup(item_t * ip, char * id){
  item_t * iterator = ip;
   while(iterator!=NULL){
     if(strcmp(iterator->id,id)==0){
        return iterator;
     }

     if(iterator->next!=NULL){
        iterator = iterator->next;
     }else{
        break;
     }
   } 
   return NULL;
}

//Adds a part to the inventory 
//@param (inventory_t) p: refers to the inventory where the part
//                        is to be added
//@param (char *) id: the id of the part 
//@return: void
void add_part(inventory_t * p,char * id){

   if(( id[0]=='P') && (int) strlen(id)<=11  ){
      part_t * new_part= malloc(sizeof(part_t));
      new_part->next=NULL;
      strcpy(new_part->id,id);
      int already=0;
       
      part_t * iterator = p->part_list;
      if(p->part_list==NULL){
         p->part_list=new_part;
	 p->part_count++;
      }
      else{
	if( strcmp(iterator->id,id)==0){
	   already=1;
	}
         while(iterator->next!=NULL){
            if(strcmp(iterator->id,id)==0||strcmp(iterator->next->id,id)==0) {
       	       already=1;
	    }
           iterator=iterator->next;
         }
     	 if(already!=1){
            iterator->next = new_part;
            p->part_count++;
	 }
	 else if(already==1){
            fprintf(stderr,"!!! %s: duplicate part ID\n",id );

            free(new_part);
         }
      }
   }else if((int) strlen(id)>11){
      fprintf(stderr,"!!! %s: part ID too long\n",id );
   }   
   else {
      fprintf(stderr, "!!! %s: part ID, must start with \'P\' \n",id );

    }
   }



//Add item is the function which is responsible for adding a single item in the
//items_needed list. It adds the item at the end of the items_needed list.
//This function also conducts the necessary error checkings for instance, the 
//max length, the first letter if its capital A and if the assembly already
//exists.
//@param (inventory_t *) invp: pointer to inventory.
//@param (char *) id: the id of the new assembly
//@param (int) quantity: the quantity of new item
//@param (items_needed_t): a pointer to items_needed which is allocated in
//                         read_input()
void add_item(items_needed_t * items, char * id, int quantity){
  item_t * item = item_lookup(items->item_list,id);
  if(item!=NULL){
     item->quantity =item->quantity+ quantity;
  }else{
     items->item_count=items->item_count+1;
     item_t * new_item = malloc(sizeof(item_t));
     strcpy(new_item->id,id);
     new_item->quantity=quantity;
     item_t * iterator = items->item_list;
     if(iterator==NULL){
        items->item_list = new_item;
     }else{
        while(1){
	   if(iterator->next==NULL){
	      iterator->next = new_item;
	      break;
	   }
	   iterator = iterator->next;
	}
     }

   }
}



//Add items needed is the function which is responsible for adding items in the
//required items list for a given assembly. It adds the item at the end of the list
//This function also conducts the necessary error checkings for instance, the 
//max length, and if the assembly already exists or not.
//
//@param (inventory_t *) invp: pointer to inventory.
//@param (char *) id: the id of the new assembly
//@param (int) quantity: the quantity of the item
//@param (items_needed_t): a pointer to an items needed allocated in read_input
void add_items_needed(inventory_t * invp, items_needed_t * needed, char * id, int quantity){
   item_t * need = needed->item_list; // reference to the linked list 
   item_t* new = malloc(sizeof(item_t));
   strcpy(new->id,id);  //since new is an array
   new->quantity = quantity;
   if(needed!=NULL && quantity>0){
      part_t * part_needed = lookup_part(invp->part_list,id);
     assembly_t *  assembly_needed = lookup_assembly(invp->assembly_list,id);
       if(part_needed!=NULL || assembly_needed!=NULL ){
	 if(need==NULL){
            needed->item_count++;
	    needed->item_list = new;
	 }else{
	    while(1){
	       item_t * item_look = item_lookup(need,id);
		if(item_look!=NULL){
		   free(new);
		   item_look->quantity = item_look->quantity + quantity;
		   return;
		}
	       if(need->next==NULL){
	          need->next = new;
		  break;
	       }
	       need = need->next;
	    }
	 }	 
	 }else{
	   fprintf(stderr,"!!! %s: part/assembly ID is not in inventory\n",id ); 
	 }
      }else if(quantity<=0){
              free(new);
	      if(id[0]=='P'){
         fprintf(stderr,"!!! %d: illegal quantity for part ID %s \n",quantity,id);
      }else if(id[0]=='A'){
      
         fprintf(stderr,"!!! %d: illegal quantity for Assembly ID %s \n",quantity,id);
      
      }

      
      }
}


//Add assembly is the function which is responsible for adding an assembly in the
//assembly list. It adds the assembly at the end of the assembly_list.
//This function also conducts the necessary error checkings for instance, the 
//max length, the first letter if its capital A and if the assembly already
//exists.
//@param (inventory_t *) invp: pointer to inventory.
//@param (char *) id: the id of the new assembly
//@param (int) capacity: the capacity of new assembly
//@param (items_needed_t)
void add_assembly(inventory_t * invp, char * id, int capacity, items_needed_t * items){
   if(( id[0]=='A' || id[0]=='a') && capacity>=0 && (int) strlen(id) <=11 ){
      assembly_t * new_assembly = malloc(sizeof(assembly_t));
      strcpy(new_assembly->id,id);
      int already=0;
      new_assembly->on_hand=0;
      new_assembly->capacity = capacity;
      new_assembly->items = items;
      assembly_t * iterator = invp->assembly_list;

     if(iterator==NULL){
        invp->assembly_list=new_assembly;
        invp->assembly_count++;
     }
     else{
	if(strcmp(iterator->id,id)==0){
	   already =1;
	}
        while(iterator->next != NULL){
           iterator= iterator->next;
           if(strcmp(iterator->id,id)==0){
	      already=1;
	   }
        } 
         if(already==0){
            iterator->next = new_assembly;
            invp->assembly_count++;
        }else{
            fprintf(stderr,"!!! %s: duplicate assembly ID\n",id);
	    free(new_assembly);
        
         }
     }
}else if((int) strlen(id)>11){
   fprintf(stderr,"!!! %s:  assembly ID too long\n",id );

}else if(capacity<=0){
   fprintf(stderr, "!!! %d: illegal capacity for %s\n",capacity, id );
}
else{
  fprintf(stderr,"!!! %s assembly id must start with \'A\'\n ",id );

}
}


//This function frees the item_t structs which are present
//in items_needed_t. After it is done freeing all the items,
//it frees items_needed_t struct which is passed as a parameter.
//@param (items_needed_t *) items: a reference to items_needed_t
//which contains linked list of item_t structs.
void free_items_needed(items_needed_t * items){
   item_t* n = items->item_list;
   while(n){
      item_t * new = n;
      n=n->next;
      free(new);
   }

}


//Free inventory frees the assembly list, part list and the
//inventory itself.
//@param (inventory_t *) invp: a pointer to inventory which
//is allocated in the main function.
void free_inventory(inventory_t * invp){
   //free the parts from the inventory
   part_t * n = invp->part_list;
   while(n){
      part_t * new = n;
      n=n->next;
      free(new);
   }
   //free assemblies
   assembly_t * a = invp->assembly_list;

   while(a){

      assembly_t * b = a;
      item_t * items = b->items->item_list;
      while(items){

        item_t * other = items;
	items = items->next;
        free(other);	
      }
     
      free(b->items);
      a=a->next;
      free(b);
   }

  //finally, free the inventory itself
  free(invp);


}


//Make function is used to manufacture assembly and parts of id specified in the parameter.
//If there is the required amount of assemblies, it makes a call to get instead. 
//function which retrieves the remaining amount of assemblies and the parts
//recursively.
//@param (char *) id: The id of assembly to be retrieved
//@param (int) n:the quantity of  assembly
//@param (items_needed_t *) parts: a reference to the parts list for the assembly
void make(inventory_t * invp, char * id,int n,items_needed_t * parts,int update){
   if(n<=0){
	   return;
   }
    if(id[0]!='A'){
       fprintf(stderr,"!!! %s must be the ID of an assembly\n",id);
    }
    assembly_t * assembly_object = lookup_assembly(invp->assembly_list,id);
    item_t * item_pointer = assembly_object->items->item_list;
    if(n==0){
       return;
    }
     printf(">>> make %d units of %s\n",n, (char *) id);
      if(item_pointer!=NULL){
       while(1){
       if(item_pointer->id[0]=='P' ){
          add_item(parts,item_pointer->id ,item_pointer->quantity*n); //create new item if its a part
       }else{
           get(invp,item_pointer->id,item_pointer->quantity*n,parts, update );
       }
       if(item_pointer->next==NULL){
          break;
       }
       item_pointer = item_pointer->next;
       }
   }
   }


//Get function is used to retrieve assembly of id specified in the parameter.
//If there isn't the required amount of assemblies, it makes a call to make
//function which manufactures the remaining amount of assemblies and the parts
//recursively.
//@param (char *) id: The id of assembly to be retrieved
//@param (int) n:the quantity of  assembly
//@param (items_needed_t *) parts: a reference to the parts list for the assembly
//note to self: refactor code in make and return error if assembly_object is NULL
void get(inventory_t * invp, char * id, int n, items_needed_t * parts,int update ){
   if (n<=0){
      return;
   }
    assembly_t * assembly_object = lookup_assembly(invp->assembly_list,id);
       if(assembly_object!=NULL){
          if(assembly_object->on_hand>=n){
             assembly_object->on_hand-=n;
       
          }else{
             make(invp,id,n-assembly_object->on_hand , parts,update);
             assembly_object->on_hand=0;
           }
        }
   }


//Prints out the parts inventory from the main inventory
//which is passed in as a parameter
//@param (inventory_t *) invp: a pointer to inventory
void print_parts(inventory_t * invp){
   printf("Part inventory:\n");
   printf("---------------\n");
   if(invp->part_list!=NULL){
      printf("Part ID\n");
      printf("===========\n");
      part_t ** part_iterator = to_part_array(invp->part_count,invp->part_list);
      int count = 0;
      while(count!=invp->part_count){
         printf("%s\n",part_iterator[count]->id);
         count++;
      }

     free(part_iterator);
  }else{
    printf("NO PARTS\n");
  }
}

//This function prints out the items needed which is passed in
//as a parameter to the function. This function also uses foramtted
//printing method for alignment
//@param (items_needed_t *) items: a pointer to items_needed_t
void print_items_needed(items_needed_t * items){
   item_t * iterator = items->item_list ;
   item_t ** item_array = to_item_array(items->item_count,items->item_list);
   int counter=0;
   if(iterator!=NULL){
      printf("Parts list:\n");
      printf("-----------\n");
      printf("Part ID     quantity\n");
      printf("=========== ========\n");

      while(counter!=items->item_count){
         printf("%-12s ",item_array[counter]->id);
	 printf("%7d\n",item_array[counter]->quantity);
	 
         counter++;
      
      }
  }
   free(item_array);
}
  


//Prints the contents of inventory's assembly and also frees the items_neeeded struct
//afterwards
//@param (inventory_t *) invp: the main inventory used to access the assemblies
//
void print_inventory(inventory_t * invp){
   printf("Assembly inventory:\n");
   printf("-------------------\n");

   if(invp->assembly_list!=NULL){
   printf("Assembly ID Capacity On Hand\n");
   printf("=========== ======== =======\n");
   assembly_t ** assembly_iterator = to_assembly_array(invp->assembly_count,invp->assembly_list);
      int count = 0;
      while(count!=invp->assembly_count ){
         printf("%-12s",assembly_iterator[count]->id);
         printf(" %7d    ",assembly_iterator[count]->capacity);

	 //if on_hand value is less than half the capacity , add an asterisk
         if(assembly_iterator[count]->on_hand<assembly_iterator[count]->capacity/2){
            printf("%3d*\n",assembly_iterator[count]->on_hand);
         }else{
            printf("%3d\n",assembly_iterator[count]->on_hand);
         }
         count++;
   }
      free(assembly_iterator);
   }else{
   
      printf("EMPTY INVENTORY\n");
   }
}

//Empties the assembly of the provided id
//@param (inventory_t *) invp: the main inventory used to access the assemblies and
//                             parts
//@param (char *) id: the id of assembly that is to be emptied
void empty(inventory_t * invp,char * id ){
   assembly_t * assembly = lookup_assembly(invp->assembly_list,id);
   if(assembly!=NULL){
      assembly->on_hand=0;
   }
}

//stock is called in read_input and stocks the number of quatites that
//the user provides for an existing assembly. This function also constructs the items
//needed list and prints them out at the end of the stock request. This function
//does error checking accordingly. If the stock quantity exceeds the capacity,
//the quantity will be set to the assembly's maximum capacity instead. If the capacity
//is 0, The assembly cannot be stocked.
//
//@param (inventory_t *) invp: the main inventory usedc to access the assemblies and
//                             parts
//@param (char *) id: the id of assembly that is to be stocked
//@param (int) quantity: number of units to be stocked.
void stock (inventory_t * invp, char * id, int quantity){

	printf("+ stock %s %d\n",id,quantity);
	//make sure quantity is greater than zero
	if(quantity<=0){
	   fprintf(stderr,"!!! %d: illegal quantity for ID %s\n",quantity,id);
	   return;
	}
	assembly_t * assembly = lookup_assembly(invp->assembly_list, id);
	//make sure assembly is not null
	if(assembly==NULL){
	   fprintf(stderr,"!!! %s: Assembly ID is not in the inventory\n",id );
	   return;
	}
        items_needed_t * parts = malloc(sizeof(items_needed_t));
	parts->item_count=0;
        int vacant = assembly->capacity - assembly->on_hand;
         if(quantity>vacant){
           quantity = vacant;
         }


	if(assembly!=NULL){
	   if(assembly->capacity>0){	
              make(invp, id, quantity, parts,1);
	      assembly->on_hand = quantity;
              print_items_needed(parts);
	      free(parts);
	      
	   }
	}else{
	   fprintf(stderr,"!!! %s: assembly ID is not in inventory\n",id);
	}
}


//fulfill_order is called in read_input and fulfills the number of orders that
//the user makes for an existing assembly. This function also constructs the items
//needed list and prints them out at the end of the request fulfillmen. This function
//does error checking accordingly.
//
//@param (inventory_t *) invp: the main inventory usedc to access the assemblies and
//                             parts
//@param (assembly_t *) assembly:
void fulfill_order(inventory_t * invp, assembly_t * assembly,char * xi , int ni){

	   if(assembly==NULL){
	      fprintf(stderr,"!!! %s: assembly ID is not in the inventory -- order cancelled\n",xi);
	      return;
	   }
	   printf("+ fulfillOrder %s %d\n",xi,ni);
	   if(ni<=0){
	      fprintf(stderr,"!!! %d: illegal order quantity for ID %s -- order canceled\n",ni,xi); 
	     return; 
	   }
           items_needed_t * items = malloc(sizeof(items_needed_t));
	   
	   while(1){
	      add_item(items,xi,ni);
	       xi = strtok(NULL," ");
	       if(xi==NULL || xi[0]=='#'){
	          break;
	       }
	   }
	   items_needed_t * parts = malloc(sizeof(items_needed_t));
           item_t * iterator = items->item_list;
	   while(iterator!=NULL){
	      get(invp,iterator->id,iterator->quantity,parts,0 );
	      iterator=iterator->next;
	   }
              //free the heaps and their items, iteratively.
              free_items_needed(items);
              free(items);
              print_items_needed(parts);
              free_items_needed(parts);
	      free(parts);
}

//This function prints out the list of available commands to stdin
//and takes in no parameter.
//
//It is only called in read_input()
void print_help(){
     printf("Requests:\n");
     printf("    addPart\n");
     printf("    addAssembly ID capacity [x1 n1 [x2 n2 ...]]\n");
     printf("    fulfillOrder [x1 n1 [x2 n2 ...]]\n");
     printf("    stock ID n\n");
     printf("    restock [ID]\n");
     printf("    empty ID\n");
     printf("    inventory [ID]\n");
     printf("    parts\n");
     printf("    help\n");
     printf("    clear\n");
     printf("    quit\n");
}

//This function implements the restock request of the program, it has
//two arguments inventory_t * invp and char *id. ID can be null or an
//ID to an existing assembly. If it is not an ID to an existing assembly
//error is reported
//
//@param (inventory_t *) invp: the main inventory
//@param (char *) id: The id of the assembly that is to be restocked.
void restock(inventory_t * invp, char * id){
   
   items_needed_t * parts = malloc(sizeof(items_needed_t));
   parts->item_count=0;
   int count = invp->assembly_count;
   if(id==NULL){
        while(count!=0){
        assembly_t * assembly = invp->assembly_list;
	for(int i=1;i<count;i++){
	   assembly = assembly->next;
	}

        int amount = assembly->capacity;
	if(assembly!=NULL){
	   if(2*assembly->on_hand < amount){
	      printf(">>> restocking assembly %s with %d items\n",assembly->id,assembly->capacity-assembly->on_hand );
              make(invp, assembly->id, assembly->capacity-assembly->on_hand, parts,1);
	      assembly->on_hand = assembly->capacity;
	   }
	   }else{
		fprintf(stderr, " %s: assembly ID is not in inventory\n",id);
		break;
	   }
             count--;
	}     
              print_items_needed(parts);

   }else if(id!=NULL){
       assembly_t * assembly = lookup_assembly(invp->assembly_list,id);
       if(assembly==NULL){

          fprintf(stderr,"!!! %s: does not exist in assembly inventory\n",id );
	  return;
       }
        int amount = assembly->capacity;
	   if(2*assembly->on_hand < amount){
	      printf(">>> restocking assembly %s with %d items\n",assembly->id,assembly->capacity-assembly->on_hand );
              make(invp, assembly->id, assembly->capacity-assembly->on_hand, parts,1);
	      assembly->on_hand = assembly->capacity;
	   }
   
}
//free the item_list and then items_needed_t
free(parts);

free_items_needed(parts);
}

//This is the main loop of the program which handles all the requests of the program
//It also takes care of error checking and handles whitespaces
//@param (FILE *) fp: a file pointer from which the input is taken
//@param (inventory_t *) invp: a pointer to the main inventory which is allocated in
//                             main
//return: void
void read_input(FILE *fp, inventory_t * invp) {
    char buf[120]=" ";
    while(fgets(buf,120,fp)){
     char *token = strtok(trim(buf)," ");
     if(token==NULL){
        continue;
     }
     if(token[0]=='#'){
        continue;
     }
     if(strcmp(token ,"addPart")==0 ){
     char * second = strtok(NULL," ");
     printf("+ addPart %s\n",second);
     add_part(invp, second);			     
     }
     else if(strcmp(token,"addAssembly")==0) {
         char * id = strtok(NULL," ");
         char * capacity_i =  strtok(NULL, " ");
	 int capacity_c = strtol(capacity_i,NULL,10);
         items_needed_t * items_needed = malloc(sizeof(items_needed_t));
	 items_needed->item_list=NULL;
         printf("+ addAssembly %s %d",id,capacity_c);
	 //check if the assembly has items needed entered.
	 while(1){
	   char * first = strtok(NULL, " ");
	   char * second = strtok(NULL, " ");
	   if(first!=NULL && second!=NULL && first[0]!='#'){
	      printf(" %s %s",first,second); 
	   }
	   if(first!=NULL && strcmp(first,"#")!=0 ){
	      add_items_needed(invp, items_needed, first,strtol(second,NULL,10) );
	   }
	   else{
	      break;
	   }
	 }printf("\n");
	
         add_assembly(invp, id, capacity_c, items_needed );
     }else if(strcmp(token,"parts")==0){
        printf("+ parts\n");
        print_parts(invp);
     
     }else if(strcmp(token,"inventory")==0){
	printf("+ inventory");
        char * id = strtok(NULL," ");
        if(id==NULL || id[0]=='#' ){ 
	        printf("\n");	
	    	print_inventory(invp);	
        }
	else{
	    printf(" %s\n",id);
            assembly_t * assembly = lookup_assembly(invp->assembly_list,id);
            printf("Assembly ID:  %s\n",assembly->id);
            printf("bin capacity: %d\n",assembly->capacity);
	    printf("on hand:      %d\n",assembly->on_hand);
	    print_items_needed(assembly->items);
	}
     }else if(strcmp(token,"quit")==0){
        exit(EXIT_SUCCESS);
     
     }else if (strcmp(token,"empty")==0){
        char * id = strtok(NULL," ");
	printf("+ empty %s\n",id);
	empty(invp,id);
     }else if(strcmp(token,"help")==0){
         print_help();
     }else if(strcmp(token,"stock")==0 ){
        char * id = strtok(NULL," ");
	int quantity = strtol(strtok(NULL," "),NULL,10 );
	stock(invp,id,quantity);
	}else if(strcmp(token,"fulfillOrder")==0||strcmp(token,"fulfillorder")==0 ){
	   char * xi = strtok(NULL," ");
	   assembly_t* assembly = lookup_assembly(invp->assembly_list,xi);
	   int  ni = strtol(strtok(NULL," "),NULL,10);
	   fulfill_order(invp,assembly,xi,ni);

     }else if(strcmp(token,"restock")==0 ){
      char * id = strtok(NULL," ");
      if(id!=NULL){
         printf("+ restock %s\n",id);
	 restock (invp,id);
      }else{
         printf("+ restock\n");
      }
      if( id==NULL || strcmp(id,"#")==0){
	      restock(invp,NULL);    
      }
      else if( id[0]=='A' || id[0]=='a'){
     }else{
         fprintf(stderr,"%s: restock ID must start with \'A\'",id);
     }
     }
     else if(strcmp(token,"#")==0){
         while(1){
	  char * id = strtok(NULL," ");
	  if(id==NULL){
	     break;
	  }
	 }
     }else if(strcmp(token,"clear")==0){
	printf("+ clear\n");
        free_inventory(invp);
     }
     else{
       printf("+ %s\n",token);
       fprintf(stderr,"!!! %s: unknown command\n",token );
     }
   }
}

//The main function of the program where the inventory is initialized and read
//input is also called
//@param (int) argc: the argument count
//@param (char *) argv[]: an array of arguments
//@return: an integer specifying sucessfull execution of the program
int main(int argc, char * argv[]){
    inventory_t * inventory = calloc(1, sizeof(inventory_t));
    inventory->assembly_list=NULL;
    inventory->part_list=NULL;
    inventory->assembly_count=0;
    inventory->part_count=0;
    if(argc==1 ){
       FILE * fp = stdin;
       read_input(fp,inventory );
			  
    }else if(argc==2){
        FILE * fp = fopen(argv[1], "r" );
        read_input(fp, inventory);
    }

    free(inventory);
    return 1;
}
