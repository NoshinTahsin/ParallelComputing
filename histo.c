#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

#define BUFFERSIZE 40

int getMax(int *buffer)
{
	int max=0;
	for(int i=0;i<BUFFERSIZE;i++)
	{
		if(buffer[i]>max)
			max=buffer[i];
	}
	printf("%d\n", max);
	return max;
}

int getMin(int *buffer)
{
	int min=1434864;
	for(int i=0;i<BUFFERSIZE;i++)
	{
		if(buffer[i]<min)
			min=buffer[i];
	}
	printf("%d\n", min);
	return min;
}

int main(int argc, char **argv)
{

	int i, cnt=0;
	int size, rank;

	int buffer[BUFFERSIZE]={1,9,25,71,5,6,41,87,23,48,96,32,52,63,91,73,18,16,64,
	52,37,12,39,82,45,15,24,93,65,11,74,32,98,10,30,71,37,3,15,12};

	int bins=5,max,min;

	max=getMax(buffer);
	min=getMin(buffer);

	int bin_size= ((max-min)/bins)+1;
	printf("bin size %d\n",bin_size);

	int init;
	init= MPI_Init(&argc, &argv);
	MPI_Status status;
	init= MPI_Comm_size(MPI_COMM_WORLD, &size);
	init=MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int r_process=0;
	int totalArray[size];
	int start,n_rows, end,send_values;
	if(rank==r_process)
	{
		int values_per_process=BUFFERSIZE/size;

		for(int j=1;j<size;j++)
		{
			start=j*values_per_process+1;
			printf("start %d\n", start);
			end=(j+1)*values_per_process;
			printf("end %d\n", end);

			if((BUFFERSIZE-end)<values_per_process)
				end=BUFFERSIZE-1;

			send_values=end-start+1;
			printf("send %d\n", send_values);
			init=MPI_Send( &send_values, 1, MPI_INT, j, 2001, MPI_COMM_WORLD);

			init=MPI_Send(&buffer[start], send_values, MPI_INT, j, 2001, MPI_COMM_WORLD);
		}

		for(int p=0;p<size;p++)
                        {
                                    totalArray[p]=0;
                        }

		for(int j=1;j<size;j++)
		{
			int count[bins];
			init=MPI_Recv(&count, bins, MPI_INT, MPI_ANY_SOURCE, 2002, MPI_COMM_WORLD, &status);

			int sender=status.MPI_SOURCE;

			printf("Partially counted array returned from process %i\n", sender);
			for(int par=0;par<size;par++)
                                    {
                                                printf("%d\n", count[par]);
                                    }

			for(int s=0;s<size;s++)
                                    {
                                                totalArray[s]+=count[s];
                                    }
		}
	}

	else
	{
		printf(" rank %d", rank);
		int receive_values;
		init=MPI_Recv( &receive_values, 1, MPI_INT, r_process, 2001, MPI_COMM_WORLD, &status);

		int received_values=receive_values;
		int array2[BUFFERSIZE-received_values];
		printf("received %d\n",received_values);

		init=MPI_Recv( &array2, receive_values, MPI_INT, r_process, 2001, MPI_COMM_WORLD, &status);

		for(int i=0;i<received_values;i++)
		{
			printf(" arr %d", array2[i]);
		}
		printf("\n");

		int partial=0;
		int count[bins];

		int lowerRange=min;
                        int upperRange=lowerRange+bin_size;

                        for(int binIt=0;binIt<bins;binIt++)
                        {
                                    for(i=0;i<received_values;i++)
                                    {
                                                if(array2[i]>=lowerRange && array2[i]<upperRange)
                                                {
                                                            count[binIt]++;
                                                }
                                    }
                                    lowerRange=upperRange;
                                    upperRange=lowerRange+bin_size;
                        }

                        for(int i=0;i<bins;i++)
		{
			printf("%d ",count[i]);
		}

                        init=MPI_Send( &count[0], bins, MPI_INT, r_process, 2002, MPI_COMM_WORLD);
	}

	if(rank==r_process)
            {
                        //printing result
                        int lowerRange=min;
                        int upperRange=lowerRange+bin_size;

                        for(int i=0,i<size;i++)
                        {
                                    printf("Range %d ---- %d \n",lowerRange,upperRange);
                                    printf("Height %d\n\n",totalArray[i]);

                                    lowerRange=upperRange;
                                    upperRange=lowerRange+bin_size;
                        }
            }


	MPI_Finalize();
}
