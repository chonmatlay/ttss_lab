
#include "mpi.h" 
#include <stdio.h> 
#include <stdlib.h> 
#define N 1000
#define FROM_MASTER 1
#define FROM_SLAVE 2

void createMatrix(); 
void printMatrix();
int main(int argc , char *argv[]){
	double a[N][N],b[N][N],c[N][N] ;
	int numtasks ;
	int taskid ;
	int numworkers ;
	int source ; 
	int dest ; 
	int rows ; 
	int offset ;
	int divrow;
	int r ;
	
	int msgtype ;

	int i, j , k,rc ;
	MPI_Status status ;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	//printf("start  workers");
	
	if (numtasks <2 ) {
		printf("Need at least two MPI tasks. Quitting...\n");
		MPI_Abort(MPI_COMM_WORLD,rc);
		exit(1) ;
	}
	numworkers= numtasks -1 ; 
	
/************ MAIN TASK *****************/
	if (taskid ==0 ) {
		//create matrix a and b
		
		for (i=0;i <N ; i++ ) 
		for ( j =0 ;j < N ;j++){ 
			a[i][j]= 1;
			b[i][j] =1 ;
		}
		
		//set start time 
		
		double start = MPI_Wtime() ; 
		
		//...
		
		divrow = N/numworkers ;
		r = N%numworkers ; 
		offset =0 ; 
		msgtype = FROM_MASTER ;
		
		// send to slave
		
		for (dest =1 ; dest <= numworkers ; dest++)
		{
			rows = dest <= r ? divrow +1 :divrow ; 
			MPI_Send(&offset , 1 , MPI_INT , dest , msgtype , MPI_COMM_WORLD);
			MPI_Send(&rows , 1 , MPI_INT , dest , msgtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0] , rows*N , MPI_DOUBLE , dest , msgtype, MPI_COMM_WORLD);
			MPI_Send(&b , N*N , MPI_DOUBLE , dest , msgtype, MPI_COMM_WORLD);
			offset += rows ; 
		}
		
		msgtype = FROM_SLAVE ;
		
		//receive
		
		for (i = 1 ; i <= numworkers ; i ++)
		{
		source = i ;
		MPI_Recv(&offset , 1 , MPI_INT , source , msgtype ,MPI_COMM_WORLD, &status); 
		MPI_Recv(&rows , 1 , MPI_INT , source , msgtype , MPI_COMM_WORLD , &status) ;
		MPI_Recv(&c[offset][0] , rows*N , MPI_DOUBLE, source ,msgtype,MPI_COMM_WORLD, &status ) ;
		}
		double 	finish=MPI_Wtime() ;
		printf("total time : %f \n", finish-start);
		/*
		 * print the matrix here
		 * 
*/
		for (i=0 ; i <N ; i++){
		for (j=0 ; j < N ; j++)
			printf("%f ",a[i][j]);
		printf("\n"); 
	}
	printf("\n");
		for (i=0 ; i <N ; i++){
		for (j=0 ; j < N ; j++)
			printf("%f ",b[i][j]);
		printf("\n"); 
	}
	printf("\n");
	
		for (i=0 ; i <N ; i++){
		for (j=0 ; j < N ; j++)
			printf("%f ",c[i][j]);
		printf("\n"); 
	}
	printf("\n");
/*	*/
	}
	/************** OTHER TASK ****************/
	if (taskid > 0 ) {
		//receive from master
		
		msgtype = FROM_MASTER ; 
		MPI_Recv(&offset , 1 , MPI_INT , 0 , msgtype, MPI_COMM_WORLD , &status) ; 
		MPI_Recv(&rows , 1 , MPI_INT ,0, msgtype , MPI_COMM_WORLD, &status ) ; 
		MPI_Recv(&a, rows*N , MPI_DOUBLE , 0 , msgtype , MPI_COMM_WORLD ,&status); 
		MPI_Recv(&b , N*N , MPI_DOUBLE , 0 , msgtype , MPI_COMM_WORLD,&status ); 
		
		 // calculate the matrix
		
		for (k=0 ; k<N ; k++ ) 
			for (i=0 ; i<rows ; i++ ) {
			c[i][k] = 0 ;
			for  (j=0 ; j < N ; j++)
				c[i][k] +=a[i][j]*b[j][k] ;
			}
			
			//send back
			
		msgtype = FROM_SLAVE ; 
		MPI_Send(&offset ,1 ,MPI_INT , 0 , msgtype , MPI_COMM_WORLD); 
		MPI_Send(&rows , 1 , MPI_INT , 0 , msgtype , MPI_COMM_WORLD ) ;
		MPI_Send(&c , rows*N , MPI_DOUBLE ,0 ,msgtype , MPI_COMM_WORLD);
	}

	
	MPI_Finalize() ;


}

