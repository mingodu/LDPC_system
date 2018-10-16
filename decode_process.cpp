#include "decoder_init.cpp"
//#include "header.hpp"
using namespace std;






void node_initialization(Bit_Node *Bit_Node, Check_Node *Check_Node, float *LLR_int){
//Initialize Check_Node
    for(int i = 0;i < M;i++){
        //Setting the going out message (Check_to_Bit_LLR) to be all 0
        for(int k = 0;k < Check_Node[i].Line_Number;k++)
            Check_Node[i].Check_to_Bit_LLR[k] = 0;
    }
		
    //Initialize Bit_Node
    for(int i = 0;i < N;i++){
        //Setting the going out message (Bit_to_Check_LLR) to be LLR_int[i](received from channel)
        for(int k = 0;k < Bit_Node[i].Line_Number;k++)
            Bit_Node[i].Bit_to_Check_LLR[k] = LLR_int[i];
    }
}