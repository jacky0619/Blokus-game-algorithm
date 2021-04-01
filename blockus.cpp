#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
/*
1.初始棋盤 (大小14*14)
2.定義兩位玩家得結構
3.先幫每個(21個)方塊建立好結構
4.判斷擺放是否合法
5.如何轉90度
6.是否紀錄最左上角的那一塊

*/
void gameinit();//遊戲初始

void readfile(char *argv[]);//讀入棋盤狀態及剩餘方塊，回傳回合數
void writefile(char *argv[]);//演算法判斷完，改寫棋盤及剩餘方塊

int setblock(int playernum,int index,int row,int col);//擺放方塊

void rotate(int playernum,int index);//順時針旋轉

void updown(int playernum,int index);//上下翻轉

void firstbl(int playernum,int index);//把最左上的位址設給該方塊的data[0]

int algo(int playernum,int method);//決定下一步擺放位置之演算法

int test_set1(int playernum,int index,int row,int col);//先擺放方塊做測試，放完後開始計算
int test_set2(int playernum,int index,int row,int col);//先擺放方塊做測試，放完後開始計算

struct coor{// 每個小方塊的座標
	int x;
	int y;
};
struct block{ //方塊集合
	struct coor data[10]; //紀錄每個小方塊，data[0]用來記錄最左上角的方塊座標，隨者旋轉鏡射會改變
	int num;//小方塊數量
	int edge;//此方塊在多大的正方形中
	int weight; //方塊權重
	int use; //此方塊是否已被使用 ，1:未被使用，0:已使用
};

struct player{
	int num;//
	char message[100]; //擺放訊息
	struct block bl[22];//每個玩家有21個方塊,從1開始

};
struct player pl[3];//
int board[16][16];
int round;
int plnum;
int main(int argc,char* argv[]){
	FILE *bstatus,*left;
	bstatus=fopen("BlokusBoard.txt","w");
	left=fopen("LeftPattern_Color.txt","a");
	if(bstatus!=NULL){//初始棋盤
		for(int i=1;i<=14;i++){
			fprintf(bstatus,"N,N,N,N,N,N,N,N,N,N,N,N,N,N\n");
		}
	}
	else
		printf("BlokusBoard.txt open failed!!!");
	if(left!=NULL){//初始剩餘拼圖檔案
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



void gameinit(){ //遊戲初始
	int i,j;
	for(i=0;i<=15;i++){
		for(j=0;j<=15;j++){
			board[i][j]=0;
		}
	}
	for(i=1;i<=2;i++){//哪一個方塊先擺之權重
		pl[i].bl[1].weight=1;//1塊
		pl[i].bl[2].weight=2;//2塊
		pl[i].bl[3].weight=4;//3塊
		pl[i].bl[4].weight=3;
		pl[i].bl[5].weight=9;//4塊
		pl[i].bl[6].weight=6;
		pl[i].bl[7].weight=7;
		pl[i].bl[8].weight=8;
		pl[i].bl[9].weight=5;
		pl[i].bl[10].weight=10;//5塊
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

	for(i=1;i<=2;i++){ // 出使各磚塊的總數
		for(j=1;j<=21;j++){
			pl[i].bl[j].use=1;//1代表此方塊尚未被使用

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
	for(i=1;i<=2;i++){  //初使各磚塊的邊
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
				pl[i].bl[j].edge=3;//3最多
		}
	}

	for(i=1;i<=2;i++){//初始兩位玩家的方塊
		for(j=1;j<=21;j++){//21個方塊
			//第一塊
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
			//第二塊
			if(j!=1&&j!=15&&j!=16&&j!=17){
				pl[i].bl[j].data[2].x=2;
				pl[i].bl[j].data[2].y=1;
			}
			else if(j==15||j==16||j==17){
				pl[i].bl[j].data[2].x=1;
				pl[i].bl[j].data[2].y=2;
			}
			//第三塊
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
			//第四塊
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
			//第五塊
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

int setblock(int playernum,int index,int row,int col){//擺放方塊

	//各個玩家的第一回合要覆蓋(5,5)或(10,10)
	//判斷二維陣列的x y 不可為負不可超過棋盤 (ok)
	//不可重疊 (ok)
	//自己的方塊只能邊角碰 (ok)
	//與別人的方塊可邊碰 (ok)
	//第二塊拼圖開始，每次放到棋盤上的拼圖都必須和同色的拼圖「邊角」連接
	int i,j;
	int n = pl[playernum].bl[index].num;//方塊總數
	int movey = row-pl[playernum].bl[index].data[0].y;
	int movex = col-pl[playernum].bl[index].data[0].x;
	bool flag=false;//判斷是否有邊角相連
	for(i=1;i<=n;i++){ //判斷二維陣列的x y 不可為負不可超過棋盤
		int x=pl[playernum].bl[index].data[i].x+movex;
		int y=pl[playernum].bl[index].data[i].y+movey;
		if(pl[playernum].bl[index].use==0){
			//printf("此方塊已被使用!!!\n");
			return -1;
		}
		if(x<1||x>14||y<1||y>14){
			//printf("請放在棋盤內\n");
			return -1;
		}
		if(board[y][x]!=0) {
			//printf("重疊到了!\n");
			return -1;
		}
		if(board[y-1][x]==playernum||board[y+1][x]==playernum||board[y][x-1]==playernum||board[y][x+1]==playernum){//
			//printf("不合法與自己的方塊有邊相連!\n");
			return -1 ;
		}
		if(board[y-1][x-1]==playernum||board[y-1][x+1]==playernum||board[y+1][x-1]==playernum||board[y+1][x+1]==playernum){//與自己的方塊有邊角相接則true
			flag=true;
		}

	}
	//先擺放再偵測，若不合法則回退
	if(flag==true||round<=2){ //前兩個回合無法與自己角相連
		pl[playernum].bl[index].use=0;
		for(i=1;i<=n;i++){
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=playernum;
		}
	}
	else {
		//printf("要與自己的方塊有邊角相接!!!\n");
		return -1;
	}

	if(round==1 && board[5][5]==0 && board[10][10]==0){
		//printf("第一回合請放到(5,5)或(10,10)\n");
		for(i=1;i<=n;i++){//恢復原狀
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
			pl[playernum].bl[index].use=1;
		}
		return -1;
	}
	else if(round==2){
		if(board[5][5] ==0 || board[10][10] ==0 ){

			for(i=1;i<=n;i++){//恢復原狀
				board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
				pl[playernum].bl[index].use=1;
			}
			return -1;
		}
	}

}
void rotate(int playernum,int index){//順時針旋轉，方塊編號
	int i,j,y,x;
	int len=pl[playernum].bl[index].edge;//邊長
	int n=pl[playernum].bl[index].num;//方塊數
	int **arr = (int **)calloc(len+1,sizeof(int *)); //宣告一個動態二維陣列
	int **temp =(int **)calloc(len+1,sizeof(int *)); //暫存轉換後的陣列
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
		int tmpy = pl[playernum].bl[index].data[i].y;//要先存起來
		int tmpx = pl[playernum].bl[index].data[i].x;//要先存起來
		pl[playernum].bl[index].data[i].y=temp[tmpy][tmpx]/10;
		pl[playernum].bl[index].data[i].x=temp[tmpy][tmpx]%10;
	}

	for(i = 0; i < len ; i++) { //回收空間
        free(arr[i]);
        free(temp[i]);
    }
    free(arr);
    free(temp);
    firstbl(playernum,index);
}
void updown(int playernum,int index){//上下翻轉，方塊編號
	int i,j;
	int len=pl[playernum].bl[index].edge;//邊長
	int n=pl[playernum].bl[index].num;//方塊數

	for(i=1;i<=n;i++){
		pl[playernum].bl[index].data[i].y = len + 1 -pl[playernum].bl[index].data[i].y;
	}
	firstbl(playernum,index);
}

void firstbl(int playernum,int index){//紀錄最左上之方塊，以便最為擺放之座標
	int i,j;
	int min=100,minx,miny;
	int n=pl[playernum].bl[index].num;//方塊總數

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

int test_set1(int playernum,int index,int row,int col){//擺放方塊
	int i,j;
	int n = pl[playernum].bl[index].num;//方塊總數
	int movey = row-pl[playernum].bl[index].data[0].y;
	int movex = col-pl[playernum].bl[index].data[0].x;
	bool flag=false;//判斷是否有邊角相連
	for(i=1;i<=n;i++){ //判斷二維陣列的x y 不可為負不可超過棋盤
		int x=pl[playernum].bl[index].data[i].x+movex;
		int y=pl[playernum].bl[index].data[i].y+movey;
		if(pl[playernum].bl[index].use==0){
			//printf("此方塊已被使用!!!\n");
			return -1;
		}
		if(x<1||x>14||y<1||y>14){
			//printf("請放在棋盤內\n");
			return -1;
		}
		if(board[y][x]!=0) {
			//printf("重疊到了!\n");
			return -1;
		}
		if(board[y-1][x]==playernum||board[y+1][x]==playernum||board[y][x-1]==playernum||board[y][x+1]==playernum){//
			//printf("不合法與自己的方塊有邊相連!\n");
			return -1 ;
		}
		if(board[y-1][x-1]==playernum||board[y-1][x+1]==playernum||board[y+1][x-1]==playernum||board[y+1][x+1]==playernum){//與自己的方塊有邊角相接則true
			flag=true;
		}

	}
	/*	技巧:
		1.盡量跟別人方塊邊碰邊
		2.跟自己的方塊盡量不要空太多空間
		3.先看對手有那裡可以放就先放那
		4.若對手有某個地方放不了，則那裡先不用管他
		5.製造出無敵角，(我方放敵方放不了的角)
		6.若有確定可放的方塊在某位置(只有此方塊可放)，則先不去動此方塊及此空間
	*/
	int enenum;//用來計算對方空格數
	if(playernum==1){
		enenum=2;
	}
	else enenum=1;
	//計算我方/敵方可放之邊角數(ok)，放完後我方/敵方可放的空間(ok)，我方/敵方可放的方塊數(最後較重要)
	//放此方塊與對手方塊有多少地方邊相鄰(ok)，放此方塊後產生多少無敵角(我方可放敵方卻放不了)(ok)、封住對手多少邊角(ok)
	int mysp=0,enesp=0,tmpmy=0,tmpene=0,unique=0,tmpcon=0;
	float total;
	if(flag==true||round<=2){ //前兩個回合無法與自己角相連
		pl[playernum].bl[index].use=0;
		for(i=1;i<=n;i++){//先試擺在棋盤上
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=playernum;
		}

		for(i=1;i<=14;i++){//開始計算
			for(j=1;j<=14;j++){
				if(board[i][j]==0){//此格為空可計入
					if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//此格不予自以方塊有邊相鄰
						tmpmy++;//我方可擺放之空間
						if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){
							mysp++;//我方可擺放之邊角數
						}
					}
				}
				if(board[i][j]==0){
					if(board[i-1][j]!=enenum&&board[i+1][j]!=enenum&&board[i][j-1]!=enenum&&board[i][j+1]!=enenum){//此格不與自己方塊有邊相鄰
						tmpene++;//敵方可擺放之空間
						if(board[i-1][j-1]==enenum||board[i-1][j+1]==enenum||board[i+1][j-1]==enenum||board[i+1][j+1]==enenum){
							enesp++;//敵方可擺放之邊角數
						}
					}
				}
				//int tmpcon=0;//(計算放此方塊後跟對手碰到了多少塊)
				if(board[i][j]==enenum){//從對方的方塊角度去看(計算放此方塊後跟對手碰到了多少塊)
					if(board[i-1][j]==playernum||board[i+1][j]==playernum||board[i][j-1]==playernum||board[i][j+1]==playernum){
						tmpcon++;
					}
				}
				//int unique=0;//產生多少無敵角(我方可放敵方卻放不了)前期較重要
				if(board[i][j]==0){
					if(board[i-1][j]==enenum||board[i+1][j]==enenum||board[i][j-1]==enenum||board[i][j+1]==enenum){ //指少有一邊與敵方相連(敵方不能放)
						if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//4邊不予自己方塊相連
							if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){//4角至多一個相連
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
		//檢查此方塊造成多少效益
		for(i=1;i<=n;i++){//先試擺在棋盤上
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex];
		}
		for(i=1;i<=n;i++){//恢復原狀
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
			pl[playernum].bl[index].use=1;
		}
		//分回合探討
		if(round<=14)//前7回合
			total=1000+1.5*mysp-3*enesp+2*n+tmpmy-tmpene+tmpcon*1.2+4*unique;//設權重
		else if(round>=15&&round<=26){//8到13回合
			total=1000+1.5*mysp-3*enesp+1.5*n+tmpmy-tmpene+2*unique;//設權重
		}
		else {//
			total=1000+1.5*mysp-3*enesp+n+tmpmy-tmpene+unique;//設權重
		}
		return total;
	}
	else {
		//printf("要與自己的方塊有邊角相接!!!\n");
		return -1;
	}

}

int test_set2(int playernum,int index,int row,int col){//擺放方塊
	int i,j;
	int n = pl[playernum].bl[index].num;//方塊總數
	int movey = row-pl[playernum].bl[index].data[0].y;
	int movex = col-pl[playernum].bl[index].data[0].x;
	bool flag=false;//判斷是否有邊角相連
	for(i=1;i<=n;i++){ //判斷二維陣列的x y 不可為負不可超過棋盤
		int x=pl[playernum].bl[index].data[i].x+movex;
		int y=pl[playernum].bl[index].data[i].y+movey;
		if(pl[playernum].bl[index].use==0){
			//printf("此方塊已被使用!!!\n");
			return -1;
		}
		if(x<1||x>14||y<1||y>14){
			//printf("請放在棋盤內\n");
			return -1;
		}
		if(board[y][x]!=0) {
			//printf("重疊到了!\n");
			return -1;
		}
		if(board[y-1][x]==playernum||board[y+1][x]==playernum||board[y][x-1]==playernum||board[y][x+1]==playernum){//
			//printf("不合法與自己的方塊有邊相連!\n");
			return -1 ;
		}
		if(board[y-1][x-1]==playernum||board[y-1][x+1]==playernum||board[y+1][x-1]==playernum||board[y+1][x+1]==playernum){//與自己的方塊有邊角相接則true
			flag=true;
		}

	}
	/*	技巧:
		1.盡量跟別人方塊邊碰邊
		2.跟自己的方塊盡量不要空太多空間
		3.先看對手有那裡可以放就先放那
		4.若對手有某個地方放不了，則那裡先不用管他
		5.製造出無敵角，(我方放敵方放不了的角)
		6.若有確定可放的方塊在某位置(只有此方塊可放)，則先不去動此方塊及此空間
	*/
	int enenum;//用來計算對方空格數
	if(playernum==1){
		enenum=2;
	}
	else enenum=1;
	//計算我方/敵方可放之邊角數(ok)，放完後我方/敵方可放的空間(ok)，我方/敵方可放的方塊數
	//放此方塊與對手方塊有多少地方邊相鄰(ok)，放此方塊後產生多少無敵角(我方可放敵方卻放不了)(ok)、封住對手多少邊角(ok)
	int mysp=0,enesp=0,tmpmy=0,tmpene=0,unique=0,tmpcon=0;
	float total;
	if(flag==true||round<=2){ //前兩個回合無法與自己角相連
		pl[playernum].bl[index].use=0;
		for(i=1;i<=n;i++){//先試擺在棋盤上
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=playernum;
		}

		for(i=1;i<=14;i++){//開始計算
			for(j=1;j<=14;j++){
				if(board[i][j]==0){//此格為空可計入
					if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//此格不予自以方塊有邊相鄰
						tmpmy++;//我方可擺放之空間
						if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){
							mysp++;//我方可擺放之邊角數
						}
					}
				}
				if(board[i][j]==0){
					if(board[i-1][j]!=enenum&&board[i+1][j]!=enenum&&board[i][j-1]!=enenum&&board[i][j+1]!=enenum){//此格不與自己方塊有邊相鄰
						tmpene++;//敵方可擺放之空間
						if(board[i-1][j-1]==enenum||board[i-1][j+1]==enenum||board[i+1][j-1]==enenum||board[i+1][j+1]==enenum){
							enesp++;//敵方可擺放之邊角數
						}
					}
				}
				//int tmpcon=0;//(計算放此方塊後跟對手碰到了多少塊)
				if(board[i][j]==enenum){//從對方的方塊角度去看(計算放此方塊後跟對手碰到了多少塊)
					if(board[i-1][j]==playernum||board[i+1][j]==playernum||board[i][j-1]==playernum||board[i][j+1]==playernum){
						tmpcon++;
					}
				}
				//int unique=0;//產生多少無敵角(我方可放敵方卻放不了)前期較重要
				if(board[i][j]==0){
					if(board[i-1][j]==enenum||board[i+1][j]==enenum||board[i][j-1]==enenum||board[i][j+1]==enenum){ //指少有一邊與敵方相連(敵方不能放)
						if(board[i-1][j]!=playernum&&board[i+1][j]!=playernum&&board[i][j-1]!=playernum&&board[i][j+1]!=playernum){//4邊不予自己方塊相連
							if(board[i-1][j-1]==playernum||board[i-1][j+1]==playernum||board[i+1][j-1]==playernum||board[i+1][j+1]==playernum){//4角至多一個相連
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
		//檢查此方塊造成多使效益
		for(i=1;i<=n;i++){//先試擺在棋盤上
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex];
		}
		for(i=1;i<=n;i++){//恢復原狀
			board[pl[playernum].bl[index].data[i].y+movey][pl[playernum].bl[index].data[i].x+movex]=0;
			pl[playernum].bl[index].use=1;
		}
		//分回合探討
		if(round<=14)//前7回合
			total=1000+1.3*mysp-2*enesp+2*n+0.7*tmpmy-0.7*tmpene+1.2*tmpcon+3*unique;//設權重
		else if(round>=15&&round<=26){//8到13回合
			total=1000+1.2*mysp-1.5*enesp+1.6*n+0.6*tmpmy-0.6*tmpene+1.2*tmpcon+2*unique;//設權重
		}
		else {//
			total=1000+mysp-1.5*enesp+n+1.2*tmpmy-1.5*tmpene+1.2*tmpcon+2*unique;//設權重
		}
		return total;
	}
	else {
		//printf("要與自己的方塊有邊角相接!!!\n");
		return -1;
	}

}
int algo(int playernum,int method){//無法再放為傳-1，可以則回傳選則的方塊編號

	/*
	1.暴力法，在14*14棋盤上，試所有方塊及所有旋轉、上下之可能
	2.可以放之後開始計算空格，找最佳解
	3.可分回合探討，前5回合只能從第編號10之方塊後放 */
	if(round==1){//各個玩家得第一回合
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
	//紀錄:方塊編號,擺放位置,旋轉幾次,上下翻轉
	//計算(1)此方塊放完我方總共有幾個邊角可放 (2)對方總共有幾個邊角可放
	//(3) 放完後我方之後還有幾個方塊可放
	int tmpi,i,j,mir,ro;
	for(tmpi=21;tmpi>=1;tmpi--){//方塊編號
		if(pl[playernum].bl[tmpi].use==1){//方塊是否已用過
			for(mir=1;mir<=2;mir++){//進去迴圈兩次 上跟下
				for(ro=1;ro<=4;ro++){//4個方向(旋轉)

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

					rotate(playernum,tmpi);//轉完4次回到原本的
				}
				updown(playernum,tmpi);
			}

		}

	}

	if(max==-1){
		//printf("無法再放,PASS!!!\n");
		return -1;
	}
	else{
		pl[playernum].bl[tmpindex].use=0;

		//一定要先做上下翻轉才能旋轉，要跟上面一樣否則會錯
		for(i=1;i<=tmpmi;i++){
			updown(playernum,tmpindex);
		}
		for(i=1;i<=tmpro;i++){
			rotate(playernum,tmpindex);
		}

		int rw=tmprow-pl[playernum].bl[tmpindex].data[0].y;
		int cl=tmpcol-pl[playernum].bl[tmpindex].data[0].x;
		for(i=1;i<=pl[playernum].bl[tmpindex].num;i++){//直接擺在棋盤上
			board[pl[playernum].bl[tmpindex].data[i].y+rw][pl[playernum].bl[tmpindex].data[i].x+cl]=playernum;
		}
		//printf("玩家%d:選擇的是方塊 %d,旋轉%d次,上下翻轉 %d次，放在(%d,%d)\n",playernum,tmpindex,tmpro,tmpmi,tmprow,tmpcol);
		return tmpindex;
	}
}

void readfile(char *argv[]){//回傳玩家１或玩家２
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
	要擺放哪裡的演算法

1.考慮的點有: (1)此方塊放完我方總共有幾個邊角可放 (2)對方總共有幾個邊角可放
(3) 放完後我方之後還有幾個方塊可放
2.每個方塊可設權重，若遇到兩個方塊一樣則比權重
3.把所有自己的方塊可擺放之邊角位置都考慮進去
4.去檢查各方塊擺放完，自己還有多少地方可以放，及對手有多少地
方可擺
5.要考慮到旋轉及鏡射各情況
6.盡量去包圍對方的旗子
7.可考慮對方剩餘的方塊，去做應對
8.最後選出其中一個方塊


	讀檔寫檔

1.讀取:棋盤目前狀態，剩餘拼圖檔案，棋步紀錄檔案
2.寫入(更新):棋盤目前狀態檔案，剩餘拼圖檔案(不用更新棋步記錄檔案)

STEP:
1.讀目前棋盤狀態，剩餘方塊 ，做判斷下一步放哪
2.決定放哪後更新棋盤狀態，剩餘方塊

QUESTION
1.如何知道自己是第一個還是第二個玩家
2.一跟二分別代表甚麼顏色
3.只要更新自己所屬的剩餘方塊?
4.可讀取別人剩餘的方塊嗎


1.每回合先讀棋盤狀態及剩餘方塊之檔案
2.將ＮＲＢ轉換成０１２（第一位玩家視為紅色）
3.代入演算法決定下一步
4.更新棋盤狀態及自己的剩餘方塊
*/

