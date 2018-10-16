//
//  min_est.h
//  
//
//  Created by TuMingo on 2018/4/24.
//  Copyright © 2018年 TuMingo. All rights reserved.
//

//Choose param_Ne to make Nd small, eg. 6 < Nd < 10
//#define param_Ne 180

//#define N 961
//#define K 775
//#define M 186

//#define N 1200
//#define K 720
//#define M 480


//#define N 2048
//#define K 1723
//#define M 325

#define N 4096
#define K 3686
#define M 410


//#define SCALE 0.75
#define SCALE 0.75

#define error_count_limit 1000000

#define Iter_Limit 50



int param_Nd;

//Define Code_Rate
float Code_Rate;

//Channel AWGN Variable
unsigned long long SEED;  //SEED must be an unsigned int smaller than 4101842887655102017
unsigned long long RANV;
int RANI = 0;



int ttemp;
//Define Channel Status(dBs)
float Ebn0;
//統計codeword數量
float codeword_count;
//Define Channel value
float *received_message;
//Define LLR_int
float *LLR_int;
//Define Lpost
float *LLR_post;
float *LLR_post_demod;
//Define Syndrome
float *Syndrome;
int *message;
