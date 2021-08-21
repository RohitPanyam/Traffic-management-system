// instructions
// 0 -- in Lanes means closed and 1 -- menas open Lane
// Road junctions are made up of three to four roads
// ecah road is one way road
// only un damaged vehicals are allowed on the road/Lane
// the burst and al times will be taken under the real simulation.
// emergrncy will be the only vechicels with the siren.


#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>



int stop;//if 1 then the critical section is acceceblr if 0 the its closed
int control; // the lanes will be bloced untill the eme-vehical has passed.
int indc[2]={0,1};//['Red','Green']
int No_lane; //number of lanes will be specifiens in main

typedef struct
{
    int serv_time; //time Quantum in seconds
    int eme; // emergency 0-no  1-yes
    int l_indc;
}Lane;

// struct Lane Lane_instance = { 1, 0, 0};


// #define init_lane(X) Lane X = {.serv_time = 1, .eme = 0, .l_indc =0}; 

void *Emergency(Lane *a[No_lane]);
void *Traffic_Baricade(Lane *b[No_lane]);
void *Schedule_service(Lane *c[No_lane]);


int main(){
    stop=1;
    control=1;
    printf("give no of lane: (3 or 4):\n");
    scanf("%d",&No_lane);
    // No_lane=3; //take some input req lanes
    Lane l[No_lane]; 
    for(int i=0;i<No_lane;i++){
        printf("give the input for server time: \n");
        scanf("%d",&l[i].serv_time);
        printf("give the input for eme [0-no,1-yes]: \n");
        scanf("%d",&l[i].eme);
        printf("give the input for lane_incicator [0-red,1-green]: \n");
        scanf("%d",&l[i].l_indc);
        
    }

    // for(int i=0;i<No_lane;i++){init_lane(l[i]);}
    l[1].l_indc=1;
    // l[0].eme=1;
    pthread_t px[No_lane];
    pthread_t px_t[No_lane];
    pthread_t px_S[No_lane];
    for(int i=0;i<No_lane;i++){
        
        if(l[i].l_indc==indc[0] && l[i].eme==1){
            pthread_create(&px[i],NULL,Emergency,&l);
            pthread_create(&px_t[i],NULL,Traffic_Baricade,&l);
            // Traffic_Baricade(&l);
        }
        else{
            pthread_create(&px_S[i],NULL,Schedule_service,&l);
            // Schedule_service(&l);
        }
    }
}

void swap(int* xp, int* yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void selectionSort(int arr[], int n) 
{ 
    int i, j, min_idx; 
    for (i = 0; i < n - 1; i++) {
        min_idx = i; 
        for (j = i + 1; j < n; j++) 
            if (arr[j] < arr[min_idx]) 
                min_idx = j;
        swap(&arr[min_idx], &arr[i]); 
    } 
}


void *Schedule_service(Lane *c[No_lane]){
    int n = No_lane;
    int r_serv_time[n];
    int STQ=0; //smart time quantum
    int r[No_lane];
    int sum={0};
    for(int i=0;i<No_lane;i++){
        r[i]=c[i]->serv_time;
    }
    selectionSort(r, n); 
    int h_rst=r[n-1];

    for (int i = 0; i < n; ++i) {
        sum += r[i];
    }

    int mean = sum / n;
    STQ=mean+h_rst;
    while(control);
    // while(stop);
    for(int j=0;j<n;j++){
        if(c[j]->l_indc==1){
            printf("the vehical enter the lane %d\n",j);
        }
        else{
            printf("the roads are shecheloded in the folloing order as the service time:\n");
            for(int i =0;i<n;i++){
                // printf("lane with sechedule time : %d\n",r[i]);
                while(STQ<r[i]){
                    printf("this road with this service time %d is scheduled\n",r[i]);
                    r_serv_time[i]=STQ-r[i];
                    if(r_serv_time[i]<=STQ){
                        printf("the same lane will be re-schelduled\n");
                    }
                }
            }
        }
    }
    return NULL;
}

void *Emergency(Lane *a[No_lane]){
    int n=No_lane;
    for(int i=0;i<n;i++){
        if(a[i]->eme==1){
            printf("there is an emergency vehical in lane %d .\n",i);
            stop=0;
            printf("the emergency vehical has been sent");
            stop=1;
        }
    }
    return NULL;

}


void *Traffic_Baricade(Lane *b[No_lane]){
    int n=No_lane;
    for(int i=0;i<n;i++){
        if(b[i]->l_indc==0 && b[i]->eme==1){
            control=0;
            while(stop);
            printf("eme-vechival has passe , baricades lifted.");
            control=1;
        }
    }
    return NULL;

}
