#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

#define BUFFERSIZE 6


int main(int argc, char **argv)
{

    int i, cnt=0;
    int size, rank;

    int buffer[BUFFERSIZE]={3,4,5,6,7,8};

    int bins=3 ;

     

    int bin_size= 2;
    printf("bin size %d\n",bin_size);

    int init;
    init= MPI_Init(&argc, &argv);
    MPI_Status status;
    init= MPI_Comm_size(MPI_COMM_WORLD, &size);
    init=MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int r_process=0;
    //int totalArray[size];
    int partialSumArr[size];
    int start,n_rows, end,send_values;
    if(rank==r_process)
    {
        int values_per_process= 2;

        for(int j=1;j<size;j++)
        {
            start=j*2-2;
           
            //printf("start %d\n", start);
            end=(j+*2+1) -2;
            //printf("end %d\n", end);

            if((BUFFERSIZE-end)<values_per_process)
                end=BUFFERSIZE-1;

            send_values=end-start+1;
            //printf("send %d\n", send_values);
            init=MPI_Send( &send_values, 1, MPI_INT, j, 2001, MPI_COMM_WORLD);

            init=MPI_Send(&buffer[start], send_values, MPI_INT, j, 2001, MPI_COMM_WORLD);
        }
       
  
       
        

        for(int j=1;j<size;j++)
        {
            int part_sum=0;

            init=MPI_Recv(&part_sum, 1, MPI_INT, MPI_ANY_SOURCE, 2002, MPI_COMM_WORLD, &status);

            int sender=status.MPI_SOURCE;

            printf("Partial sum returned from process %i\n", sender);

            printf("%d",part_sum);

            printf("\n");

            partialSumArr[j]=part_sum;

        }


        int totalSum=0;

        for(int t=0;t<size;t++)
            totalSum+=partialSumArr[t];

        printf("Total Sum is: \n");
        printf("%d\n",totalSum );
   
    }

    else
    {
        //printf(" rank %d", rank);
        int partial_sum=0;
        int receive_values;
        init=MPI_Recv( &receive_values, 1, MPI_INT, r_process, 2001, MPI_COMM_WORLD, &status);

        int received_values=receive_values;

        int array2[received_values];
        //printf("received %d\n",received_values);

        init=MPI_Recv( &array2, receive_values, MPI_INT, r_process, 2001, MPI_COMM_WORLD, &status);

        for(int s=0;s<received_values;s++)
        {
            partial_sum+=array2[s];
        }
   

        init=MPI_Send( &partial_sum, 1, MPI_INT, r_process, 2002, MPI_COMM_WORLD);
    }

   

    MPI_Finalize();
}

