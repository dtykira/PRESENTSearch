#include "Search.h"
#include "Types.h"
#include "DesFunc.h"
#include "DiffDistribution.h"
#include "LookUpTables.h"
#include "GlobalV.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>

#include <cstdlib>
#include <queue>
#include <stack>
using namespace std;

int B[N]={0,-2,-4,-8,-12,-20,-24,-28,-32,-36,-41,-46,-52,-56,-62};

int rounds;
int B_n_bar;
int a[N+1][17]={0};
int p[N+1]={0};
u8 dx[N+1][17]={0};
u8 dy[N+1][17]={0};
FILE* stream;
int pr_cut[N+1][17];
int pr_whole;

bool done=0;


int coun=0;
struct trieNode{
	int pro;
	trieNode *son[PB];
};
trieNode* root=new trieNode();
int proba[N+1]={0};

inline void ResetCharacter(int k,int l,int round){
	for(int i=k+1;i<l;i++){
		dx[round][i]=0;
		dy[round][i]=0;
	}
}

void printAndSetBound(){
	B[rounds]=pr_whole;
	done=1;
	for(int r=1;r<=rounds;r++){
		fprintf(stream,"dx%d:",r);
		for(int i=1;i<=16;i++){
			fprintf(stream,"%x ",dx[r][i]);
		}
		fprintf(stream,"\ndy%d:",r);
		for(int i=1;i<=16;i++){
			fprintf(stream,"%x ",dy[r][i]);
		}
		fprintf(stream,"\tp%d:%d\n",r,p[r]);
	}
	fprintf(stream,"B_n_bar:%d\n==============\n",pr_whole);
}

void Round_N_(int j,int pr,int pr_round){
	if(done==1)return;
	if(dx[rounds][j]==0){
		dy[rounds][j]=0;
		if(j==16){
			if(pr_round==proba[rounds]){
				p[rounds]=pr_round;
				pr_whole=pr_round+pr;
				printAndSetBound();
			}
		}else{
			Round_N_(j+1,pr,pr_round);
		}
	}else{
		for(int freq=0,prob=-2,length;freq<=1;freq++,prob--){
			length=DDT_SearchInOrderWithFixedXLength[freq][dx[rounds][j]];
			if(length==0) continue;
			if((prob+pr_round+pr)<B_n_bar) break;
			for(int index=0;index<length;index++){
				dy[rounds][j]=DDT_SearchInOrderWithFixedX[freq][dx[rounds][j]][index];
				if(j==16){
					p[rounds]=prob+pr_round;
					if(p[rounds]==proba[rounds]){
						pr_whole=p[rounds]+pr;
						printAndSetBound();
					}
				}else{
					if((prob+pr_round)<proba[rounds]||(prob+pr_round)==(proba[rounds]+1)) break;
					Round_N_(j+1,pr,prob+pr_round);
				}
			}
			break;
		}
	}
}

void Round_(int i,int pr_former);
void Round__(int i,int j,int pr,int pr_round){
	if(done==1)return;
	if(dx[i][j]==0){
		dy[i][j]=0;
		if(j==16){
			if(pr_round==proba[i]){
				p[i]=pr_round;
				Round_(i+1,pr_round+pr);
			}
		}else{
			Round__(i,j+1,pr,pr_round);
		}
	}else{
		for(int freq=0,prob=-2,length;freq<=1;freq++,prob--){
			length=DDT_SearchInOrderWithFixedXLength[freq][dx[i][j]];
			if(length==0) continue;
			if((prob+pr_round+pr+pr_cut[i][j]+B[rounds-i])<B_n_bar) break;
			for(int index=0;index<length;index++){
				dy[i][j]=DDT_SearchInOrderWithFixedX[freq][dx[i][j]][index];
				if(j==16){
					p[i]=prob+pr_round;
					if(p[i]==proba[i]){
						Round_(i+1,p[i]+pr);
					}
				}else{
					if((prob+pr_round)<proba[i]||(prob+pr_round)==(proba[i]+1)) break;
					Round__(i,j+1,pr,prob+pr_round);
				}
			}
		}
	}
}


void Round_(int i,int pr_former){
	if(done==1)return;
	u64 x;
	word4_2_word64(&x,dy[i-1]+1);
	x=permutationTL(x);
	word64_2_word4(dx[i]+1,x);
	for(int k=15;k>=0;k--){
		if(dx[i][k+1]!=0){
			pr_cut[i][k]=pr_cut[i][k+1]-2;
		}else{
			pr_cut[i][k]=pr_cut[i][k+1];
		}
	}

	if(i==rounds){
		p[rounds]=0;
		Round_N_(1,pr_former,0);
	}else{
		Round__(i,1,pr_former,0);
	}
}

void Round_1_(int j,int pr_round){
	for(a[1][j]=a[1][j-1]+1;a[1][j]<=16;a[1][j]++){
		if(done==1)return;
		ResetCharacter(a[1][j-1],a[1][j],1);
		if((pr_round+(-2)+B[rounds-1])<B_n_bar) break;
		for(int y=1;y<16;y++){
			for(int freq=0,prob=-2,length;freq<=1;freq++,prob--){
				length=DDT_SearchInOrderWithFixedYLength[freq][y];
				if(length==0) continue;
				if((pr_round+prob+B[rounds-1])<B_n_bar) break;
				for(int index=0;index<length;index++){
					dy[1][a[1][j]]=y;
					dx[1][a[1][j]]=DDT_SearchInOrderWithFixedY[freq][y][index];
					p[1]=prob+pr_round;
					ResetCharacter(a[1][j],17,1);
					if(p[1]==proba[1]){
						Round_(2,p[1]);
					}
					if(a[1][j]!=16){
						if((prob+pr_round)<proba[1]||(prob+pr_round)==(proba[1]+1)) break;
						Round_1_(j+1,p[1]);
					}
				}
			}
		}
	}
}

void Round_1(){
	errno_t err;
	err=fopen_s(&stream,"fprint.txt","w");
	Round_1_(1,0);
	fclose(stream);
}



void search(int r){
	rounds=r;
	B_n_bar=B[r-1]+1;
	B[rounds]=1;
	do{
		B_n_bar-=1;
		Round_1();
	}
	while(B[rounds]!=B_n_bar);
}

void searchAll(){
	clock_t start,end;
	for(int i=3;i<35;i++){
		start=clock();
		done=0;
		search(i);
		end=clock();
		printf("%d轮搜索的结果为%d，搜索时间为%f\n",i,B[i],(double)(end-start)/CLK_TCK);
	}
}

void searchPattern(){
	//errno_t err;
	//err=fopen_s(&stream,"fprint.txt","w");
	Round_1_(1,0);
	//fclose(stream);
}

void insert(){
	trieNode *node=root;
	for(int i=1;i<=rounds;i++){
		if(node->son[-2-proba[i]]==NULL){
			node->son[-2-proba[i]]=new trieNode();
		}
		node=node->son[-2-proba[i]];
		node->pro=proba[i];
	}
}
bool flag=0;
void makeList(int i,int pr_former){
	if(done==1) return;
	for(int p=-2;p>=-1-PB;p--){
		if((pr_former+p+B[rounds-i])<B_n_bar) break;
		proba[i]=p;
		if(i==rounds){
			int temp;
			temp=0;
			for(int k=1;k<=rounds;k++){
				temp+=proba[k];
			}
			if(temp==B_n_bar){
				flag=0;
				for(int k=1;k<=rounds;k++){
					for(int l=3;l<(rounds-k+1);l++){
						temp=0;
						for(int m=k;m<=k+l-1;m++){
							temp+=proba[m];
						}
						if(temp>B[l]){
							flag=1;
							break;
						}
					}
					if(flag==1){
						break;
					}
				}
				if(flag==0){
					//insert();
					//coun++;
					searchPattern();
					if(done==1){
						for(int i=1;i<=rounds;i++){
							printf("%d ",proba[i]);
						}
						printf("\n");
					}
					done=0;
					
				}
				break;
			}
		}else{
			makeList(i+1,pr_former+p);
		}
	}
}

void trieBreadthSearch(){
	queue<trieNode*> nodeQueue;
	nodeQueue.push(root);
	trieNode* node;
	while(!nodeQueue.empty()){
		node=nodeQueue.front();
		printf("%d ",node->pro);
		nodeQueue.pop();
		for(int i=0;i<PB;i++){
			if(node->son[i]){
				nodeQueue.push(node->son[i]);
			}else break;
		}
	}
	printf("\n");
}

void trieDepthSearch(){
	stack<trieNode*> nodeStack;
	nodeStack.push(root);
	trieNode* node;
	while(!nodeStack.empty()){
		node=nodeStack.top();
		printf("%d ",node->pro);
		nodeStack.pop();
		for(int i=PB-1;i>=0;i--){
			if(node->son[i]){
				nodeStack.push(node->son[i]);
			}
		}
	}
	printf("\n");
}


void trieSearch(int i,trieNode* thisNode){
	trieNode* node;
	for(int j=0;j<PB;j++){
		if(thisNode->son[j]==NULL) continue;
		node=thisNode->son[j];
		proba[i]=node->pro;
		if(i==rounds){
			for(int i=1;i<=rounds;i++){
				fprintf(stream,"%d ",proba[i]);
			}
			fprintf(stream,"\n");
			coun++;
		}else{
			trieSearch(i+1,node);
		}
	}
}

void make(int r,int b){
	errno_t err;
	err=fopen_s(&stream,"fprint.txt","w");

	rounds=r;
	B_n_bar=b;
	makeList(1,0);
	//fprintf(stream,"\n");
	//trieBreadthSearch();
	//trieDepthSearch();
	//trieSearch(1,root);
	//printf("%d",coun);

	fclose(stream);
}

void searchByMaking(int r){
	rounds=r;
	int b=B[r-1]+1;
	B[rounds]=1;
	do{
		B_n_bar-=1;
		make(r,b);
	}
	while(done==0);
}