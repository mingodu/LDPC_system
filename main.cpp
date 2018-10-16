//
//  main.cpp
//  LDPCTraining1
//
//  Created by TuMingo on 2018/4/24.
//  Copyright © 2018 TuMingo. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <ctime>
#include <cstdlib>

//#include "header.hpp"
//#include "decoder_init.cpp"
#include  "decode_process.cpp"
using namespace std;

void perturbing_message_with_position(int noise_position, float *corrupted_message);
void all_zero_message(float *corrupted_message);
void perturbing_message_with_count(int error_count, float *corrupted_message, float *perturbed_message);
int convertDecimalToBinary(int n);
void rand_message(int *rand_message, int seed);
void encode(int *rand_message, int *encoded_message, int **G_Array);
float sigma(float Code_Rate, float Ebn0);
void BPSK(int *encode_message, int *BPSK_encoded_message);

double Ranq1();
float normal(float n1, float n2, float sigma);
void AWGN(float sigma, int *encode_message, float *received_message);


int main(int argc, const char * argv[]) {
	int ttpp;	
	
	Check_Node *Check_Node = new struct Check_Node[M];
	Bit_Node *Bit_Node = new struct Bit_Node[N];
	
	//Define Channel value
    float *perturbed_message = new float[N];
	
    //Define Code_Rate
    Code_Rate = 0.9;
	//Ebn0 = 3.0;
	Ebn0 = atof(argv[1]);;
	int *uncoded_message = new int[K];
	int *encoded_message = new int[N];
	int *BPSK_encoded_message = new int[N];
    float *received_message = new float[N];
    float *LLR_int = new float[N];
    float *LLR_post = new float[N];
    float *LLR_post_tmp = new float[N];
    float *LLR_post_demod = new float[N];
    float *Syndrome = new float[N];
	int weight_record[N];
	for(int rr = 0;rr < N;rr++)
		weight_record[rr] = 0;
	
	
	
	int **H_Array;
	H_Array = new int *[M];
	for(int i = 0;i < M;i++){
		H_Array[i] = new int [N];
	}

	int **G_Array;
	G_Array = new int *[K];
	for(int i = 0;i < K;i++){
		G_Array[i] = new int [N];
	}
	
	int **permuted_H_Array;
	permuted_H_Array = new int *[M];
	for(int i = 0;i < M;i++){
		permuted_H_Array[i] = new int [N];
	}
	
    //open file
	ifstream infileH;
	ifstream infileG;
	infileH.open("test.txt");  
	infileG.open("result.txt");  
	//ifstream infileH("TEST_ARRAY_10_20.txt");  
	//ifstream infileH("H.1200.720.3");
	
	ofstream ofileResult;
	ofileResult.open(argv[2]);
	
	ofileResult << "N = " << N << endl << "K = " << K << endl << "M = " << M << endl;
	
    //Pass the value of H array
    while(!infileH.eof()){
        for(int i = 0;i < M;i++){
            for(int j = 0;j < N;j++){
                infileH >> H_Array[i][j];
                permuted_H_Array[i][j] = H_Array[i][j];
            }
        }
    }
	infileH.close();	
	
    //Pass the value of G array
    while(!infileG.eof()){
        for(int i = 0;i < K;i++){
            for(int j = 0;j < N;j++){
                infileG >> G_Array[i][j];
            }
        }
    }
	infileG.close();

	
	//*******************************Min-sum decoding*******************************
	
	//*******************************Initialization of the H information*******************************
	
	set_line_num_zero(Bit_Node, Check_Node);
    set_line_num(Bit_Node, Check_Node, H_Array);
    allocate_LLR_storage(Bit_Node, Check_Node);
    set_connecting_target(Bit_Node, Check_Node, H_Array);
	
	//*******************************End of Initialization of the H information*******************************
	
	//*******************************AWGN SETTING*******************************

	float sigma0 = sigma(Code_Rate, Ebn0);
	ofileResult << "EbN0 = " << Ebn0 << endl;
	ofileResult << "Sigma = " << sigma0 << endl;
	
	
	int ErrorBit = 0;
	int ErrorFrame = 0;
	int TotalFrame = 0;
	
	//*******************************Start Decoding*******************************
	for(int error_count = 0;error_count < error_count_limit;error_count++){  //insert error at every position
		//cout << "Number " << error_count << " ";
		//ofileResult << "Number " << error_count << " ";
		
		//Store the original noisy message in perturbed_message, and LLR_int to be modified
		//all_zero_message(perturbed_message);
		//perturbing_message_with_count(error_count, perturbed_message, LLR_int);
		
		//Encode
		rand_message(uncoded_message, 300);
		encode(uncoded_message, encoded_message, G_Array);
		
		
		
		//BPSK
		BPSK(encoded_message, BPSK_encoded_message);
		
		//Insert AWGN noise
		AWGN(sigma0, BPSK_encoded_message, LLR_int);
		
		//for(int i = 0;i < N;i++){
		//	if(LLR_int[i] != encoded_message[i])
		//		cout << i << " ";
		//}
		//cout << endl;
		
		//Decode process
		//Hard input to the min-sum decoder
		//Check node LLR, Bit node LLR assignment
		//N - K = M Check nodes, N Bit nodes
		
		//1.Initialization
		node_initialization(Bit_Node, Check_Node, LLR_int);
		
		//2.Repeatedly do the update
		int check_node_count = 0, bit_node_count = 0;
		int iteration = 0;		//calculate iteration count
		bool Syndrome_Bool = 1;	//0 = syndrome check correct; 1 = syndrome check wrong
	
		
		//iteration not exceed the MAX ITERATION and syndrome is not all 0
		while(iteration < Iter_Limit && Syndrome_Bool){
			iteration++;
			
			//(1)Check node update
			for(int i = 0;i < M;i++){  //choose i-th Check node
				//debug_cout << "Operating " << i << "-th Check node" << endl;
				float temp_val;  //register storing minimun val of the connected bit node
				float temp_sgn;  //register storing sign of the multiple value of all connected bit node
				check_node_count = 0;
				for(int j = 0;j < N;j++){  //choose j-th Bit node, update the information from i-th check node to j-th bit node
					if(H_Array[i][j] == 1){  //Connection exists, and do the operation
						//debug_cout << "From " << i << "-th Check node to " << j << "-th Bit node" << endl;
						//Initialization of register
						temp_val = 100;	//In min-sum, we update the value to the smallest absolute coming value
						temp_sgn = 1;	//Sign register initialization
						//Start summing the in coming value
						for(int k = 0;k < N;k++){  //Scan all the Bit Node to find the coming value
							//k-th Bit Node is connected to i-th Check Node, and k is not j
							if(H_Array[i][k] == 1 && k != j){
								//Finding the coming value from k-th Bit_Node
								for(int LLR_Number = 0;LLR_Number < Bit_Node[k].Line_Number;LLR_Number++){
									//The k-th Bit_to_Check_LLR is connected to i-th Bit Node
									if(Bit_Node[k].Bit_Node_Line[LLR_Number] == i){  
										//tanh used in sum-product, changed to min-sum
										//temp_val *= tanh(Bit_Node[k].Bit_to_Check_LLR[LLR_Number]/2.0);
										//debug_cout << "	" << k << "-th Bit node " << Bit_Node[k].Bit_to_Check_LLR[LLR_Number] << endl;
										//debug_cout << "		Original temp_val = " << temp_val << endl;
										if(temp_val > abs( Bit_Node[k].Bit_to_Check_LLR[LLR_Number] )){
											temp_val = abs( Bit_Node[k].Bit_to_Check_LLR[LLR_Number] );
										}
										if(Bit_Node[k].Bit_to_Check_LLR[LLR_Number] < 0){
											temp_sgn *= (-1);
										}
										//debug_cout << "		temp_sgn = " << temp_sgn << " temp_val = " << temp_val << endl;
										break;
									}
								}
							}
						}
						//有線才要寫入值，才要check_node_count加1
						//Check_Node[i].Check_to_Bit_LLR[check_node_count] = 2.0 * atanh(temp_val);
						Check_Node[i].Check_to_Bit_LLR[check_node_count] = SCALE * temp_sgn * temp_val;
						//debug_cout << "	Check_to_Bit_LLR = " << Check_Node[i].Check_to_Bit_LLR[check_node_count] << endl;
						check_node_count++;
					}
				}
			}
			
			//(2)Bit node update
			for(int i = 0;i < N;i++){  //選擇第i個Bit node
				//debug_cout << "Operating " << i << "-th Bit node" << endl;
				bit_node_count = 0;
				for(int j = 0;j < M;j++){  //選擇第j個Check node
					if(H_Array[j][i] == 1){  //有線
						//debug_cout << "From " << i << "-th Bit node to " << j << "-th Check node" << endl;
						Bit_Node[i].Bit_to_Check_LLR[bit_node_count] = LLR_int[i];
						for(int k = 0;k < M;k++){  //掃過其他所有的Check Node，並加總
							if(H_Array[k][i] == 1 && k != j){  //第k個Check Node有連線到這個(第i個)Bit Node，且不是同一條
								for(int LLR_Number = 0;LLR_Number < Check_Node[k].Line_Number;LLR_Number++){  //要找到Check_Node過來的值是多少
									if(Check_Node[k].Check_Node_Line[LLR_Number] == i){
										//debug_cout << "	" << k << "-th Check node " << Check_Node[k].Check_to_Bit_LLR[LLR_Number] << endl;
										//debug_cout << "		Bit_to_Check_LLR = " << Bit_Node[i].Bit_to_Check_LLR[bit_node_count] << " + " << Check_Node[k].Check_to_Bit_LLR[LLR_Number] << endl;
										Bit_Node[i].Bit_to_Check_LLR[bit_node_count] += Check_Node[k].Check_to_Bit_LLR[LLR_Number];
										break;
									}
								}
							}
						}
						//有線才要bit_node_count+1
						//debug_cout << "	Bit_to_Check_LLR = " << Bit_Node[i].Bit_to_Check_LLR[bit_node_count] << endl;
						bit_node_count++;
					}
				}
			}
			
			//debug_cout << "Calculate LLR_Post " << endl;
			//(3)Calculate Lpost
			for(int i = 0;i < N;i++){  //choose i-th Bit Node
				LLR_post[i] = LLR_int[i];
				//debug_cout << i << "-th LLR_Post = " << LLR_int[i];
				for(int j = 0;j < M;j++){  //Scan all Check Node
					if(H_Array[j][i] == 1){  //There is connection
						for(int k = 0;k< Check_Node[j].Line_Number;k++){  //找這條連線是Check Node連線中的第幾個
							if(Check_Node[j].Check_Node_Line[k] == i){  //內存連線資料和當前連線吻合
								//debug_cout  << " + (" << j << "-th Check Node) ";
								LLR_post[i] += Check_Node[j].Check_to_Bit_LLR[k];
								//debug_cout << Check_Node[j].Check_to_Bit_LLR[k];
								break;
							}
						}
					}
				}
				//debug_cout << " = " << LLR_post[i] << endl;
			}
			
			//(4)Hard Decision + Demodulation
			//Hard Decision
			// >0 map to 1, <0 map to  -1
			//Demodulate using BPSK, -1 -> 1, 1 -> 0
			//To conclude, >0 map to 0, <0 map to 1
			for(int i = 0;i < N;i++){
				if(LLR_post[i] < 0)
					LLR_post_demod[i] = 1;
				else
					LLR_post_demod[i] = 0;
				//debug_cout << LLR_post_demod[i] << " ";
			}
			
			
			
			//Check syndrome
			Syndrome_Bool = 0;  //先將情況視為成功，若發現有錯再改為1
			for(int i = 0;i < M;i++){	//M Parity check equation
				int temp = 0;
				for(int j = 0;j < N;j++){
					temp += H_Array[i][j] * LLR_post_demod[j];  //H * x(T)
				}
				if((temp%2) == 0)  //Exclusive OR 奇數為1, 偶數為0
					Syndrome[i] = 0;
				else{
					Syndrome[i] = 1;  //只要任何一個為1, 就判斷為失敗
					Syndrome_Bool = 1;
				}
			}
			//cout << "Syndrome_Bool = " << Syndrome_Bool << endl;
			if( (Syndrome_Bool == 1) && (iteration == Iter_Limit) ){
				for(int i = 0;i < N;i++){
					ofileResult << encoded_message[i] << " ";
				}
				ofileResult << endl;
				for(int i = 0;i < N;i++){
					ofileResult << LLR_int[i] << " ";
				}
				ofileResult << endl;
				for(int i = 0;i < N;i++){
					ofileResult << LLR_post_demod[i] << " ";
				}
				ofileResult << endl;
			}
		}
		//到這邊已經確定Syndrome_Bool=0或是到達次數上限	
		
		
		//Accumulate Errors
		int error_flag = 0;
		for(int i = 0;i < N;i++){
			if(LLR_post_demod[i] != encoded_message[i]){
				ErrorBit++;
				error_flag = 1;
			}
		}
		
		if(error_flag)
			ErrorFrame++;
		
		TotalFrame++;
		
		float error_rate = float( float(ErrorBit)/float(TotalFrame*N));
		
		//cout << "ErrorBit = " << ErrorBit << " ErrorFrame = " << ErrorFrame << " TotalFrame = " << TotalFrame << " Error_rate =" << error_rate << endl;
		//ofileResult << "ErrorBit = " << ErrorBit << " ErrorFrame = " << ErrorFrame << " TotalFrame = " << TotalFrame << " Error_rate =" << error_rate << endl;
		
    }
	
	//OUTPUT ERROR COUNT
	float error_rate = float(ErrorBit/(TotalFrame*N));
	//cout << "SNR = " << Ebn0 << endl;
	ofileResult << "SNR = " << Ebn0 << endl;
	//cout << "Error Rate = " << error_rate << endl;
	ofileResult << "Error Rate = " << error_rate << endl;
	return 0;
}

void perturbing_message_with_position(int noise_position, float *corrupted_message){
	for(int i = 0;i < N;i++){
		if(i == noise_position){
			corrupted_message[i] = -1;
		}
		else{
			corrupted_message[i] = 1;
		}
	}
}

void all_zero_message(float *corrupted_message){
	for(int i = 0;i < N;i++){
		corrupted_message[i] = 1;
	}
}

void rand_message(int *rand_message, int seed){
	srand(seed);
	for(int i = 0;i < N;i++){
		rand_message[i] = rand()%2;
	}
}

void encode(int *rand_message, int *encoded_message, int **G_Array){
	for(int i = 0;i < N;i++){
		int sum = 0;
		for(int j = 0;j < K;j++){
			sum += rand_message[j] * G_Array[j][i];			
		}
		encoded_message[i] = sum %2;
	}
}

void perturbing_message_with_count(int error_count, float *corrupted_message, float *LLR_int){

	int seed = 100;
	srand(seed);
	vector<int> myvector;
	
	// set some values:
	for(int i = 0; i < N; ++i){
		myvector.push_back(i); // 1 2 3 4 5 6 7 8 9
	}
	
	// using built-in random generator:
	random_shuffle( myvector.begin(), myvector.end() );
	
	for(int i = 0;i < error_count;i++){
		corrupted_message[myvector.at(i)] = -1;
	}

    for(int i = 0;i < N;i++)
		LLR_int[i] = corrupted_message[i];
}

int convertDecimalToBinary(int n){
    int binaryNumber = 0;
    int remainder, i = 1;
	int step = 1;

    while(n!=0){
        remainder = n%2;
        n /= 2;
        binaryNumber += remainder*i;
        i *= 10;
	}
    return binaryNumber;
}

double Ranq1(){
    if (RANI == 0){
        RANV = SEED ^ 4101842887655102017LL;
        RANV ^= RANV >> 21;
        RANV ^= RANV << 35;
        RANV ^= RANV >> 4;
        RANV = RANV * 2685821657736338717LL;
        RANI ++;
    }
    RANV ^= RANV >> 21;
    RANV ^= RANV << 35;
    RANV ^= RANV >> 4;
    return RANV * 2685821657736338717LL * 5.42101086242752217E-20;
}

float normal(float n1, float n2, float sigma){
    double x1, x2, s;
    do{
        x1 = Ranq1();
        x2 = Ranq1();
        x1 = 2*x1 - 1;
        x2 = 2*x2 - 1;
        s = pow(x1, 2.0) + pow(x2, 2.0);
    }while (s >= 1.0);
    n1 = sigma * x1 * sqrt( (-2 * log(s) )/s);
    n2 = sigma * x2 * sqrt( (-2 * log(s) )/s);
    return n1;
}

void AWGN(float sigma, int *encode_message, float *received_message){
    //float sum = 0;
	for(int i = 0;i < N;i++){
        float temp1, temp2;
        temp1 = normal(temp1, temp2, sigma);
        received_message[i] = encode_message[i] + temp1;
		//cout << temp1 << " ";
		//sum += temp1;
    }
	//cout << endl << "SUM = " << sum << endl;
}

float sigma(float Code_Rate, float Ebn0){
    //Set Sigma, coderate = K/N, Ebn0 = Channel status(dBs)
    return sqrt(1 / (2 * Code_Rate * pow(10, Ebn0 / 10)));
}

void BPSK(int *encode_message, int *BPSK_encode_message){
    //BPSK Binary Phase Shift Keying
    //1 -> -1, 0 -> 1
    for(int i = 0;i < N;i++){
        //cout << encode_message[i] << " ";
        if(encode_message[i] == 0)
            BPSK_encode_message[i] = 1;
        else
            BPSK_encode_message[i] = -1;
        //cout << encode_message[i] << endl;
    }
}
