//
//  main.c
//  kureung
//
//  Created by Han Chaeyeon on 2021/06/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define Threshold 0 // 밀집도 임계값 (바꿀수있음)

//점을 나타내는 구조체 정의 시작
typedef struct point{
double x;
double y;
}point;
//점을 나타내는 구조체 정의 끝

// 정류장의 정보를 담는 구조체 정의 시작 (틀이라고 생각하면 쉬움)
typedef struct bike{
    int num; // 대여소 번호
    point pt; // 위도 경도 위치값
    int sum;  //거치대 총 개수
    
}bike;

// 정류장의 정보를 담는 구조체 정의 끝

//첫번째 함수 시작 : csv 파일의 총 line 수를 세는 함수 : 정류장의 총 개수나 data 의 총 개수를 세는 데 사용됨. 굳이 필요는 없으나, 구조체 배열을 size 없이 동적 할당하기 어려워서 만든 함수...
int totalline (char *name){
    FILE *fp;
    int line =0;
    char c;
    fp=fopen(name,"r");
    while((c=fgetc(fp))!=EOF)
        if(c=='\n') line++;
            fclose(fp);
    return (line);
    
} // 첫번째 함수 끝


//두번째 함수 시작
double dense (bike a, int time){ // 사용자가 입력한 정류장의 정보와 사용자가 입력한 시간을 불러와 함수를 실행
    int n,m,l;
    char *nameout= "data.csv";
    
    int size;
    size=totalline("data.csv"); // data 파일의 총 line 수를 셈 (첫번째 함수 활용)

    int i=0;
    int minus=0;
    int plus=0;
    
    FILE *fp=NULL;
    fp=fopen(nameout,"r"); //data 파일 불러옴
    for(i=0;i<size;i++){ // 반복문 : data 파일의 첫번째 행부터 마지막 행까지 다음 구문을 반복함
        fscanf(fp,"%d,%d,%d",&n,&m,&l); // 각 행의 데이터 : (출발한 대여소 번호, 도착한 대여소 번호, 시간대)
        if((n==a.num)&&(l==time)) // 만약 출발한 대여소의 번호와 시간대가 모두 일치하는 행이 있으면
        {minus++;} // 유출량을 1만큼 증가시키고
        if((m==a.num)&&(l==time)) // 만약 도착한 대여소의 번호와 시간대가 모두 일치하는 행이 있으면
        {plus++;}} // 유입량을 1만큼 증가시킨다
    
    fclose(fp);
    
    printf("%d시 해당 정류장의 평균 유입량은 %d, 평균 유출량은 %d입니다\n",time, plus, minus);
    printf("해당 정류장의 거치대 수는 %d 대이며,",a.sum);
   
        int delta;
        delta= plus - minus; //delta를 유입량에서 유출량을 뺀 값으로 정의함
        double dense;
        dense= (double) delta/ (double) a.sum; // delta를 총 거치대 수로 나누어 dense를 구함
    return dense; //dense를 출력
}// 두번째 함수 끝

//세번재 함수 시작 : 두 점 사이의 거리를 구하는 함수 (지구는 구면이라 원래 삼각함수써야하지만 귀찮으니 평면으로 근사~)
double dist (point a, point b){
    double xdiff, ydiff, dis;
    
    xdiff=a.x-b.x; // x좌표 변화량
    ydiff=a.y-b.y; // y좌표 변화량
    
    dis= sqrt((xdiff*xdiff)+(ydiff*ydiff)); // 점과 점사이의 거리 구하는 공식~
    return dis; // 거리 출력
    
}
//세번째 함수 끝

//네번째 함수 시작 : 근처 가장 가까운 대여소를 추천해주는 함수

void recc (bike *a,int k, int size){ //전체 정류소 정보를 담은 구조체 배열, 사용자가 입력한 정류소에 해당하는 인덱스 번호, 정류소 총 개수를 매개변수로 함
    double n=1;
    int min;
    double r;
    int i=0;
    
    for(i=0; i<size; i++) // 모든 정류소 반복
    {
        r=dist(a[k].pt, a[i].pt); // 지금 처리하고 있는 정류소와 사용자가 입력한 정류소 사이의 거리 구하기
        if ((r<n)&& (k!=i))// 방금 구한 거리가 지금 저장되어있는 거리보다 작으면
        {min = a[i].num; // 최소값을 지금 처리하고 있는 정류소의 번호로 바꾸고
            n=r; // 저장되었는 거리에 방금 구한 거리를 덮어씌우기
        }
    }
    
    //결국 모든 대여소와 사용자가 입력한 대여소 사이의거리를 비교한 후에 최소값을 구하는 알고리즘이라고 이해하시면 됩니다
    
    //네번째 함수 끝 : 반복문 모르면 제가 위에다 뭐라고 썼는지 이해가 안되실수도 있지만..ㅠㅠ 파이썬이나 알에도 비슷한게 분명 나올겁니당..
    
    printf("가장 가까운 추천 정류장 : %d \n", min); // 최종적으로 선정된 정류장 출력
    
    //printf("거리 : %d \n",k); // 그 최단거리를 출력 : 그냥 보여주기용.. 단위도 안맞음
    
}

//네번째 함수 끝

// main 시작

int main(int argc, const char * argv[]) {

    int numb;
    int time;
    
    printf("대여소 번호를 입력하세요 : ");
    scanf("%d",&numb); // 사용자로부터 대여소 번호를 받고
    
    printf("시간을 입력하세요 : ");
    scanf("%d",&time); // 사용자로부터 원하는 시간대를 0~24 폼으로 받습니다
    
    int size;
    size=totalline("inform.csv"); // 총 정류장 개수를 구하고
    
    bike *arr;
    arr = (bike*)malloc(size*sizeof(bike)); //동적할당 (몰라도됨)
    
    FILE*fp=NULL;
    char *namein= "inform.csv";
    fp=fopen(namein,"r"); // 정류장 파일 불러오기
    
    
    int i=0;

    int k;
    for(i=0; i<size ;i++){ // 모든 정류장을 대상으로 반복문
        
        fscanf(fp,"%d,%lf,%lf,%d",&arr[i].num,&arr[i].pt.x,&arr[i].pt.y,&arr[i].sum);
        if (numb==(arr[i].num)) //사용자가 입력한 정류소 번호랑 지금 처리하고 있는 정류소 번호가 같으면
        {k=i;} // k라는 변수에 배열 번호를 대입
        
    }

    free(arr); //메모리반납
    fclose(fp); //파일닫기
    

    double d;
    d=dense(arr[k],time); //밀집도 계산 : 아까 앞에서 정의한 두번째 함수 이용 (매개변수는 사용자가 입력한 정류소 번호에 해당하는 정류소 정보, 그리고 사용자가 입력한 시간대)
    
    printf("%d시 %d 정류장의 밀집도는 %lf 입니다. \n", time, numb, d); // 밀집도 출력
    
    if (d<Threshold){ // 정류소에 자전거가 없으면
      
        printf("추천 정류장은 : %d \n",numb);} // 사용자가 입력한 정류소 추천
        
        if (d>=Threshold){ // 만약 밀집도가 임계값보다 크면, 즉 대여소에 자전거가 없을 확률이 크면
    
            recc(arr, k, size); // 근처 다른 정류소 추천 (네번째 함수 이용)
        }
}
