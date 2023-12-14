#include "heterocc.h"
#include <stdio.h>
#include <stdlib.h>


#define LEFT_DIM 20
#define RIGHT_DIM 30
#define COMMON_DIM 10

#ifndef DEVICE
#define DEVICE hetero::CPU_TARGET
#endif

void matmul(int* Res, std::size_t Res_Size,
                int* V1, std::size_t V1_Size, 
                int* V2, std::size_t V2_Size,
                std::size_t left_dim, std::size_t right_dim,
                std::size_t common_dim){

    void* Section =  __hetero_section_begin();

    void* wrapper = 
            __hetero_task_begin(
                    /* Num Input Pairs */ 6,
                    Res, Res_Size, V1, V1_Size, V2, V2_Size,
                    left_dim, right_dim, common_dim,
                    /* Num Output Pairs */ 1,
                    Res, Res_Size
                    , /* Optional Node Name */ "matmul_parallel_loop"
                    );


    void* _Section =  __hetero_section_begin();


    for(int i = 0; i < left_dim ; i++){
        for(int j = 0; j < right_dim; j++){
            __hetero_parallel_loop(
                    /* Num Parallel Enclosing Loops */ 2,
                    /* Num Input Pairs */ 6,
                    Res, Res_Size, V1, V1_Size, V2, V2_Size,
                    left_dim, right_dim, common_dim,
                    /* Num Output Pairs */ 1,
                    Res, Res_Size
                    , /* Optional Node Name */ "matmul_parallel_loop"
                    );

            __hetero_hint(/* TARGET */ DEVICE);

            Res[i*right_dim + j] = 0;
            {
            for(int k = 0 ; k < common_dim; k ++){
                // Res[i,j] += V1[i,k] + V2[k,j]
                Res[i*right_dim + j] += V1[i*common_dim + k] * V2[k*right_dim +j];

            }}

        }




    }

    __hetero_section_end(_Section);
        __hetero_task_end(wrapper);

    __hetero_section_end(Section);

}

int main(int argc, char** argv){


    int left_dim = LEFT_DIM;
    int right_dim = RIGHT_DIM;
    int common_dim = COMMON_DIM;

    // If 3 dimension values
    // provided.
    if(argc == 4){
        left_dim = atoi(argv[1]);
        right_dim = atoi(argv[2]);
        common_dim = atoi(argv[3]);
    }

    std::size_t A_Size = sizeof(int) * left_dim * common_dim;
    std::size_t B_Size = sizeof(int) * right_dim * common_dim;
    std::size_t C_Size = sizeof(int) * right_dim * left_dim;

    int* A = (int*) malloc(A_Size);
    int* B = (int*) malloc(B_Size);
    int* C = (int*) malloc(C_Size);


    // Initialize matrices
    for(int i = 0; i < (A_Size)/sizeof(int); i++){
        A[i] = 1;
    }


    for(int i = 0; i < (B_Size)/sizeof(int); i++){
        B[i] = -1;
    }


    printf("Launching matrix multiply dataflow graph! \n");
    void* MatMulDFG = __hetero_launch(
            /* Root Function */
            (void*) matmul,
            /* Num Input Pairs */ 6,
            C, C_Size, false,
            A, A_Size, false,  
            B, B_Size, false, 
            left_dim,
            right_dim, common_dim,
            /* Num Output Pairs */ 1,
            C, C_Size, false
            );

    // Blocking call which waits
    // for the execution of MatmulDFG to complete.
    __hetero_wait(MatMulDFG);
    printf("DFG finished executing!\n");


    printf("Printing Resultant Matrix:\n");
    for(int i = 0; i < left_dim; i++){
        for(int j =0; j < right_dim; j++){
            printf("%d  %s",C[i*right_dim + j], (j == (right_dim - 1)) ? "\n" : " ");
        }
    }



    free(A);
    free(B);
    free(C);
    return 0;


}
