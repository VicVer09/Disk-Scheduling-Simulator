#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 53

//compare function for qsort
//you might have to sort the request array
//use the qsort function 
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int abs (int a) { if (a > 0) return a; else return -a; }
void MergeSort(int *A,int n);
void Merge(int *A,int *L,int leftCount,int *R,int rightCount);


//function to swap 2 integers
void swap(int *a, int *b)
{
    if (*a != *b)
    {
        *a = (*a ^ *b);
        *b = (*a ^ *b);
        *a = (*a ^ *b);
        return;
    }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
    int i, last, acc = 0;
    last = START;
    printf("\n");
    printf("%d", START);
    for (i = 0; i < numRequest; i++)
    {
        printf(" -> %d", request[i]);
        acc += abs(last - request[i]);
        last = request[i];
    }
    printf("\nPerformance : %d\n", acc);
    return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
    //simplest part of assignment
    printf("\n----------------\n");
    printf("FCFS :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{
    
    int i, j;
    int* distance = malloc(numRequest * sizeof(int));

    // get distance array
    for (i = 0; i < numRequest; i++) distance[i] = abs(START - request[i]);

    // swap out of order elements
    for (i = 0; i < numRequest; i++) for (j = i + 1; j < numRequest; j++){
        if (distance[i] > distance[j]) {
                swap (&distance[i], &distance[j]);
                swap (&request[i], &request[j]);
        }
    }
    free(distance);
    
    printf("\n----------------\n");
    printf("SSTF :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{
    
    MergeSort(request,numRequest);
    int* newRequest;
    int i,j,newCnt;
    int* distance = malloc(numRequest * sizeof(int));

    // get distance array
    for (i = 0; i < numRequest; i++) distance[i] = abs(START - request[i]);

    for (i = 0; i < numRequest; i++) {
        
        if ((i > 0 && distance[i-1]<=distance[i] && request[i] > START ) || (i == numRequest - 1 && request[i] < START)) {
            // Scan left

            //printf("\ngo left first starting from %i\n", i);

            if (i == numRequest - 1) { // scan only left

                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < numRequest; j++) newRequest[j] = request[i - j];

            } else { // scan left then right
                
                //printf("\nreached\n");
                newCnt = numRequest + 1;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < i; j++) newRequest[j] = request[i - j - 1];
                newRequest[i] = 0;
                for (j = i + 1; j < newCnt; j++) newRequest[j] = request[j-1];
                
            }

            break;

        } else if ((i == 0 && request[i] > START) || (i > 0 && distance[i-1]>distance[i] && request[i] > START) ) { 
            // Case where we go right first
            //printf("\ngo right first starting from %i\n", i);
            if (i == 0) {

                newCnt = numRequest;
                newRequest = request; // return sorted array

            } else {

                //printf("\nreached %i\n", i);
                newCnt = numRequest + 1;
                newRequest = malloc(newCnt * sizeof(int));
                int pivot = numRequest - i;
                for (j = 0; j < pivot; j++) newRequest[j] = request[i + j];
                newRequest[pivot] = 199;
                //for (j = i; j < newCnt; j++) newRequest[j] = request[numRequest - j];
                for (j = pivot + 1; j < newCnt; j++) newRequest[j] = request[numRequest - j];

            }
            break;
        }
    }
    


    printf("\n----------------\n");
    printf("SCAN :");
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{


    MergeSort(request,numRequest);
    int* newRequest;
    int i,j,newCnt;
    int* distance = malloc(numRequest * sizeof(int));

    // get distance array
    for (i = 0; i < numRequest; i++) distance[i] = abs(START - request[i]);

    for (i = 0; i < numRequest; i++) {
        
        if ((i > 0 && distance[i-1]<=distance[i] && request[i] > START ) || (i == numRequest - 1 && request[i] < START)) {
            // Scan left

            //printf("\ngo left first starting from %i\n", i);

            if (i == numRequest - 1) { // scan only left

                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < numRequest; j++) newRequest[j] = request[i - j];

            } else { // scan left then quickly go back to 199 then back left
                
                //printf("\nreached\n");
                newCnt = numRequest + 2;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < i; j++) newRequest[j] = request[i - j - 1];
                newRequest[i] = 0;
                newRequest[i+1] = 199;
                for (j = i + 2; j < newCnt; j++) newRequest[j] = request[numRequest - (j - i - 1)];
                
            }

            break;

        } else if ((i == 0 && request[i] > START) || (i > 0 && distance[i-1]>distance[i] && request[i] > START) ) { 
            // Case where we go right first
            //printf("\ngo right first starting from %i\n", i);
            if (i == 0) {

                newCnt = numRequest;
                newRequest = request; // return sorted array

            } else {

                //printf("\nreached %i\n", i);
                newCnt = numRequest + 2;
                newRequest = malloc(newCnt * sizeof(int));
                int pivot = numRequest - i;
                for (j = 0; j < pivot; j++) newRequest[j] = request[i + j];
                newRequest[pivot] = 199;
                newRequest[pivot + 1] = 0;
                for (j = pivot + 2; j < newCnt; j++) newRequest[j] = request[j - 1 - i];

            }
            break;
        }
    }
    



    printf("\n----------------\n");
    printf("CSCAN :");
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
    
    MergeSort(request,numRequest);
    int* newRequest;
    int i,j,newCnt;
    int* distance = malloc(numRequest * sizeof(int));

    // get distance array
    for (i = 0; i < numRequest; i++) distance[i] = abs(START - request[i]);

    for (i = 0; i < numRequest; i++) {
        
        if ((i > 0 && distance[i-1]<=distance[i] && request[i] > START ) || (i == numRequest - 1 && request[i] < START)) {
            // Scan left

            //printf("\ngo left first starting from %i\n", i);

            if (i == numRequest - 1) { // scan only left

                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < numRequest; j++) newRequest[j] = request[i - j];

            } else { // scan left then right
                
                //printf("\nreached %i\n", i);
                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < i; j++) newRequest[j] = request[i - j - 1];
                for (j = i; j < newCnt; j++) newRequest[j] = request[j];
                
            }

            break;

        } else if ((i == 0 && request[i] > START) || (i > 0 && distance[i-1]>distance[i] && request[i] > START) ) { 
            // Case where we go right first
            //printf("\ngo right first starting from %i\n", i);
            if (i == 0) {

                newCnt = numRequest;
                newRequest = request; // return sorted array

            } else {

                //printf("\nreached %i\n", i);
                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                int pivot = numRequest - i;
                for (j = 0; j < pivot; j++) newRequest[j] = request[i + j];
                for (j = pivot; j < newCnt; j++) newRequest[j] = request[numRequest - j - 1];
                    
            }
            break;
        }
    }

    printf("\n----------------\n");
    printf("LOOK :");
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{


    MergeSort(request,numRequest);
    int* newRequest;
    int i,j,newCnt;
    int* distance = malloc(numRequest * sizeof(int));

    // get distance array
    for (i = 0; i < numRequest; i++) distance[i] = abs(START - request[i]);

    for (i = 0; i < numRequest; i++) {
        
        if ((i > 0 && distance[i-1]<=distance[i] && request[i] > START ) || (i == numRequest - 1 && request[i] < START)) {
            // Scan left

            //printf("\ngo left first starting from %i\n", i);
            if (i == numRequest - 1) { // scan only left

                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < numRequest; j++) newRequest[j] = request[i - j];

            } else { // scan left then quickly go back to rightmost entry then back left
                
                printf("\nreached\n");
                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                for (j = 0; j < i; j++) {
                    newRequest[j] = request[i - j - 1];
                    //printf("newRequest[%i] = %i\n",j,newRequest[j]);
                }
                for (j = i; j < newCnt; j++) {
                    newRequest[j] = request[numRequest - (j - i + 1)];
                    //printf("newRequest[%i] = %i\n",j,newRequest[j]);
                }
            }

            break;

        } else if ((i == 0 && request[i] > START) || (i > 0 && distance[i-1]>distance[i] && request[i] > START) ) { 
            // Case where we go right first
            //printf("\ngo right first starting from %i\n", i);
            if (i == 0) {

                newCnt = numRequest;
                newRequest = request; // return sorted array

            } else {

                //printf("\nreached %i\n", i);
                newCnt = numRequest;
                newRequest = malloc(newCnt * sizeof(int));
                int pivot = numRequest - i;
                for (j = 0; j < pivot; j++) newRequest[j] = request[i + j];
                for (j = pivot; j < newCnt; j++) newRequest[j] = request[j + 1 - i];

            }
            break;
        }
    }
     

    //write your logic here
    printf("\n----------------\n");
    printf("CLOOK :");
    printSeqNPerformance(newRequest,newCnt);
    printf("----------------\n");
    return;
}

void Merge(int *A,int *L,int leftCount,int *R,int rightCount) {
    int i,j,k;

    // i - to mark the index of left aubarray (L)
    // j - to mark the index of right sub-raay (R)
    // k - to mark the index of merged subarray (A)
    i = 0; j = 0; k =0;

    while(i<leftCount && j< rightCount) {
        if(L[i]  < R[j]) A[k++] = L[i++];
        else A[k++] = R[j++];
    }
    while(i < leftCount) A[k++] = L[i++];
    while(j < rightCount) A[k++] = R[j++];
}

// Recursive function to sort an array of integers. 
void MergeSort(int *A,int n) {
    int mid,i, *L, *R;
    if(n < 2) return; // base condition. If the array has less than two element, do nothing. 

    mid = n/2;  // find the mid index. 

    // create left and right subarrays
    // mid elements (from index 0 till mid-1) should be part of left sub-array 
    // and (n-mid) elements (from mid to n-1) will be part of right sub-array
    L = (int*)malloc(mid*sizeof(int)); 
    R = (int*)malloc((n- mid)*sizeof(int)); 
    
    for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
    for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray

    MergeSort(L,mid);  // sorting the left subarray
    MergeSort(R,n-mid);  // sorting the right subarray
    Merge(A,L,mid,R,n-mid);  // Merging L and R into A as sorted list.
        free(L);
        free(R);
}

int main()
{
    int *request, numRequest, i,ans;

    //allocate memory to store requests
    printf("Enter the number of disk access requests : ");
    scanf("%d", &numRequest);
    request = malloc(numRequest * sizeof(int));

    printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    printf("\nSelect the policy : \n");
    printf("----------------\n");
    printf("1\t FCFS\n");
    printf("2\t SSTF\n");
    printf("3\t SCAN\n");
    printf("4\t CSCAN\n");
    printf("5\t LOOK\n");
    printf("6\t CLOOK\n");
    printf("----------------\n");
    scanf("%d",&ans);

    switch (ans)
    {
    //access the disk location in FCFS
    case 1: accessFCFS(request, numRequest);
        break;

    //access the disk location in SSTF
    case 2: accessSSTF(request, numRequest);
        break;

        //access the disk location in SCAN
     case 3: accessSCAN(request, numRequest);
        break;

        //access the disk location in CSCAN
    case 4: accessCSCAN(request,numRequest);
        break;

    //access the disk location in LOOK
    case 5: accessLOOK(request,numRequest);
        break;

    //access the disk location in CLOOK
    case 6: accessCLOOK(request,numRequest);
        break;

    default:
        break;
    }
    return 0;
}