
#include "decoder_struc.hpp"
#include "header.hpp"
using namespace std;


void set_line_num_zero(Bit_Node *Bit_Node, Check_Node *Check_Node){
    //setting the line number to 0
    for(int i = 0;i < N;i++)
        Bit_Node[i].Line_Number = 0;
    for(int i = 0;i < M;i++)
        Check_Node[i].Line_Number = 0;
}

void set_line_num(Bit_Node *Bit_Node, Check_Node *Check_Node, int **H_Array){
    //Count the connection node number of every node 
    //Bit Node
    for(int i = 0;i < N;i++){
        for(int j = 0;j < M;j++){
            if(H_Array[j][i] == 1)  //If there is a connection
                Bit_Node[i].Line_Number++;            
        }
    }	

    //Check Node
    for(int i = 0;i < M;i++){
        for(int j = 0;j < N;j++){
            if(H_Array[i][j] == 1)
                Check_Node[i].Line_Number++;
        }
    }
}

void allocate_LLR_storage(Bit_Node *Bit_Node, Check_Node *Check_Node){
    //Allocate LLR Storage
    //Bit Node
    for(int i = 0;i < N;i++){
        Bit_Node[i].Bit_to_Check_LLR = new float[Bit_Node[i].Line_Number];  //Allocate only the number it needs
        Bit_Node[i].Bit_Node_Line = new int[Bit_Node[i].Line_Number];  //Storage of the connected node
    }
    //Check Node
    for(int i = 0;i < N-K;i++){
        Check_Node[i].Check_to_Bit_LLR = new float[Check_Node[i].Line_Number];  //Allocate only the number it needs
        Check_Node[i].Check_Node_Line = new int[Check_Node[i].Line_Number];  //Storage of the connected node
    }


}

void set_connecting_target(Bit_Node *Bit_Node, Check_Node *Check_Node, int **H_Array){
//Record the connecting target
    for(int i = 0;i < M;i++){  //Choose a Check_Node
        int flag = 0;
        for(int j = 0;j < N;j++){  //Scan all the Bit_Node
            if(H_Array[i][j] == 1){  //If connected, store it
				Check_Node[i].Check_Node_Line[flag] = j;
                if(flag >= Check_Node[i].Line_Number){
                    cout << "Line_Number Error" << endl;
                    exit(1);
                }
                flag++;
            }
        }
    }
	
	for(int i = 0;i < N;i++){  //Choose a Bit_Node
        int flag = 0;
        for(int j = 0;j < N-K;j++){
            if(H_Array[j][i] == 1){
                Bit_Node[i].Bit_Node_Line[flag] = j;
                if(flag >= Bit_Node[i].Line_Number){
                    cout << "Line_Number Error" << endl;
                    exit(1);
                }
                flag++;
            }
        }
    }
}


