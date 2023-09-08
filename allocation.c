#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define total_memory (1024 * 1024)
int ArrayIndex = 0;
int count = 0;
int releases = 0;
int LastElement = 0;

typedef struct allocator{
    char processid[4];
    int base;
    int limit;
    int size;
    int is_allocated;
    int position; //Use
}allocator;

typedef struct holes{
    int base;
    int limit;
    int size;
    int position;
} holes;


allocator process_array[30] = {0};
allocator temp_arr[30] = {0};

//first fit algo
void first_fit(char *memory, allocator *process, int request_size, char processid[4]){
        int x = 0;
        while(x == 0){
            if(count == 0){
                printf("\nGoing through If");
                memset(memory,'1',request_size);
                strcpy(process->processid,processid);
                process->base = 0;
                process->limit = request_size - 1;
                process->size = request_size;
                process->is_allocated = 1;
                printf("\nArrayIndex:%d", ArrayIndex);
                printf("\nbase: %d  limit: %d  %s", process->base, process->limit, process->processid);
                x++;
            }
            else{
                ArrayIndex = 0;
                while(ArrayIndex <= count+1){
                    printf("\n");
                    printf("\nArrayIndex:%d   Count%d", ArrayIndex, count);
                    if((process + ArrayIndex)->is_allocated == 0 && ArrayIndex == 0 && request_size <= (process + ArrayIndex)->size){
                        printf("\nGoing through Else 1");
                        memset(memory,'1',request_size);
                        strcpy(process->processid,processid);
                        process->base = 0;
                        process->limit = request_size - 1;
                        process->size = request_size;
                        process->is_allocated = 1;
                        printf("\nArrayIndex:%d", ArrayIndex);
                        printf("\nbase: %d  limit: %d  %s", (process+ArrayIndex)->base, (process+ArrayIndex)->limit, (process+ArrayIndex)->processid);
                        ArrayIndex = count + 1;
                    }
                    else if((process + ArrayIndex)->is_allocated == 0 && request_size <= (process + ArrayIndex)->size){
                        printf("\nGoing through Else 2");
                        memset(memory + (process+(ArrayIndex-1))->size, "1", request_size);
                        strcpy((process+ArrayIndex)->processid,processid);
                        (process+ArrayIndex)->base = ((process+ArrayIndex)-1)->limit+1;
                        (process+ArrayIndex)->size = request_size;
                        (process+ArrayIndex)->limit = (process+ArrayIndex)->base + (process+ArrayIndex)->size-1;
                        (process+ArrayIndex)->is_allocated = 1;
                        printf("\nArrayIndex:%d", ArrayIndex);
                        printf("\nPrevious Element base: %d  limit: %d  %s", (process+ArrayIndex - 1)->base, (process+ArrayIndex - 1)->limit, (process+ArrayIndex - 1)->processid);
                        printf("\nNext Element base: %d  limit: %d  %s", (process+ArrayIndex + 1)->base, (process+ArrayIndex + 1)->limit, (process+ArrayIndex + 1)->processid);
                        printf("\nbase: %d  limit: %d  %s", (process+ArrayIndex)->base, (process+ArrayIndex)->limit, (process+ArrayIndex)->processid);
                        ArrayIndex = count + 1;
                    }
                    else if(( ( (process + ArrayIndex)->limit+1) != (process + (ArrayIndex + 1))->base) && request_size <= ((process + (ArrayIndex+1))->base - (process + ArrayIndex)->limit)   &&  ((process + (ArrayIndex + 1))->base != 0) ){
                        printf("\nGoing through Else 3 (Shift)");
                        shift_right(ArrayIndex);
                        ArrayIndex++;
                        strcpy((process_array+ArrayIndex)->processid, processid);
                        (process_array+ArrayIndex)->base = ((process_array+ArrayIndex)-1)->limit+1;
                        (process_array+ArrayIndex)->size = request_size;
                        (process_array+ArrayIndex)->limit = ((process_array+ArrayIndex))->base + request_size - 1;
                        (process_array+ArrayIndex)->is_allocated = 1;
                        ArrayIndex = count + 1;
                    }
                    else if((process + ArrayIndex)->is_allocated == 0 && (process+ArrayIndex)->size == 0){   
                        printf("\nGoing through Else 4");                 
                        memset(memory + (process+(ArrayIndex-1))->size, "1", request_size);
                        strcpy((process+ArrayIndex)->processid,processid);
                        (process+ArrayIndex)->base = ((process+ArrayIndex)-1)->limit+1;
                        (process+ArrayIndex)->size = request_size;
                        (process+ArrayIndex)->limit = (process+ArrayIndex)->base + (process+ArrayIndex)->size-1;
                        (process+ArrayIndex)->is_allocated = 1;
                        ArrayIndex = count + 1;
                    }
                printf("\nID:%s   BASE:%d   LIMIT:%d    SIZE:%d    IS ALLOCATED:%d", (process_array+ArrayIndex)->processid,(process_array+ArrayIndex)->base,(process_array+ArrayIndex)->limit,(process_array+ArrayIndex)->size,(process_array+ArrayIndex)->is_allocated);
                ArrayIndex++;
                }
            }
        x++;
        }
    }

//release memory
void release(allocator *process, char processid[2], char *memory){
   int x = 0;
   int i = 0;
    printf("Releasing memory from %s", processid);
    while(x == 0) {
        if(strcmp((process + i)->processid, processid) == 0){
            memset(memory + (process + i)->base, "0", (process + i)->size);
            (process + i)->is_allocated = 0;
            printf("\nSuccessfully Released %s", (process + i)->processid);
            x++;
        }
    i++;
    }
    printf("\n");
}

//shift_right
void shift_right(int index){
    int i = (LastElement+1);
    while(i != (index)){
        process_array[i] = process_array[i - 1];
        i--;
    }
}

//Shift_left
void shift_left(int index, int hole_size, char *memory){
    int temp = 0;
    int temp_index = 0;
    while(index <= LastElement){
        printf("\nIndex: %d", index);
        printf("\n     ID:%s   BASE:%d   LIMIT:%d    SIZE:%d    IS ALLOCATED:%d", (process_array+index)->processid, (process_array+index)->base,(process_array+index)->limit,(process_array+index)->size,(process_array+index)->is_allocated);
        printf("\nNEXT ID:%s   BASE:%d   LIMIT:%d    SIZE:%d    IS ALLOCATED:%d", (process_array+index+1)->processid, (process_array+index+1)->base,(process_array+index+1)->limit,(process_array+index+1)->size,(process_array+index+1)->is_allocated);
        if(index == 0){
            printf("\nGoing through index == 0");
            (process_array+index)->base = 0;
            strcpy((process_array+index)->processid, (process_array+(index+1))->processid);
            (process_array+index)->size = (process_array+(index+1))->size;
            (process_array+index)->limit = (process_array+(index))->size - 1;
            (process_array+index)->is_allocated = 1;
        }
        else if(index != 0){
            printf("\nGoing through index != 0");
            if((process_array+index+1)->is_allocated == 0 && index != (LastElement) && index != temp){
                printf("\nGoing through additional if");
                (process_array+index)->is_allocated = 0;
            }
            else{
            printf("\nGoing through additional else");
            (process_array+index)->is_allocated = 1;
            }
            (process_array+index)->base = (process_array+(index-1))->limit+1;
            strcpy((process_array+index)->processid, (process_array+(index+1))->processid);
            (process_array+index)->size = (process_array+(index+1))->size;
            (process_array+index)->limit = (process_array+index)->base + (process_array+(index+1))->size - 1;
        }
        if(index == LastElement){
            printf("\nGoing through index Last ELement");
            strcpy((process_array+LastElement)->processid,"");
            memset(memory,'0', (process_array+LastElement)->size);
            (process_array+LastElement)->base = 0;
            (process_array+LastElement)->limit = 0;
            (process_array+LastElement)->size = 0;
            (process_array+LastElement)->is_allocated = 0;
            }
        // printf("\nPrevious Element base: %d  limit: %d  %s", (process+ArrayIndex - 1)->base, (process+ArrayIndex - 1)->limit, (process+ArrayIndex - 1)->processid);
        // printf("\nNext Element base: %d  limit: %d  %s", (process+ArrayIndex + 1)->base, (process+ArrayIndex + 1)->limit, (process+ArrayIndex + 1)->processid);        
        index++;
    }
    printf("\nLast Element:%d", LastElement);
}

//compress
void compress(allocator *process, char * memory){
    int i = 0;
    int iter = 0;
    int x=0;
    while(i != count){
        if((process+i)->is_allocated == 0){
            printf("\nComperss Iterarion: %d", iter + 1);
            shift_left(i, (process+i)->size, memory);
            last();
        }
        else if(((process+i)->is_allocated == 1 && ((process+i)->limit + 1) != (process + i + 1)->base && (process + i + 1)->base != 0)){
            printf("\nGoing through Small hole");
            while (x != count){
            (process_array+i+1)->base = (process_array+(i))->limit+1;
            //strcpy((process_array+i)->processid, (process_array+(i+1))->processid);
            //(process_array+i+1)->size = (process_array+(i+1))->size;
            (process_array+i+1)->limit = (process_array+i+1)->base + (process_array+(i+1))->size - 1;
            x++;
            }
        }
        i++;
        printf("\nCompressor i:%d", i);

    }
}

//last
void last(){
    int x = 0;
    int i = 0;
    while(x == 0){
        if( ( (process_array+i)->base == 0) && ((process_array+i)->limit == 0) && ((process_array+i)->size == 0)  && ((process_array+i)->size == 0) && ((process_array+i)->is_allocated == 0)){
            LastElement = i - 1;
            x++;
        }
        i++;
    }
}

//status
void status(allocator *process, int count){
    printf("\n");
    int total_allocation = 0;
    int i = 0;
    while(i <= LastElement){
        if((process+i)->is_allocated == 1){          
            total_allocation = total_allocation + (process+i)->size;
        }
        i++;
    }
    printf("Partitions [Allocated Memory = %d]",total_allocation);
    i = 0;
    while(i<=LastElement){
        if((process+i)->is_allocated == 1){  
            printf("\nAddress [%d:%d] Process %s",(process+i)->base, (process+i)->limit, (process+i)->processid);
        }
        i++;
    }
    printf("\n");
    printf("\nHoles [Free Memory = %d]", total_memory - total_allocation);
    i = 0;
    while(i <= LastElement){
        if((process+i)->is_allocated == 0 || ((process+i)->limit+1) != (process + i+1)->base){
            if((process+i)->is_allocated == 0 && (i != LastElement)){
                printf("\nif is_allocated == 0");
                printf("\nAddress [%d:%d] Length:%d [Process: %s]",(process+i)->base, (process+(i+1))->base-1, (process+i)->size, (process+i)->processid);
            }
            else if((process+i)->is_allocated == 1 && ((process+i)->limit + 1) != (process + i + 1)->base && (process + i + 1)->base != 0){
                printf("\nif is_allocated = 1 and limit + 1 !=  next base  and next base != 0");
                printf("\nAddress [%d:%d] Length:%d [Process: %s]",(process+i)->limit+1, (process+(i+1))->base-1, (process+(i+1))->base-1 - (process+i)->limit+1, (process+i)->processid);  
            }
        }
            if(i == LastElement){
                if((process+LastElement)->is_allocated == 0){
                    printf("\nAddress [%d:%d] Length:%d [Last Process: %s]", (process+LastElement)->base, total_memory, total_memory - (process+LastElement)->base, (process+LastElement)->processid);
                }
                else{               
                printf("\nAddress [%d:%d] Length:%d [Last Process: %s]", (process+LastElement)->limit + 1, total_memory, total_memory - (process+LastElement)->limit, (process+LastElement)->processid);
                printf("\ni:%d", i);
                }
        }
    i++;
    }
    printf("\n");
}



int main(){

char *memory = (char*)malloc(total_memory);

char input[15];
char command[2];
char processid[2];
int request_size;
char algo[2];

int x = 0;
    while(x == 0){   
        printf("\nHere, I am using First Fit approach and have allocated 1048576 bytes of memory");
        printf("\nAllocator>");

        scanf("%s", input);
         if(strcmp(input,"EXIT") == 0){
            break;
         }

         else if(strcmp(input,"RQ") == 0){
            scanf("%s %d %s", processid, &request_size, &algo);
            first_fit(memory, process_array, request_size, processid);
            count++;
            last();
        }
         else if(strcmp(input,"RL") == 0){
            scanf("%2s", processid);
            release(process_array, processid, memory);
            last();
            releases++;
        }
        else if (strcmp(input,"help")==0){
            int y = 0;
            while(y <= count){
                printf("\n");
                printf("\nY=%d Count=%d",y,count);
                printf("\n");
                printf("\nID:%s   BASE:%d   LIMIT:%d    SIZE:%d    IS ALLOCATED:%d", (process_array+y)->processid,(process_array+y)->base,(process_array+y)->limit,(process_array+y)->size,(process_array+y)->is_allocated);
                y++;  
            }
            printf("\nLast Element Num :%d Last Id:%s", LastElement, (process_array+LastElement)->processid);
        }
        else if(strcmp(input, "C") == 0){
            compress(process_array, memory);
        }
        else if(strcmp(input, "Status") == 0){
            status(process_array, count);
        }
    }
}

// RQ P0 2000 F
// RQ P1 3000 F
// RQ P2 4000 F
// RQ P3 5000 F
// RQ P4 6000 F