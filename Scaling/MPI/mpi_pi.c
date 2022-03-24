// This program is to caculate PI using MPI
// The algorithm is based on integral representation of PI. If f(x)=4/(1+x^2), then PI is the intergral of f(x) from 0 to 1

#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <string.h>

#define N 1E9
#define d 1E-9
#define d2 1E-18
#define PI 3.14159265359

int main (int argc, char* argv[])
{
    int rank, size, error, scallingFactor=1;
    long int i;
    double pi=0.0, result=0.0, sum=0.0, begin=0.0, end=0.0, x2, persentError;
    char scalling[7];

    for(i = 0; i < argc; i++)
    {
        if(i==1)
	   strncpy(scalling,argv[i],7);
    } 
    error=MPI_Init (&argc, &argv);
    
    //Get process ID
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    
    //Get processes Number
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    
    if(strncmp(scalling,"Weak",5)==0)
       scallingFactor=size;

    //Synchronize all processes and get the begin time
    MPI_Barrier(MPI_COMM_WORLD);
    begin = MPI_Wtime();
    
    //Each process will caculate a part of the sum
    for (i=rank; i<N*scallingFactor; i+=size)
    {
        x2=d2*i*i/(scallingFactor*scallingFactor);
        result+=1.0/(1.0+x2);
    }
    
    //Sum up all results
    MPI_Reduce(&result, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    //Synchronize all processes and get the end time
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    
    //Caculate and print PI
    if (rank==0)
    {
        pi=4*d*sum/scallingFactor;
        persentError=(fabs(pi - PI)/PI)*100;
        printf("np=%2d;    Time=%fs;    PI=%0.11f;	Error=%0.11f\n", size, end-begin, pi, persentError);

    }
    
    error=MPI_Finalize();
    
    return 0;
}
