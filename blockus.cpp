#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
/*
1.��l�ѽL (�j�p14*14)
2.�w�q��쪱�a�o���c
3.�����C��(21��)����إߦn���c
4.�P�_�\��O�_�X�k
5.�p����90��
6.�O�_�����̥��W�������@��

*/
void gameinit();//�C����l

void readfile(char *argv[]);//Ū�J�ѽL���A�γѾl����A�^�Ǧ^�X��
void writefile(char *argv[]);//�t��k�P�_���A��g�ѽL�γѾl���

int setblock(int playernum,int index,int row,int col);//�\����

void rotate(int playernum,int index);//���ɰw����

void updown(int playernum,int index);//�W�U½��

void firstbl(int playernum,int index);//��̥��W����}�]���Ӥ����data[0]

int algo(int playernum,int method);//�M�w�U�@�B�\���m���t��k

int test_set1(int playernum,int index,int row,int col);//���\���������աA�񧹫�}�l�p��
int test_set2(int playernum,int index,int row,int col);//���\���������աA�񧹫�}�l�p��

struct coor{// �C�Ӥp������y��
	int x;
	int y;
};
struct block{ //������X
	struct coor data[10]; //�����C�Ӥp����Adata[0]�ΨӰO���̥��W��������y�СA�H�̱�����g�|����
	int num;//�p����ƶq
	int edge;//������b�h�j������Τ�
	int weight; //����v��
	int use; //������O�_�w�Q�ϥ� �A1:���Q�ϥΡA0:�w�ϥ�
};

struct player{
	int num;//
	char message[100]; //�\��T��
	struct block bl[22];//�C�Ӫ��a��21�Ӥ��,�q1�}�l

};
struct player pl[3];//
int board[16][16];
int round;
int plnum;
int main(int argc,char* argv[]){
	FILE *bstatus,*left;
	bstatus=fopen("BlokusBoard.txt","w");
	left=fopen("LeftPattern_Color.txt","a");
	if(bstatus!=NULL){//��l�ѽL
		for(int i=1;i<=14;i++){
			fprintf(bstatus,"N,N,N,N,N,N,N,N,N,N,N,N,N,N\n");
		}
	}
	else
		printf("BlokusBoard.txt open failed!!!");
	if(left!=NULL){//��l�Ѿl�����ɮ�
		fprintf(left,"Player = N26091819\n");
		fprintf(left,"[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]\n");
	}
	else
		printf("LeftPattern_Color.txt open failed!!!");
	fclose(bstatus);
	fclose(left);
	
	gameinit();
	readfile(argv);
	if(algo(plnum,1)==-1){
		//printf("PASS!!!");
		return 1;
	}

	writefile(argv);
	return 0;
}



void gameinit(){ //�C����l
	int i,j;
	for(i=0;i<=15;i++){
		for(j=0;j<=15;j++){
			board[i][j]=0;
		}
	}
	for(i=1;i<=2;i++){//���@�Ӥ�����\���v��
		pl[i].bl[1].weight=1;//1��
		pl[i].bl[2].weight=2;//2��
		pl[i].bl[3].weight=4;//3��
		pl[i].bl[4].weight=3;
		pl[i].bl[5].weight=9;//4��
		pl[i].bl[6].weight=6;
		pl[i].bl[7].weight=7;
		pl[i].bl[8].weight=8;
		pl[i].bl[9].weight=5;
		pl[i].bl[10].weight=10;//5��
		pl[i].bl[11].weight=20;
		pl[i].bl[12].weight=13;
		pl[i].bl[13].weight=12;
		pl[i].bl[14].weight=19;
		pl[i].bl[15].weight=18;
		pl[i].bl[16].weight=17;
		pl[i].bl[17].weight=21;
		pl[i].bl[18].weight=11;
		pl[i].bl[19].weight=16;
		pl[i].bl[20].weight=15;
		pl[i].bl[21].weight=14;

	}

	for(i=1;i<=2;i++){ // �X�ϦU�j�����`��
		for(j=1;j<=21;j++){
			pl[i].bl[j].use=1;//1�N������|���Q�ϥ�

			if(j==1)
				pl[i].bl[j].num=1;
			else if(j==2)
				pl[i].bl[j].num=2;
			else if(j==3||j==4)
				pl[i].bl[j].num=3;
			else if(j==5||j==6||j==7||j==8||j==9)
				pl[i].bl[j].num=4;
			else
				pl[i].bl[j].num=5;
		}
	}
	for(i=1;i<=2;i++){  //��ϦU�j������
		for(j=1;j<=21;j++){
			if(j==1)
				pl[i].bl[j].edge=1;
			else if(j==2||j==3||j==5)
				pl[i].bl[j].edge=2;
			else if(j==9||j==18||j==19||j==20)
				pl[i].bl[j].edge=4;
			else if(j==21)
				pl[i].bl[j].edge=5;
			else
				pl[i].bl[j].edge=3;//3�̦h
		}
	}

	for(i=1;i<=2;i++){//��l��쪱�a�����
		for(j=1;j<=21;j++){//21�Ӥ��
			//�Ĥ@��
			if(j!=17){
				pl[i].bl[j].data[0].x=1;
				pl[i].bl[j].data[0].y=1;
				pl[i].bl[j].data[1].x=1;
				pl[i].bl[j].data[1].y=1;
			}
			else if(j==17){
				pl[i].bl[j].data[0].x=2;
				pl[i].bl[j].data[0].y=1;
				pl[i].bl[j].data[1].x=2;
				pl[i].bl[j].data[1].y=1;
			}
			//�ĤG��
			if(j!=1&&j!=15&&j!=16&&j!=17){
				pl[i].bl[j].data[2].x=2;
				pl[i].bl[j].data[2].y=1;
			}
			else if(j==15||j==16||j==17){
				pl[i].bl[j].data[2].x=1;
				pl[i].bl[j].data[2].y=2;
			}
			//�ĤT��
			if(j!=1&&j!=2&&j!=3&&j!=5&&j!=8&&j!=14&&j!=15&&j!=16&&j!=17){
				pl[i].bl[j].data[3].x=3;
				pl[i].bl[j].data[3].y=1;
			}
			else if(j==5){
				pl[i].bl[j].data[3].x=1;
				pl[i].bl[j].data[3].y=2;
			}
			else if(j==3||j==8||j==14||j==15||j==16||j==17){
				pl[i].bl[j].data[3].x=2;
				pl[i].bl[j].data[3].y=2;
			}
			//�ĥ|��
			if(j==8||j==14||j==15||j==16||j==17||j==19){
				pl[i].bl[j].data[4].x=3;
				pl[i].bl[j].data[4].y=2;
			}
			else if(j==5||j==7||j==12){
				pl[i].bl[j].data[4].x=2;
				pl[i].bl[j].data[4].y=2;
			}
			else if(j==6||j==10||j==11||j==13){
				pl[i].bl[j].data[4].x=1;
				pl[i].bl[j].data[4].y=2;
			}
			else if(j==9||j==18||j==20||j==21){
				pl[i].bl[j].data[4].x=4;
				pl[i].bl[j].data[4].y=1;
			}
			//�Ĥ���
			if(j==10||j==20){
				pl[i].bl[j].data[5].x=2;
				pl[i].bl[j].data[5].y=2;
			}
			else if(j==11){
				pl[i].bl[j].data[5].x=3;
				pl[i].bl[j].data[5].y=2;
			}
			else if(j==12||j==16||j==17){
				pl[i].bl[j].data[5].x=2;
				pl[i].bl[j].data[5].y=3;
			}
			else if(j==13){
				pl[i].bl[j].data[5].x=1;
				pl[i].bl[j].data[5].y=3;
			}
			else if(j==14||j==15){
				pl[i].bl[j].data[5].x=3;
				pl[i].bl[j].data[5].y=3;
			}
			else if(j==18){
				pl[i].bl[j].data[5].x=1;
				pl[i].bl[j].data[5].y=2;
			}
			else if(j==19){
				pl[i].bl[j].data[5].x=4;
				pl[i].bl[j].data[5].y=2;
			}
			else if(j==21){
				pl[i].bl[j].data[5].x=5;
				pl[i].bl[j].data[5].y=1;
			}
		}

	}

}

int setblock(int playernum,int index,int row,int col){//�\����

	//�U�Ӫ��a���Ĥ@�^�X�n�л\(5,5)��(10,10)
	//�P�_�G���}�C��x y ���i���t���i�W�L�ѽL (ok)
	//���i���| (ok)
	//�ۤv������u���䨤�I (ok)
	//�P�O�H������i��I (ok)
	//�ĤG�����϶}�l�A�C�����ѽL�W�����ϳ������M�P�⪺���ϡu�䨤�v�s��
	int i,j;
	int n = pl[playernum].bl[index].num;//����`��
	int movey = row-pl[playernum].bl[index].data[0].y;
	int movex = col-pl[playernum].bl[index].data[0].x;
	bool flag=false;//�P�_�O�_���䨤�۳s
	for(i=1;i<=n;i++){ //�P�_�G���}�C��x y ���i���t���i�W�L�ѽL
		int x=pl[playernum].bl[index].data[i].x+movex;
		int y=pl[playernum].bl[index].data[i].y+movey;
		if(pl[playernum].bl[index].use==0){
			//printf("������w�Q�ϥ�!!!\n");
			return -1;
		}
		if(x<1||x>14||y<1||y>14){
			//printf("�Щ�b�ѽL��\n");
			return -1;
		}
		if(board[y][x]!=0) {
			//printf("���|��F!\n");
			return -1;
		}
		if(board[y-1][x]==playernum||board[y+1][x]==playernum||board[y][x-1]==playernum||board[y][x+1]==playernum){//
			//printf("���X�k�P�ۤv���������۳s!\n");
			return -1 ;
		}
		if(board[y-1][x-1]==playernum||board[y-1][x+1]==playernum||board[y+1][x-1]==playernum||board[y+1][x+1]==playernum){//�P�ۤv��������䨤�۱��htrue
			flag=true;
		}

	}
	//���\��A�����A�Y���X�k�h�^�h
	if(flag==true||round<=2){ //�e��Ӧ^�X�L�k�P�ۤv���۳s
		pl[playernum].bl[index].use=0;
		for(i=1;i<=n;i++){
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=playernum;
		}
	}
	else {
		//printf("�n�P�ۤv��������䨤�۱�!!!\n");
		return -1;
	}

	if(round==1 && board[5][5]==0 && board[10][10]==0){
		//printf("�Ĥ@�^�X�Щ��(5,5)��(10,10)\n");
		for(i=1;i<=n;i++){//��_�쪬
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
			pl[playernum].bl[index].use=1;
		}
		return -1;
	}
	else if(round==2){
		if(board[5][5] ==0 || board[10][10] ==0 ){

			for(i=1;i<=n;i++){//��_�쪬
				board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
				pl[playernum].bl[index].use=1;
			}
			return -1;
		}
	}

}
void rotate(int playernum,int index){//���ɰw����A����s��
	int i,j,y,x;
	int len=pl[playernum].bl[index].edge;//���
	int n=pl[playernum].bl[index].num;//�����
	int **arr = (int **)calloc(len+1,sizeof(int *)); //�ŧi�@�ӰʺA�G���}�C
	int **temp =(int **)calloc(len+1,sizeof(int *)); //�Ȧs�ഫ�᪺�}�C
	for(i=0;i<=len;i++){
		arr[i]=(int *)calloc(len+1,sizeof(int));
		temp[i]=(int *)calloc(len+1,sizeof(int));
	}
	for(y=1;y<=len;y++){
		for(x=1;x<=len;x++){
			arr[y][x]=10*y+x;
		}
	}
	for(y=1;y<=len;y++){
		for(x=1;x<=len;x++){
			temp[y][x]=arr[x][len+1-y];
		}
	}
	for(i=1;i<=n;i++){
		int tmpy = pl[playernum].bl[index].data[i].y;//�n���s�_��
		int tmpx = pl[playernum].bl[index].data[i].x;//�n���s�_��
		pl[playernum].bl[index].data[i].y=temp[tmpy][tmpx]/10;
		pl[playernum].bl[index].data[i].x=temp[tmpy][tmpx]%10;
	}

	for(i = 0; i < len ; i++) { //�^���Ŷ�
        free(arr[i]);
        free(temp[i]);
    }
    free(arr);
    free(temp);
    firstbl(playernum,index);
}
void updown(int playernum,int index){//�W�U½��A����s��
	int i,j;
	int len=pl[playernum].bl[index].edge;//���
	int n=pl[playernum].bl[index].num;//�����

	for(i=1;i<=n;i++){
		pl[playernum].bl[index].data[i].y = len + 1 -pl[playernum].bl[index].data[i].y;
	}
	firstbl(playernum,index);
}

void firstbl(int playernum,int index){//�����̥��W������A�H�K�̬��\�񤧮y��
	int i,j;
	int min=100,minx,miny;
	int n=pl[playernum].bl[index].num;//����`��

	for(i=1;i<=n;i++){
		if(min > 10*pl[playernum].bl[index].data[i].y+pl[playernum].bl[index].data[i].x){
			min=10*pl[playernum].bl[index].data[i].y+pl[playernum].bl[index].data[i].x;
			miny=pl[playernum].bl[index].data[i].y;
			minx=pl[playernum].bl[index].data[i].x;
		}
	}
	pl[playernum].bl[index].data[0].y=miny;
	pl[playernum].bl[index].data[0].x=minx;
}

int test_set1(int playernum,int index,int row,int col){//�\����
	int i,j;
	int n = pl[playernum].bl[index].num;//����`��
	int movey = row-pl[playernum].bl[index].data[0].y;
	int movex = col-pl[playernum].bl[index].data[0].x;
	bool flag=false;//�P�_�O�_���䨤�۳s
	for(i=1;i<=n;i++){ //�P�_�G���}�C��x y ���i���t���i�W�L�ѽL
		int x=pl[playernum].bl[index].data[i].x+movex;
		int y=pl[playernum].bl[index].data[i].y+movey;
		if(pl[playernum].bl[index].use==0){
			//printf("������w�Q�ϥ�!!!\n");
			return -1;
		}
		if(x<1||x>14||y<1||y>14){
			//printf("�Щ�b�ѽL��\n");
			return -1;
		}
		if(board[y][x]!=0) {
			//printf("���|��F!\n");
			return -1;
		}
		if(board[y-1][x]==playernum||board[y+1][x]==playernum||board[y][x-1]==playernum||board[y][x+1]==playernum){//
			//printf("���X�k�P�ۤv���������۳s!\n");
			return -1 ;
		}
		if(board[y-1][x-1]==playernum||board[y-1][x+1]==playernum||board[y+1][x-1]==playernum||board[y+1][x+1]==playernum){//�P�ۤv��������䨤�۱��htrue
			flag=true;
		}

	}
	/*	�ޥ�:
		1.�ɶq��O�H�����I��
		2.��ۤv������ɶq���n�ŤӦh�Ŷ�
		3.���ݹ�⦳���̥i�H��N����
		4.�Y��⦳�Y�Ӧa��񤣤F�A�h���̥����κޥL
		5.�s�y�X�L�Ĩ��A(�ڤ��Ĥ�񤣤F����)
		6.�Y���T�w�i�񪺤���b�Y��m(�u��������i��)�A�h�����h�ʦ�����Φ��Ŷ�
	*/
	int enenum;//�Ψӭp����Ů��
	if(playernum==1){
		enenum=2;
	}
	else enenum=1;
	//�p��ڤ�/�Ĥ�i���䨤��(ok)�A�񧹫�ڤ�/�Ĥ�i�񪺪Ŷ�(ok)�A�ڤ�/�Ĥ�i�񪺤����(�̫�����n)
	//�񦹤���P��������h�֦a����۾F(ok)�A�񦹤���Უ�ͦh�ֵL�Ĩ�(�ڤ�i��Ĥ�o�񤣤F)(ok)�B�ʦ���h���䨤(ok)
	int mysp=0,enesp=0,tmpmy=0,tmpene=0,unique=0,tmpcon=0;
	float total;
	if(flag==true||round<=2){ //�e��Ӧ^�X�L�k�P�ۤv���۳s
		pl[playernum].bl[index].use=0;
		for(i=1;i<=n;i++){//�����\�b�ѽL�W
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=playernum;
		}

		for(i=1;i<=14;i++){//�}�l�p��
			for(j=1;j<=14;j++){
				if(board[i][j]==0){//���欰�ťi�p�J
					if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//���椣���ۥH�������۾F
						tmpmy++;//�ڤ�i�\�񤧪Ŷ�
						if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){
							mysp++;//�ڤ�i�\���䨤��
						}
					}
				}
				if(board[i][j]==0){
					if(board[i-1][j]!=enenum&&board[i+1][j]!=enenum&&board[i][j-1]!=enenum&&board[i][j+1]!=enenum){//���椣�P�ۤv�������۾F
						tmpene++;//�Ĥ�i�\�񤧪Ŷ�
						if(board[i-1][j-1]==enenum||board[i-1][j+1]==enenum||board[i+1][j-1]==enenum||board[i+1][j+1]==enenum){
							enesp++;//�Ĥ�i�\���䨤��
						}
					}
				}
				//int tmpcon=0;//(�p��񦹤�������I��F�h�ֶ�)
				if(board[i][j]==enenum){//�q��誺������ץh��(�p��񦹤�������I��F�h�ֶ�)
					if(board[i-1][j]==playernum||board[i+1][j]==playernum||board[i][j-1]==playernum||board[i][j+1]==playernum){
						tmpcon++;
					}
				}
				//int unique=0;//���ͦh�ֵL�Ĩ�(�ڤ�i��Ĥ�o�񤣤F)�e�������n
				if(board[i][j]==0){
					if(board[i-1][j]==enenum||board[i+1][j]==enenum||board[i][j-1]==enenum||board[i][j+1]==enenum){ //���֦��@��P�Ĥ�۳s(�Ĥ褣���)
						if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//4�䤣���ۤv����۳s
							if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){//4���ܦh�@�Ӭ۳s
								if(board[i-1][j-1]==playernum&&board[i+1][j+1]==playernum)
									break;
								else if(board[i-1][j+1]==playernum&&board[i+1][j-1]==playernum)
									break;
								else
									unique++;
							}
						}
					}
				}

			}

		}
		//�ˬd������y���h�֮įq
		for(i=1;i<=n;i++){//�����\�b�ѽL�W
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex];
		}
		for(i=1;i<=n;i++){//��_�쪬
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
			pl[playernum].bl[index].use=1;
		}
		//���^�X���Q
		if(round<=14)//�e7�^�X
			total=1000+1.5*mysp-3*enesp+2*n+tmpmy-tmpene+tmpcon*1.2+4*unique;//�]�v��
		else if(round>=15&&round<=26){//8��13�^�X
			total=1000+1.5*mysp-3*enesp+1.5*n+tmpmy-tmpene+2*unique;//�]�v��
		}
		else {//
			total=1000+1.5*mysp-3*enesp+n+tmpmy-tmpene+unique;//�]�v��
		}
		return total;
	}
	else {
		//printf("�n�P�ۤv��������䨤�۱�!!!\n");
		return -1;
	}

}

int test_set2(int playernum,int index,int row,int col){//�\����
	int i,j;
	int n = pl[playernum].bl[index].num;//����`��
	int movey = row-pl[playernum].bl[index].data[0].y;
	int movex = col-pl[playernum].bl[index].data[0].x;
	bool flag=false;//�P�_�O�_���䨤�۳s
	for(i=1;i<=n;i++){ //�P�_�G���}�C��x y ���i���t���i�W�L�ѽL
		int x=pl[playernum].bl[index].data[i].x+movex;
		int y=pl[playernum].bl[index].data[i].y+movey;
		if(pl[playernum].bl[index].use==0){
			//printf("������w�Q�ϥ�!!!\n");
			return -1;
		}
		if(x<1||x>14||y<1||y>14){
			//printf("�Щ�b�ѽL��\n");
			return -1;
		}
		if(board[y][x]!=0) {
			//printf("���|��F!\n");
			return -1;
		}
		if(board[y-1][x]==playernum||board[y+1][x]==playernum||board[y][x-1]==playernum||board[y][x+1]==playernum){//
			//printf("���X�k�P�ۤv���������۳s!\n");
			return -1 ;
		}
		if(board[y-1][x-1]==playernum||board[y-1][x+1]==playernum||board[y+1][x-1]==playernum||board[y+1][x+1]==playernum){//�P�ۤv��������䨤�۱��htrue
			flag=true;
		}

	}
	/*	�ޥ�:
		1.�ɶq��O�H�����I��
		2.��ۤv������ɶq���n�ŤӦh�Ŷ�
		3.���ݹ�⦳���̥i�H��N����
		4.�Y��⦳�Y�Ӧa��񤣤F�A�h���̥����κޥL
		5.�s�y�X�L�Ĩ��A(�ڤ��Ĥ�񤣤F����)
		6.�Y���T�w�i�񪺤���b�Y��m(�u��������i��)�A�h�����h�ʦ�����Φ��Ŷ�
	*/
	int enenum;//�Ψӭp����Ů��
	if(playernum==1){
		enenum=2;
	}
	else enenum=1;
	//�p��ڤ�/�Ĥ�i���䨤��(ok)�A�񧹫�ڤ�/�Ĥ�i�񪺪Ŷ�(ok)�A�ڤ�/�Ĥ�i�񪺤����
	//�񦹤���P��������h�֦a����۾F(ok)�A�񦹤���Უ�ͦh�ֵL�Ĩ�(�ڤ�i��Ĥ�o�񤣤F)(ok)�B�ʦ���h���䨤(ok)
	int mysp=0,enesp=0,tmpmy=0,tmpene=0,unique=0,tmpcon=0;
	float total;
	if(flag==true||round<=2){ //�e��Ӧ^�X�L�k�P�ۤv���۳s
		pl[playernum].bl[index].use=0;
		for(i=1;i<=n;i++){//�����\�b�ѽL�W
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=playernum;
		}

		for(i=1;i<=14;i++){//�}�l�p��
			for(j=1;j<=14;j++){
				if(board[i][j]==0){//���欰�ťi�p�J
					if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//���椣���ۥH�������۾F
						tmpmy++;//�ڤ�i�\�񤧪Ŷ�
						if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){
							mysp++;//�ڤ�i�\���䨤��
						}
					}
				}
				if(board[i][j]==0){
					if(board[i-1][j]!=enenum&&board[i+1][j]!=enenum&&board[i][j-1]!=enenum&&board[i][j+1]!=enenum){//���椣�P�ۤv�������۾F
						tmpene++;//�Ĥ�i�\�񤧪Ŷ�
						if(board[i-1][j-1]==enenum||board[i-1][j+1]==enenum||board[i+1][j-1]==enenum||board[i+1][j+1]==enenum){
							enesp++;//�Ĥ�i�\���䨤��
						}
					}
				}
				//int tmpcon=0;//(�p��񦹤�������I��F�h�ֶ�)
				if(board[i][j]==enenum){//�q��誺������ץh��(�p��񦹤�������I��F�h�ֶ�)
					if(board[i-1][j]==playernum||board[i+1][j]==playernum||board[i][j-1]==playernum||board[i][j+1]==playernum){
						tmpcon++;
					}
				}
				//int unique=0;//���ͦh�ֵL�Ĩ�(�ڤ�i��Ĥ�o�񤣤F)�e�������n
				if(board[i][j]==0){
					if(board[i-1][j]==enenum||board[i+1][j]==enenum||board[i][j-1]==enenum||board[i][j+1]==enenum){ //���֦��@��P�Ĥ�۳s(�Ĥ褣���)
						if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//4�䤣���ۤv����۳s
							if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){//4���ܦh�@�Ӭ۳s
								if(board[i-1][j-1]==playernum&&board[i+1][j+1]==playernum)
									break;
								else if(board[i-1][j+1]==playernum&&board[i+1][j-1]==playernum)
									break;
								else
									unique++;
							}
						}
					}
				}

			}

		}
		//�ˬd������y���h�Ϯįq
		for(i=1;i<=n;i++){//�����\�b�ѽL�W
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex];
		}
		for(i=1;i<=n;i++){//��_�쪬
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
			pl[playernum].bl[index].use=1;
		}
		//���^�X���Q
		if(round<=14)//�e7�^�X
			total=1000+1.3*mysp-2*enesp+2*n+0.7*tmpmy-0.7*tmpene+1.2*tmpcon+3*unique;//�]�v��
		else if(round>=15&&round<=26){//8��13�^�X
			total=1000+1.2*mysp-1.5*enesp+1.6*n+0.6*tmpmy-0.6*tmpene+1.2*tmpcon+2*unique;//�]�v��
		}
		else {//
			total=1000+mysp-1.5*enesp+n+1.2*tmpmy-1.5*tmpene+1.2*tmpcon+2*unique;//�]�v��
		}
		return total;
	}
	else {
		//printf("�n�P�ۤv��������䨤�۱�!!!\n");
		return -1;
	}

}
int algo(int playernum,int method){//�L�k�A�񬰶�-1�A�i�H�h�^�ǿ�h������s��

	/*
	1.�ɤO�k�A�b14*14�ѽL�W�A�թҦ�����ΩҦ�����B�W�U���i��
	2.�i�H�񤧫�}�l�p��Ů�A��̨θ�
	3.�i���^�X���Q�A�e5�^�X�u��q�Ľs��10�������� */
	if(round==1){//�U�Ӫ��a�o�Ĥ@�^�X
		setblock(playernum,17,4,5);
		pl[playernum].bl[17].use=0;
		return 17;
	}
	else if(round==2){
		if(board[5][5]==0)
			setblock(playernum,17,4,5);
		else
			setblock(playernum,17,9,10);
		pl[playernum].bl[17].use=0;
		return 17;
	}

	int tmpindex,tmpro,tmpmi,tmprow,tmpcol;
	float total,max=-1;
	//����:����s��,�\���m,����X��,�W�U½��
	//�p��(1)������񧹧ڤ��`�@���X���䨤�i�� (2)����`�@���X���䨤�i��
	//(3) �񧹫�ڤ褧���٦��X�Ӥ���i��
	int tmpi,i,j,mir,ro;
	for(tmpi=21;tmpi>=1;tmpi--){//����s��
		if(pl[playernum].bl[tmpi].use==1){//����O�_�w�ιL
			for(mir=1;mir<=2;mir++){//�i�h�j��⦸ �W��U
				for(ro=1;ro<=4;ro++){//4�Ӥ�V(����)

					for(i=1;i<=14;i++){
						for(j=1;j<=14;j++){
							if(method==1)
								total=test_set1(playernum,tmpi,i,j);
							else if(method==2)
								total=test_set2(playernum,tmpi,i,j);

							if(total>max){
								max=total;
								tmprow=i;
								tmpcol=j;
								tmpindex=tmpi;
								tmpro=ro-1;
								tmpmi=mir-1;
							}
						}
					}

					rotate(playernum,tmpi);//�৹4���^��쥻��
				}
				updown(playernum,tmpi);
			}

		}

	}

	if(max==-1){
		//printf("�L�k�A��,PASS!!!\n");
		return -1;
	}
	else{
		pl[playernum].bl[tmpindex].use=0;

		//�@�w�n�����W�U½��~�����A�n��W���@�˧_�h�|��
		for(i=1;i<=tmpmi;i++){
			updown(playernum,tmpindex);
		}
		for(i=1;i<=tmpro;i++){
			rotate(playernum,tmpindex);
		}

		int rw=tmprow-pl[playernum].bl[tmpindex].data[0].y;
		int cl=tmpcol-pl[playernum].bl[tmpindex].data[0].x;
		for(i=1;i<=pl[playernum].bl[tmpindex].num;i++){//�����\�b�ѽL�W
			board[pl[playernum].bl[tmpindex].data[i].y+rw][pl[playernum].bl[tmpindex].data[i].x+cl]=playernum;
		}
		//printf("���a%d:��ܪ��O��� %d,����%d��,�W�U½�� %d���A��b(%d,%d)\n",playernum,tmpindex,tmpro,tmpmi,tmprow,tmpcol);
		return tmpindex;
	}
}

void readfile(char *argv[]){//�^�Ǫ��a���Ϊ��a��
	int i,j;
	char temp[20][20];
	char arr[100]={0};
	FILE *bstatus,*left,*slog;
	char ch,color[5];
	int col=0,row=1;

	//color=fopen(argv[1],"r");//"color.txt"
	strcpy(color,argv[1]);

	color[0]=tolower(color[0]);
	if(color[0]=='r')
		plnum=1;
	else
		plnum=2;
	//printf("test:%c,plnum:%d\n",color[0],plnum);

	bstatus=fopen(argv[2],"r");//"BlokusBoard.txt"
	while((ch=getc(bstatus))!=EOF){
			if(ch=='N'||ch=='R'||ch=='B'){
				col++;
				if(col!=15){
					temp[row][col]=ch;
				}
				else if(col==15){
					col=1;
					row++;
					temp[row][col]=ch;
				}
			}
	}
	for(i=1;i<=14;i++){
		for(j=1;j<=14;j++){
			if(temp[i][j]=='N')
				board[i][j]=0;
			else if(temp[i][j]=='R')
				board[i][j]=1;
			else if(temp[i][j]=='B')
				board[i][j]=2;
		}
	}
	fclose(bstatus);

	left=fopen(argv[3],"r");//"LeftPattern_Color.txt"
	while(fgets(arr,sizeof(arr),left)){
		arr[strlen(arr)-1]='\0';
	}
	int num=1;
	for(i=0;i<strlen(arr);i++){
		if(arr[i]=='1'){
			pl[plnum].bl[num].use=1;
			num++;
		}
		else if(arr[i]=='0'){
			pl[plnum].bl[num].use=0;
			num++;
		}
	}
	fclose(left);
	/*for(i=1;i<=21;i++){
		printf("%d ",pl[plnum].bl[i].use);
	}
	printf("\n");*/

	for(i=0;i<100;i++){
		arr[i]=' ';
	}
	slog=fopen(argv[4],"r");//"StepLog.txt"
	while(fgets(arr,sizeof(arr),slog)){
		arr[strlen(arr)-1]='\0';
		//printf("%s,arr[0]:%d\n",arr,arr[0]);
	}
	int tmpr;
	tmpr=atoi(arr);
	if(tmpr<0)
		round=1;
	else
		round=tmpr+1;
	//printf("tmpr:%d\n",tmpr);

	fclose(slog);

}
void writefile(char *argv[]){
	int i,j;
	FILE *bstatus,*left;
	bstatus=fopen(argv[2],"w");
	left=fopen(argv[3],"a");

	for(i=1;i<=14;i++){
		for(j=1;j<=14;j++){
			if(j!=14){
				if(board[i][j]==0)
					fprintf(bstatus,"%c,",'N');
				else if(board[i][j]==1){
					fprintf(bstatus,"%c,",'R');
				}
				else if(board[i][j]==2){
					fprintf(bstatus,"%c,",'B');
				}
			}
			else if(j==14){
				if(board[i][j]==0)
					fprintf(bstatus,"%c",'N');
				else if(board[i][j]==1){
					fprintf(bstatus,"%c",'R');
				}
				else if(board[i][j]==2){
					fprintf(bstatus,"%c",'B');
				}
			}
		}
		fprintf(bstatus,"\n");
	}
	fprintf(left,"\n[");
	for(i=1;i<=21;i++){
		if(i!=21)
			fprintf(left,"%d,",pl[plnum].bl[i].use);
		else
			fprintf(left,"%d]",pl[plnum].bl[i].use);
	}
	fclose(bstatus);
	fclose(left);
}
/*
	�n�\����̪��t��k

1.�Ҽ{���I��: (1)������񧹧ڤ��`�@���X���䨤�i�� (2)����`�@���X���䨤�i��
(3) �񧹫�ڤ褧���٦��X�Ӥ���i��
2.�C�Ӥ���i�]�v���A�Y�J���Ӥ���@�˫h���v��
3.��Ҧ��ۤv������i�\���䨤��m���Ҽ{�i�h
4.�h�ˬd�U����\�񧹡A�ۤv�٦��h�֦a��i�H��A�ι�⦳�h�֦a
��i�\
5.�n�Ҽ{��������g�U���p
6.�ɶq�h�]���誺�X�l
7.�i�Ҽ{���Ѿl������A�h������
8.�̫��X�䤤�@�Ӥ��


	Ū�ɼg��

1.Ū��:�ѽL�ثe���A�A�Ѿl�����ɮסA�ѨB�����ɮ�
2.�g�J(��s):�ѽL�ثe���A�ɮסA�Ѿl�����ɮ�(���Χ�s�ѨB�O���ɮ�)

STEP:
1.Ū�ثe�ѽL���A�A�Ѿl��� �A���P�_�U�@�B���
2.�M�w������s�ѽL���A�A�Ѿl���

QUESTION
1.�p�󪾹D�ۤv�O�Ĥ@���٬O�ĤG�Ӫ��a
2.�@��G���O�N��ƻ��C��
3.�u�n��s�ۤv���ݪ��Ѿl���?
4.�iŪ���O�H�Ѿl�������


1.�C�^�X��Ū�ѽL���A�γѾl������ɮ�
2.�N�ܢ���ഫ���������]�Ĥ@�쪱�a��������^
3.�N�J�t��k�M�w�U�@�B
4.��s�ѽL���A�Φۤv���Ѿl���
*/

