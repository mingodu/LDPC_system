#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<cmath>
#include <fstream>
#include<iomanip>


using namespace std;
const float CodeRate = 0.9;
const int code = 4096;
const int msg = 3686;	//CodeRate * code

const int decode = code - msg;
fstream  file;
int H[decode][code];
int degree_C[decode];
int Valid_C[decode];
int Valid_C_t[decode];
int Valid_V[code];
int Valid_V_t[code];
int choose[decode];
int degree_t;
int counter;
//regular 38
const int Dv = 3;
const int Dc = 8;
int r;
int run;
int sum, finish;
int mm1 = 0;
int mm2 = 0;
int mm3 = 0;
int mmm = 0;
int rt = 0;
int cyclefault;

int main(){
	
	//cout << "K = " << msg << endl;
	srand(time(NULL));

	for (int i = 0; i < decode; i++)
	{
		for (int j = 0; j < code; j++)
		{
			H[i][j] = 0;
		}
	}
	for (int i = 0; i < decode; i++)
	{
		degree_C[i] = 0;
	}
	for (run = 0; run < code; run++)//////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		for (int m = 0; m < Dv; m++)
		{
			if (m == 0)
			{
				r = 0;
				degree_t = degree_C[0];
				for (int i = 1; i < decode; i++)
				{
					if (degree_C[i] - degree_t < 0)
					{
						r = i;
						degree_t = degree_C[i];
					}
				}
				degree_C[r] += 1;
				H[r][run] = 1;
				rt = r;
				mm1 += 1;
				//			cout << run;


				fill(Valid_C, Valid_C + decode, 1);
				fill(Valid_C_t, Valid_C_t + decode, 1);
				fill(Valid_V, Valid_V + code, 0);
				fill(Valid_V_t, Valid_V_t + code, 0);
			}
			else
			{
				///////////////////////////////////TRAVEL////////////////////////////////////////////////////////////////////////////////////////////////////////////
				Valid_V[run] = 1;
				finish = 0;
				int l = 0;
				do
				{
					for (int k = 0; k < code; k++)
					{
						if (Valid_V[k] == 1)
						{
							for (int i = 0; i < decode; i++)
							{
								if (H[i][k] == 1 && (Valid_C_t[i] != 0))
								{
									Valid_C_t[i] = 0;
									Valid_V_t[k] = 0;
									Valid_V[k] = 0;
									for (int j = 0; j < code; j++)
									{
										if (H[i][j] == 1 && (j != k))
										{
											Valid_V_t[j] = 1;
										}
									}
								}

							}
						}
					}
					for (int y = 0; y < code; y++)
						Valid_V[y] = Valid_V_t[y];
					int sum_t = 0;
					sum_t = sum;
					sum = 0;
					for (int g = 0; g < decode; g++)
						sum += Valid_C_t[g];
					int sum_v = 0;
					for (int g = 0; g < code; g++)
						sum_v += Valid_V_t[g];
					if (sum == 0)
					{
						finish = 1;
//						cout << l;
						break;
					}
					else if (sum_v == 0 || sum_t == sum)/////////////////////////////
					{
						finish = 1;
						for (int g = 0; g < decode; g++)
							Valid_C[g] = Valid_C_t[g];
//						cout << l;
						break;
					}
					else
					{
						for (int g = 0; g < decode; g++)
							Valid_C[g] = Valid_C_t[g];
					}
					l += 1;
				} while (finish == 0);
				int ww = 0;//////////////////////////////////////////////////////////////
				for (int d = 0; d < decode; d++)
				{
					if (degree_C[d] < 8 && Valid_C[d] == 1)
						ww += 1;
				}
				if (ww == 0)
				{
//					cout << " " << run;
					for (int u = 0; u < decode; u++)
					{
//						cout << Valid_C[u];
//						cout << degree_C[u];
					}
					cout << "GG";
				}/////////////////////////////////////////////////////////////////////////
				counter = 0;
				r = 0;
				int start = 0;
				degree_t = degree_C[0];
				for (int b = 0; b < decode; b++)
				{
					if (Valid_C[b] == 1)
					{
						start = b;
						degree_t = degree_C[b];
						break;
					}
				}/////////////////////////////////////////////////////////////////////////
				choose[counter] = start;
				for (int h = start + 1; h < decode; h++)
				{
					if (degree_C[h] - degree_t < 0 && Valid_C[h] == 1)
					{
						fill(choose, choose + decode, 0);
						counter = 0;
						choose[counter] = h;
						degree_t = degree_C[h];

					}
					else if (degree_C[h] - degree_t == 0 && Valid_C[h] == 1)
					{
						counter += 1;
						choose[counter] = h;
					}
				}
				if (counter == 0)
				{
					degree_C[choose[counter]] += 1;
					if (H[choose[counter]][run] == 1)
					{
//						cout << " " << counter << " " << choose[counter] << " " << rt << " " << run << " ";
						cout << "GGGG";
					}
					H[choose[counter]][run] = 1;
					mm2 += 1;
					//	cout << run;
				}
				else
				{
					r = (rand() % counter);
					degree_C[choose[r]] += 1;
					if (H[choose[r]][run] == 1)
						cout << "GGGG";
					H[choose[r]][run] = 1;
					mm3 += 1;
					//	cout << run;
				}


				fill(choose, choose + decode, 0);
				fill(Valid_C, Valid_C + decode, 1);
				fill(Valid_C_t, Valid_C_t + decode, 1);
				fill(Valid_V, Valid_V + code, 0);
				fill(Valid_V_t, Valid_V_t + code, 0);
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
		}
	}
	/////////////////////////////////////////cycle 4//////////////////////////////////////////////////////

	for (int y = 0; y < code; y++)
	{
		if (cyclefault == 1)
			break;
		for (int u = 0; u < decode; u++)
		{
			if (cyclefault == 1)
				break;
			if (H[u][y] == 1)
			{
				for (int r = 0; r < code; r++)
				{
					if (cyclefault == 1)
						break;
					if (H[u][r] == 1 && r != y)
					{
						for (int q = 0; q < decode; q++)
						{
							if (cyclefault == 1)
								break;
							if (H[q][r] == 1 && q != u)
							{
								for (int e = 0; e < code; e++)
								{
									if (cyclefault == 1)
										break;
									if (H[q][e] == 1 && e == y)
									{
										cyclefault = 1;
//										cout << "4:" << u << y << "  " << u << r << "  " << q << r << "  " << q << e;

										break;
									}
									if (cyclefault == 1)
										break;
								}
							}

						}
					}
				}

			}

		}

	}





	///////////////////////////////////////////////////////////////////////////////////////////////


	file.open("test.txt", ios::out);

	char tt;
	int cc = 0;
	int dd = 0;
	for (int i = 0; i < decode; i++)
	{
		cc = 0;
		for (int j = 0; j < code; j++)
		{
//			cout << H[i][j];
			tt = H[i][j] + '0';
			file << tt;
			file << " ";
			if (H[i][j] == 1)
			{
				dd += 1;
				cc += 1;
			}
		}
		if (cc > 8 || cc < 7)
		{
			cout << " \n I=" << i;
			cout << "GGGGGGGGGGGGG";
		}
		file << "\n";
		cout << "\n";
	}
	cout << dd;
	cout << "\n";
	cout << mm1 << " " << mm2 << " " << mm3;
	mmm = mm1 + mm2 + mm3;
	cout << mmm;
	file.close();

	return 0;
}








