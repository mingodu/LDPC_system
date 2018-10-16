

//Set Structure of Log-Liklihood of Check nodes
//只存出去的
struct Check_Node{
    int *Check_Node_Line;  //存有連線的#
    int Line_Number;  //存這個點有連到幾個Bit Node
    float *Check_to_Bit_LLR;  //當個Check node出去的LLR值
};

//Set Structure of Log-Liklihood of Bit nodes
struct Bit_Node{
    int *Bit_Node_Line;  //存有連線的#
    int Line_Number;  //存這個點有連到幾個Check Node
    float *Bit_to_Check_LLR;  //當個Bit node出去的LLR值
};