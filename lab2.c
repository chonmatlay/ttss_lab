#include "mpi.h" 
#include <stdio.h> 
#include <stdlib.h> 
#define N 100
#define FROM_MASTER 1
#define FROM_SLAVE 2
int a[N][N],b[N][N],c[N][N] ;
int aa[N][N],cc[N][N];
void createMatrix(); 
void printMatrix();
int main(int argc , char *argv[]){

	int numtasks ;
	int taskid ;
	long numworkers ;
	long source ; 
	long dest ; 
	long rows ; 
	long offset ;
	long divrow;
	long r ;
	
	int msgtype ;

	long i, j , k,rc ;
	MPI_Status status ;
	
	double start , end ;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	
	
	if (numtasks <2 ) {
		printf("Need at least two MPI tasks. Quitting...\n");
		MPI_Abort(MPI_COMM_WORLD,rc);
		exit(1) ;
	}
	numworkers= numtasks -1 ; 
	divrow = N/numworkers ;
		r = N%numworkers ; 

		if (r!= 0 ){
			printf("non divisible");
			 MPI_Abort(MPI_COMM_WORLD,rc);
			 exit(1);
			 }
/************ MAIN TASK *****************/
	if (taskid ==0 ) {
		//create matrix a and b
		
		for (i=0;i <N ; i++ ) 
		for ( j =0 ;j < N ;j++){ 
			aa[i][j]= 1;
			b[i][j] = 1 ;
		}
		
		//set start time 
		
		start = MPI_Wtime() ; 
		
		//...
		
		
		offset =0 ; 
		msgtype = FROM_MASTER ;
		
		
			
		
		}

	MPI_Scatter(&aa,divrow*N  ,MPI_DOUBLE, &a ,divrow*N , MPI_DOUBLE , 0 , MPI_COMM_WORLD );
	MPI_Bcast(&b,N*N, MPI_DOUBLE ,0 , MPI_COMM_WORLD );

	/************** OTHER TASK ****************/
	if (taskid > 0 ) {
		
		
		msgtype = FROM_MASTER ; 
		 
		
		int upperbound = divrow ;
		for (k=0 ; k<N ; k++ ) 
			for (i=0 ; i<upperbound ; i++ ) {
			c[i][k] = 0 ;
			for  (j=0 ; j < N ; j++)
				c[i][k] +=a[i][j]*b[j][k] ;
			}
			
			//send back
			
		msgtype = FROM_SLAVE ; 
		//MPI_Send(&offset ,1 ,MPI_INT , 0 , msgtype , MPI_COMM_WORLD); 
		//MPI_Send(&rows , 1 , MPI_INT , 0 , msgtype , MPI_COMM_WORLD ) ;

	}
	MPI_Gather(&cc, divrow * N , MPI_DOUBLE , &c , divrow*N,MPI_DOUBLE , 0 , MPI_COMM_WORLD);
	if (taskid ==0 ){
		

	
	printf("\n");
	
		for (i=0 ; i <N ; i++){
		for (j=0 ; j < N ; j++)
			printf("%f ",cc[i][j]);
		printf("\n"); 
	}
	printf("\n");

		
		}
	MPI_Finalize() ;


}