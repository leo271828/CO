// description: matrix muliply without for loop
#include<stdio.h>

int main()
{ 
    int i=0;
    int h[9]={0}, x[3]={0}, y[3]={0}; 
    FILE *input = fopen("../input/4.txt","r");

    for(i = 0; i<9; i++) fscanf(input, "%d", &h[i]);
    for(i = 0; i<3; i++) fscanf(input, "%d", &x[i]);
    for(i = 0; i<3; i++) fscanf(input, "%d", &y[i]);
    fclose(input);

    int *p_x = &x[0] ;
    int *p_h = &h[0] ;
    int *p_y = &y[0] ;

    // x0 = zero
    asm volatile(
        "xor t1, t1, t1\n\t" // L1_index
        "xor t2, t2, t2\n\t" // L2_index
        "addi t3, x0, 3\n\t" // vector length
        "xor t4, t4, t4\n\t" // y[i] addr
        // t5 is y reg
        // t6 is x reg
        // t0 is h reg

        "L1: \n\t" 
        "slli t0, t1, 2  \n\t"
        "add t4, %[y], t0 \n\t"
        "lw  t5, 0(t4)   \n\t"
        "xor t2, t2, t2\n\t" // L2_index

        "L2: \n\t" 
        "slli t0, t2, 2  \n\t"
        "add t6, %[x], t0 \n\t"
        "lw  t6, 0(t6)   \n\t"

        "mul t0, t1, t3  \n\t"
        "add t0, t0, t2  \n\t"
        "slli t0, t0, 2  \n\t"
        "add t0, %[h], t0 \n\t"
        "lw  t0, 0(t0)   \n\t"

        "mul t0, t0, t6  \n\t"
        "add t5, t5, t0  \n\t"

        "addi t2, t2, 1 \n\t" // L2 end
        "bne t2, t3, L2 \n\t"
        

        "sw t5, 0(t4) \n\t"
        "addi t1, t1, 1 \n\t" // L1 end
        "bne t1, t3, L1 \n\t"

        :[y] "+r"(p_y)
        :[h] "r"(p_h) , [x] "r"(p_x)
    );
    
    p_y = &y[0];
    for(i = 0; i<3; i++)
        printf("%d \n", *p_y++);
        
    return(0) ; 
 
}
