

/*
	����ԕ����H���f��2D
	  
	���̃v���O�����́CPspice A/D��p���ăV�~�����[�V�������s�����߂ɕK�v�ȁC�l�b�g���X�g,.ALIASES�C.prb���쐬���܂��B
	�f�q�ƃm�[�h�̋L�q�ȊO��Pspice�Ő������ꂽ�t�@�C���Ɠ������͂������Ă��܂��D
	�m�[�h�ԍ��̕ς���(�Ȃ����)�͈ȉ��̂Ƃ���

	I_I1	N2	0
	R_Rm1	N1	0
	R_Rp1   N1  N2
	R_Rs1	N1	N3
	C_Cs1	N1	N3

	I_I2	N4	0
	R_Rm2	N3	0
	R_Rp2   N3  N4
	R_Rs2	N3	N5
	C_Cs2	N3	N5

	I_I3	N6	0	
	R_Rm3	N5	0
	R_Rp3   N5  N6	
	R_Rs3	N5	N7
	C_Cs3	N5	N7

*/

#define ANSI
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include<string.h>
#include "nrutil.h" 
#include<time.h>//���Ԃ�p����
#include <direct.h>//�t�H���_���쐬��
#include<sstream> //�����X�g���[��
#include <string>
#include<fstream>
#include<iostream>

void itoa(int val, char *s);

int main(){

////////////////////�����ݒ�@��������//////////////////////////////////////////////////////////////////////////////
	//��H�T�C�Y���w��D�����ł͓��͓d����pixel �~ pixel ���w��
	int sosi_max=31;
	char memo[64]="31�~31kernel";

	//�p�����[�^���w��
	char Rm_parameter[256]="100k";
	char Rg_parameter[256]="100k";
	char Rs_parameter[256]="0.5k";
	char Rc_parameter[256]="100k";
	char Cs_parameter[256]="10p";

	int x,y;

	//matrix
	int **input1=imatrix(1,1000,1,1000);
	int **input2=imatrix(1,1000,1,1000);
	int **NV=imatrix(1,1000,1,1000);
	int **nVR=imatrix(1,1000,1,1000);
	int **nVU=imatrix(1,1000,1,1000);
	//int **gaso=imatrix(1,1000,1,1000);
	//int **input_csv_deta=imatrix(1,1000,1,1000);

	for(y=1;y<=1000;y=y+1){
		for(x=1;x<=1000;x=x+1){

			input1[x][y]=0;
			input2[x][y]=0;
			NV[x][y]=0;
			nVR[x][y]=0;
			nVU[x][y]=0;
			//input_csv_deta[x][y]=0;
		}
	}
	
	//���͓d��(������ԁj���w��
	/*
	for(y=1;y<=sosi_max;++y){
	input1[65][y]=3;//������int�^�ł����w��ł��Ȃ����߁C�����_��\������ɂ͏C�����K�v
	input2[65][y]=3;//������int�^�ł����w��ł��Ȃ����߁C�����_��\������ɂ͏C�����K�v
	}
	*/
	input1[16][16]=3;
	input2[16][16]=3;

	/*
	int csv_read=1;
	static int input_csv_deta_b[512];
	if(csv_read==1){
		x=1;y=1;
		char use_name[128]="input_csv";
		char iutput_csv_name[128];
		char inputimage_directory[64]="..\\input_Image\\";
		sprintf(iutput_csv_name,"%s%s.csv",inputimage_directory,use_name);		//���͉摜�̉�f�l.csv

		
		std::ifstream input_csv(iutput_csv_name);

		if(!input_csv){printf("���̓G���[ input_csv.csv������܂���");return 1;}

		std::string str_input_csv;

		 while(getline(input_csv,str_input_csv)){
        std::string token_input_csv;
        std::istringstream stream(str_input_csv);
		
		while(getline(stream,token_input_csv,',')){
			
		double temp_input_csv=stof(token_input_csv);
		input_csv_deta_b[x]=temp_input_csv;
		//input_csv_deta[x][y]=temp_input_csv;
		//input_csv_deta[x][y]=input_csv_deta_b[x];
		if(input_csv_deta_b[x]==0){
			input1[x][y]=3;
			input2[x][y]=3;
			
		}
		++x;
		}x=1;++y;
		 }
		// fclose(input_csv);
	}*/

////////////////////////////�����ݒ�@�����܂�//////////////////////////////////////////////////////////////////////////

	char *math_name1_s = "SCHEMATIC1.net";				//�o�͌��ʂ̃t�@�C�����̎w��
	char *math_name2_s = "SCHEMATIC1.ALS";
	char *math_name3_s ="�v���p";
	char *math_name4_s;

	char math_name1[64];							//net���X�g						
	char math_name2[64];							//ALS
	char math_name3[64];							//�ϑ��p
	char math_name4[256];

	int i;

	int parameter_num=1;
	int parameter_num_c=1;
	int palameter_num_c_flag=0;
	int parameter_num_V=1;

	

	int measurement_point;					//0,45,90,135,180,225,270,315���w��

	int count_blue;
	char simulation_profile[128];
	char ALIASES[128]="CN @TEST2.SCHEMATIC1(sch_1)";
	FILE *fp;

////////////////////////////�t�H���_�쐬�@��������/////////////////////////////////////////////////////////
	//���s�����̕\��
	time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
	char date[128]="";
	sprintf(date,"%2d/%2d/%2d - %d:%d:%d\n",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	printf(date);

	//���ʂ�ۑ�����t�H���_�̐���
	//�t�H���_���͎��s�����ɂȂ�
	char date_directory[128];
	sprintf(date_directory,"..\\result_buta\\%2d-%02d%02d-%02d%02d%02d\\",pnow->tm_year+1900,pnow->tm_mon + 1,pnow->tm_mday,pnow->tm_hour,pnow->tm_min,pnow->tm_sec);
	 if(_mkdir(date_directory)==0){
        printf("�t�H���_ %s ���쐬���܂���\n",date_directory);
    }else{
        printf("�t�H���_�쐬�Ɏ��s���܂����B\n");
    }

	 sprintf(simulation_profile,"8������H�l�b�g���X�g�쐬�v���O����\n�f�q��%d�~%d\nRm=%s,Rg=%s,Rs=%s,Rc=%s,Cs=%s\n",sosi_max,sosi_max,Rm_parameter,Rg_parameter,Rs_parameter,Rc_parameter,Cs_parameter);
	 printf("%s",simulation_profile);

	strcpy(math_name1, date_directory);
    strcat(math_name1, math_name1_s);
	strcpy(math_name2, date_directory);
    strcat(math_name2, math_name2_s);

/////////////////////////�t�H���_�쐬 �����܂�///////////////////////////////////////////////////////////////////////
	
////////////////////////////////netlist�i��H�f�[�^�j��������///////////////////////////////////////////////////////

/////////////�t�@�C���I�[�v��///////////////////////////////////////////////////////////

	if((fp=fopen(math_name1,"w"))==NULL){
		printf("SCHEMATIC1.net���I�[�v���ł��܂���D\n");
		exit(1);
	}

	fprintf(fp,"* source TEST \n\n");			//�t�@�C����?��s��


///////////////////////Rs�̘a////////////////////////////////////////////////////////////////

	//int N1=16+13*(sosi_max-2)+(13+11*(sosi_max-2))*(sosi_max-2);	//Rs�ɂȂ��m�[�h�̐� = ���̎�����Rs,Cs���Ȃ��n�߂�
	//int N2=N1+8*(sosi_max-1)*(sosi_max-1);									//N1 + Cs,Rc�ɂȂ��m�[�h�̐� = ���̎�����Rg,Rm,V���Ȃ��n�߂�
	int N1=16+13*(sosi_max-1)+(13+11*(sosi_max-1))*(sosi_max-1);
	int N2=N1+8*sosi_max*sosi_max;
	int i2=N1+1;
	int i3=N2+1;
	

	count_blue=1;
	i=1;

	for(y=1;y<=sosi_max;++y){
		for(x=1;x<=sosi_max;++x){

			if(x<sosi_max && y<sosi_max){

//////////y=1 ��������/////////////////////////////////////

			if(y==1){

//////////y=1,x=1 ��������/////////////////////////////////////
				if(x==1){
				for(count_blue=1;count_blue<=16;++count_blue){
					if(count_blue==16){
								
						fprintf(fp,"R_Rs%d	N%d N1 %s\n",parameter_num, i, Rs_parameter);
						fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2-7, i, Cs_parameter);
					}else{
					
					fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
				}
	
					if(palameter_num_c_flag==0){
						palameter_num_c_flag=1;
					}else{
						fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c,i2, i, Rc_parameter);
						if(count_blue!=16)fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c ,i2+1, i, Cs_parameter);
						palameter_num_c_flag=0;
						++parameter_num_c;
						++i2;
					}

					
						if(count_blue==1)NV[1][1]=i;
						if(count_blue==5)NV[1][2]=i;
						if(count_blue==9)NV[2][2]=i;
						if(count_blue==13)NV[x+1][y]=i;
					
						if(count_blue==7)nVU[1][1]=i;
						if(count_blue==11)nVR[1][1]=i;
						
						++i;
					
					++parameter_num;

					
					//++parameter_num_c;
					
				}

//////////y=1,x=1 �����܂�/////////////////////////////////////
				}else{
//////////y=1,x>1 ��������/////////////////////////////////////

					for(count_blue=1;count_blue<=16;++count_blue){
						if(count_blue==1){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, NV[x][y], i, Rs_parameter);fprintf(fp,".model	R_Rs%d RES R=1 DEV=5%%\n",parameter_num);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==2){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, nVR[x-1][y], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==3){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, nVR[x-1][y], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==4){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, NV[x][y+1], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==5){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, NV[x][y+1], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==16){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, NV[x][y], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2-8, i, Cs_parameter);
							++i;
						}

						if(count_blue==6){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==7){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);nVU[x][y]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==8){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==9){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);NV[x+1][y+1]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==10){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==11){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);nVR[x][y]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==12){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==13){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);NV[x+1][y]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==14){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==15){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						
						++parameter_num;
						if(palameter_num_c_flag==0){palameter_num_c_flag=1;}
						else{palameter_num_c_flag=0;++parameter_num_c;}
					
					}
			}
//////////y=1,x>1 �����܂�/////////////////////////////////////
			}
//////////y=1 �����܂�/////////////////////////////////////

//////////y>1 ��������/////////////////////////////////////
			if(y>1){

//////////y>1,x=1 ��������/////////////////////////////////////
				if(x==1){
					for(count_blue=1;count_blue<=16;++count_blue){

						if(count_blue==1){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, NV[x][y], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==12){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, NV[x+1][y], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==13){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, NV[x+1][y], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==14){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, nVU[x][y-1], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==15){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, nVU[x][y-1], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==16){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, NV[x][y], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2-8, i, Cs_parameter);
							++i;
						}

						if(count_blue==2){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==3){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==4){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==5){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);NV[x][y+1]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==6){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==7){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);nVU[x][y]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==8){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==9){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);NV[x+1][y+1]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==10){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==11){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);nVR[x][y]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						
	
						//if(count_blue==5)NV[x][y+1]=i;
						//if(count_blue==9)NV[x+1][y+1]=i;
					
						//if(count_blue==7)nVU[x][y]=i;
						//if(count_blue==11)nVR[x][y]=i;
						
					
					
					++parameter_num;
					if(palameter_num_c_flag==0){palameter_num_c_flag=1;}
					else{palameter_num_c_flag=0;++parameter_num_c;}
////////////////////////////////////////////////////////////////////////////////
					
				}
//////////y>1,x=1 �����܂�/////////////////////////////////////
				}else{


//////////y>1,x>1 ��������/////////////////////////////////////

					for(count_blue=1;count_blue<=16;++count_blue){

						if(count_blue==1){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, NV[x][y], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==2){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, nVR[x-1][y], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==3){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, nVR[x-1][y], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==4){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, NV[x][y+1], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==5){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, NV[x][y+1], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==12){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, NV[x+1][y], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==13){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, NV[x+1][y], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==14){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, nVU[x][y-1], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==15){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, nVU[x][y-1], i, Rs_parameter);
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i, Rc_parameter);
							++i2;
						}
						if(count_blue==16){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, NV[x][y], Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2-8, i, Cs_parameter);
							++i;
						}

						if(count_blue==6){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==7){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);nVU[x][y]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==8){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==9){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);NV[x+1][y+1]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}
						if(count_blue==10){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);
							fprintf(fp,"C_Cs%d	N%d N%d %s\n",parameter_num_c, i2, i, Cs_parameter);
							++i;
						}
						if(count_blue==11){
							fprintf(fp,"R_Rs%d	N%d N%d %s\n",parameter_num, i, i+1, Rs_parameter);nVR[x][y]=i;
							fprintf(fp,"R_Rc%d	N%d N%d %s\n",parameter_num_c, i2, i+1, Rc_parameter);
							++i2;
							++i;
						}

						//if(count_blue==9)NV[x+1][y+1]=i;
						//if(count_blue==13)NV[x+1][y]=i;

						//if(count_blue==7)nVU[x][y]=i;
						//if(count_blue==11)nVR[x][y]=i;

						
						++parameter_num;
						if(palameter_num_c_flag==0){palameter_num_c_flag=1;}
					else{palameter_num_c_flag=0;++parameter_num_c;}
					}
				}

			}
			}
			fprintf(fp,"R_Rm%d	N%d N%d %s\n",parameter_num_V, NV[x][y], i3, Rm_parameter);
			//���͓d���̐ݒ�
			fprintf(fp,"V_V%d	N%d 0	pulse(0V %d.%dV 0us 0.1ns 0.1ns 3.2us 20us)\n", parameter_num_V, i3,input1[x][y],input2[x][y]);

			++i3;
			fprintf(fp,"R_Rg%d	N%d 0 %s\n",parameter_num_V, NV[x][y], Rg_parameter);
			++i3;
			++parameter_num_V;



			
			
			}//x�̐��
		}
//////////y>1,x>1 �����܂�/////////////////////////////////////
			
			
		
	

////////////////////////////////netlist�i��H�f�[�^�j�I���///////////////////////////////////////////////////////


//////////////////////////////////////ALIASES�@��������@��{��netlist�Ɠ����Ƃ���//////////////////////////////////////////////////////

	if((fp=fopen(math_name2,"w"))==NULL){
		printf("SCHEMATIC1.ALS���I�[�v���ł��܂���D\n");
		exit(1);
	}

	fprintf(fp,".ALIASES \n\n");			//��s��




	//matrix������
	for(y=1;y<=1000;y=y+1){
		for(x=1;x<=1000;x=x+1){

			input1[x][y]=0;
			input2[x][y]=0;
			NV[x][y]=0;
			nVR[x][y]=0;
			nVU[x][y]=0;

		}
	}

	parameter_num=1;
	parameter_num_c=1;
	palameter_num_c_flag=0;
	parameter_num_V=1;

	i=1;
	i2=N1+1;
	i3=N2+1;

for(y=1;y<=sosi_max;++y){
		for(x=1;x<=sosi_max;++x){
			if(x<sosi_max && y<sosi_max){

//////////ALIASES y=1 ��������/////////////////////////////////////
			if(y==1){

//////////ALIASES y=1,x=1 ��������/////////////////////////////////////
				if(x==1){
				for(count_blue=1;count_blue<=16;++count_blue){
					if(count_blue==16){
						fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N1 )%s\n",parameter_num, parameter_num,i, ALIASES);
						fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c,i2-7, i, ALIASES);		
						
					}else{
					fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num, parameter_num, i, i+1, ALIASES);
				}
	
					if(palameter_num_c_flag==0){
						palameter_num_c_flag=1;
					}else{
						fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c,i2, i, ALIASES);
						if(count_blue!=16)fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c ,parameter_num_c,i2+1, i, ALIASES);
						palameter_num_c_flag=0;
						++parameter_num_c;
						++i2;
					}

					
						if(count_blue==1)NV[1][1]=i;
						if(count_blue==5)NV[1][2]=i;
						if(count_blue==9)NV[2][2]=i;
						if(count_blue==13)NV[x+1][y]=i;
					
						if(count_blue==7)nVU[1][1]=i;
						if(count_blue==11)nVR[1][1]=i;
						
						++i;
					
					++parameter_num;

				}

//////////ALIASES y=1,x=1 �����܂�/////////////////////////////////////
				}else{

//////////ALIASES y=1,x>1 ��������/////////////////////////////////////
					for(count_blue=1;count_blue<=16;++count_blue){
						if(count_blue==1){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, NV[x][y], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==2){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, nVR[x-1][y], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==3){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, nVR[x-1][y], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==4){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, NV[x][y+1], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==5){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, NV[x][y+1], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==16){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, NV[x][y], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2-8, i, ALIASES);
							++i;
						}

						if(count_blue==6){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==7){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);nVU[x][y]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==8){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==9){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);NV[x+1][y+1]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==10){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==11){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);nVR[x][y]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==12){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==13){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);NV[x+1][y]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==14){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==15){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}


						++parameter_num;
						if(palameter_num_c_flag==0){palameter_num_c_flag=1;}
					else{palameter_num_c_flag=0;++parameter_num_c;}
					}
				}
			}
//////////ALIASES y=1,x>1 �����܂�/////////////////////////////////////
			

//////////ALIASES y>1 ��������/////////////////////////////////////
			if(y>1){

//////////ALIASES y>1,x=1 ��������/////////////////////////////////////
				if(x==1){
					for(count_blue=1;count_blue<=16;++count_blue){

						if(count_blue==1){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, NV[x][y], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==12){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, NV[x+1][y], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==13){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, NV[x+1][y], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==14){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, nVU[x][y-1], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==15){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, nVU[x][y-1], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==16){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, NV[x][y], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2-8, i, ALIASES);
							++i;
						}

						if(count_blue==2){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==3){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==4){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==5){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);NV[x][y+1]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==6){
							
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							
							++i;
						}
						if(count_blue==7){
							
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);nVU[x][y]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==8){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==9){
							
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);NV[x+1][y+1]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==10){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==11){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);nVR[x][y]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						
	

					
					++parameter_num;
					
					if(palameter_num_c_flag==0){palameter_num_c_flag=1;
					}else{palameter_num_c_flag=0;++parameter_num_c;}
				}
//////////ALIASES y>1,x=1 �����܂�/////////////////////////////////////
				}else{
					
//////////ALIASES y>1,x>1 ��������/////////////////////////////////////
					for(count_blue=1;count_blue<=16;++count_blue){

						
						if(count_blue==1){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, NV[x][y], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==2){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, nVR[x-1][y], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==3){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, nVR[x-1][y], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==4){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, NV[x][y+1], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==5){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, NV[x][y+1], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==12){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, NV[x+1][y], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==13){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, NV[x+1][y], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==14){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, nVU[x][y-1], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==15){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, nVU[x][y-1], i, ALIASES);
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i2;
						}
						if(count_blue==16){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, NV[x][y], ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2-8, i, ALIASES);
							++i;
						}
						if(count_blue==6){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						
						if(count_blue==7){
							
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);nVU[x][y]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==8){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==9){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);NV[x+1][y+1]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}
						if(count_blue==10){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);
							fprintf(fp,"C_Cs%d	Cs%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i, ALIASES);
							++i;
						}
						if(count_blue==11){
							fprintf(fp,"R_Rs%d	Rs%d(1=N%d 2=N%d )%s\n",parameter_num,parameter_num, i, i+1, ALIASES);nVR[x][y]=i;
							fprintf(fp,"R_Rc%d	Rc%d(1=N%d 2=N%d )%s\n",parameter_num_c,parameter_num_c, i2, i+1, ALIASES);
							++i2;
							++i;
						}


						++parameter_num;
						if(palameter_num_c_flag==0){palameter_num_c_flag=1;}
					else{palameter_num_c_flag=0;++parameter_num_c;}
					}
				}

			}
			}
			fprintf(fp,"R_Rm%d	Rm%d(1=N%d 2=N%d )%s\n",parameter_num_V, parameter_num_V,NV[x][y], i3, ALIASES);
			fprintf(fp,"V_V%d	V%d(+-=N%d -=0	)%s\n", parameter_num_V, parameter_num_V,i3, ALIASES);
			++i3;
			fprintf(fp,"R_Rg%d	Rg%d(1=N%d 2=0 )%s\n",parameter_num_V, parameter_num_V,NV[x][y], ALIASES);
			++i3;
			++parameter_num_V;

			
			}//x�̐��
			
		}
//////////ALIASES y>1,x>1 �����܂�/////////////////////////////////////

	fprintf(fp,".ENDALIASES \n");			//����
	fclose(fp);

//////////////////////////////////////ALIASES�I���//////////////////////////////////////////////////////


////////////////////////////////////////////�v���p.txt/////////////////////////////////////////////
	
	int count_measure;
	int measurement_number;
	int count_sum;

	for(count_measure=1;count_measure<=8;++count_measure){

		count_sum=0;
		measurement_point=45*(count_measure-1);

		if(measurement_point==0)measurement_number=6;
		if(measurement_point==45)measurement_number=7;
		if(measurement_point==90)measurement_number=8;
		if(measurement_point==135)measurement_number=1;
		if(measurement_point==180)measurement_number=2;
		if(measurement_point==225)measurement_number=3;
		if(measurement_point==270)measurement_number=4;
		if(measurement_point==315)measurement_number=5;

		sprintf(math_name3,"%s%s%d.txt",date_directory,math_name3_s,measurement_point);

		if((fp=fopen(math_name3,"w"))==NULL){
			printf("�v���p.txt���I�[�v���ł��܂���D\n");
			exit(1);
		}

		int count_ani=0;
	for(y=1;y<sosi_max;++y){
		
		for(x=1;x<sosi_max;++x){	
			int i4=measurement_number+count_sum;			//6��0���������甽���v����7
		//	if(y==65){
			fprintf(fp,"V(Rc%d:1) ", i4);
			//}
			count_sum+=8;
			++count_ani;
			//if(count_ani%40==0){fprintf(fp,"\n");count_ani=0;}
			if(count_ani==400){fprintf(fp,"\n");count_ani=0;}
			if(count_ani==100 || count_ani==200 || count_ani==300){fprintf(fp,"\t");}
			
		}
		//fprintf(fp,"\n");		//�f�q��s���Ƃɉ��s
	}
	fclose(fp);
	}
	
////////////////////////////////////////////�v���p�I���/////////////////////////////////////////////

	/////////////////////////////////////////log�t�@�C���̍쐬/////////////////////////////////////////////
	sprintf(math_name4,"%s8dire_RmRg%sRs%sRc%sCs%s_%s_netlist.txt",date_directory,Rm_parameter,Rs_parameter,Rc_parameter,Cs_parameter,memo);
	if((fp=fopen(math_name4,"w"))==NULL){
			printf("log.txt���I�[�v���ł��܂���D\n");
			exit(1);
		}

	fprintf(fp,"8dire_RmRg%sRs%sRc%sCs%s_netlist",Rm_parameter,Rs_parameter,Rc_parameter,Rc_parameter);

	fclose(fp);



	//�z��̉�@

	free_imatrix(input1, 1,1000,1,1000);
	free_imatrix(input2, 1,1000,1,1000);
	free_imatrix(NV, 1,1000,1,1000);
	free_imatrix(nVR, 1,1000,1,1000);
	free_imatrix(nVU, 1,1000,1,1000);

	

	return 0;
}
