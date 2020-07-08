//
//  main.c
//  EinsteinProblem
//
//  Created by ParkZoo on 2020/05/10.
//  Copyright © 2020 박주형. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define DECLARE_SYMBOL(category, num, n1, n2, n3, n4, n5) \
enum tag##category { n1,n2,n3,n4,n5 }; \
const char *str##category[5]={ #n1, #n2, #n3, #n4, #n5 };

//category를 enumerated type으로 열거
//각 category Name을 가지고 str+Category 조합, 각 카테고리 별 요소 값에 접근 가능.

#define NATION 0
#define HOUSE_COLOR 1
#define BEVERAGE 2
#define PET 3
#define CIGAR 4

DECLARE_SYMBOL(NATION, 0,BRIT, DANE, GERMAN, NORWEIGN, SWEDE)
DECLARE_SYMBOL(HOUSE_COLOR, 1, BLUE, GREEN, RED, WHITE, YELLOW)
DECLARE_SYMBOL(BEVERAGE, 2, BEER, COFFEE, MILK, TEA, WATER)
DECLARE_SYMBOL(PET, 3, CAT, BIRD, DOG, FISH, HORSE)
DECLARE_SYMBOL(CIGAR,  4, BLUEMASTER, DUNHILL, PALLMALL, PRINCE, BLEND)


int total_matrix[120][5]; // 5! = 120

//permutation
void init_matrix(const int *allo, int level)
{
    static int count=0;
    static int making[5];
    int tmp[5]={0,};
    int i=0;
    
    if(level==5) {
        for(i=0;i<5;i++)
            total_matrix[count][i]=making[i];
        count++;
        return;
    }
    
    for(i=0;i<5;i++)
        tmp[i]=allo[i];
    
    for(i=0;i<5;i++){
        if(!allo[i]) {
            tmp[i]=1; making[level]=i;
            init_matrix(tmp, level+1);
            tmp[i]=0;
        }
    }
}


//5개의 집 선언, 해당 집이 가지는 category요소 들을 저장할 index배열를 가지고 있음.
typedef struct{
    int index[5];
}HOUSE;

HOUSE house[5];

//해당 카테고리의 요소 값을 확인하게 해줄 함수.
const HOUSE *search(int category, int value){
    int i;
    for(i=0;i<5;i++) if(house[i].index[category]==value) return &house[i];
    return NULL;
}


void print_result(int nodes){
    int i;
    for(i=0;i<5;i++){
        printf("House %d: ", i+1);
        printf("%s ",strNATION[house[i].index[0]]);
        printf("%s ",strHOUSE_COLOR[house[i].index[1]]);
        printf("%s ",strBEVERAGE[house[i].index[2]]);
        printf("%s ",strPET[house[i].index[3]]);
        printf("%s ",strCIGAR[house[i].index[4]]);
        printf("\n");
    }
    
    printf("%d nodes\n", nodes);
}


void promising(const int *sequnce, int level){
    int i;
    
    //레벨은 4까지 있기떄문. 5까지 가는 것은 함수 종료.
    if(level==5) return;
    
    //promising을 확인할 level의 해당 sequnece(0~4의 조합) 배열을 각 하우스의 탐색 level에 대입.
    for(i=0;i<5;i++)
        house[i].index[level]=sequnce[i];
    
    switch(level){ //각 level(집색, 국적, 음료, 담배, 애완동물) 당 Hint을 swith case 문으로 입력.
        case HOUSE_COLOR:
            // The Brit lives in a red house.
            if(search(NATION, BRIT)->index[HOUSE_COLOR] != RED)
                return;
            //The green house is on the left of the white house (next to it).
            if(!((house[2].index[HOUSE_COLOR] == GREEN && house[3].index[HOUSE_COLOR]==WHITE) || (house[3].index[HOUSE_COLOR]==GREEN && house[4].index[HOUSE_COLOR]==WHITE)))
                return;
            //The Norwegian lives next to the blue house.
            if(house[1].index[HOUSE_COLOR] != BLUE)
                return;
            break;
            
        case NATION:
            //The Norwegian lives in the first house.
            if(house[0].index[NATION]!=NORWEIGN)
                return;
            break;
            
        case BEVERAGE:
            //The Dane drinks tea.
            if(search(NATION, DANE)->index[BEVERAGE]!=TEA)
                return;
            //The green house owner drinks coffee.
            if(search(HOUSE_COLOR, GREEN)->index[BEVERAGE]!=COFFEE)
                return;
            //The man living in the house right in the center drinks milk.
            if(house[2].index[BEVERAGE]!=MILK)
                return;
            break;
            
        case CIGAR:
            //The person who smokes Pall Mall rears birds.
            if(search(CIGAR, PALLMALL)->index[PET]!=BIRD)
                return;
            //The owner of the yellow house smokes Dunhill.
            if(search(HOUSE_COLOR, YELLOW)->index[CIGAR]!=DUNHILL)
                return;
            // The man who smokes Blend lives next to the one who keeps cats.
            if(pow(search(CIGAR, BLEND)-search(PET, CAT),2)!= 1)
                return;     //옆집일 경우 house번호의 차이가 1 or -1이기 때문.
            //The man who keeps horses lives next to the man who smokes Dunhill.
            if(pow(search(CIGAR, DUNHILL)-search(PET, HORSE),2)!=1)
                return;
            //The owner who smokes Blue Master drinks beer.
            if(search(CIGAR, BLUEMASTER)->index[BEVERAGE]!=BEER)
                return;
            // The German smokes Prince.
            if(search(NATION, GERMAN)->index[CIGAR]!=PRINCE)
                return;
            // The man who smokes Blend has a neighbor who drinks water.
            if(pow(search(CIGAR, BLEND)-search(BEVERAGE, WATER),2)!=1)
                return;
            break;
            
        case PET:
            // The Swede keeps dogs as pets.
            if(search(NATION, SWEDE)->index[PET]!=DOG)
                return;
            break;
    }
    
    //다음 레벨 확인.
    for(i=0;i<120;i++) promising(total_matrix[i], level+1);
    
}


int main() {
    int declare[5]={0,};
    int i=0;
    
    init_matrix(declare, 0);
    
    for(i=0;i<120;i++)
        promising(total_matrix[i],0);
    
    return 0;
}

