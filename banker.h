#ifndef __banker_h__
#define __banker_h__
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#define NUMBER_OF_CUSTOMERS 5 
#define NUMBER_OF_RESOURCES 3 
using namespace std;

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int myrequest[NUMBER_OF_RESOURCES];
int myrelease[NUMBER_OF_RESOURCES];
int safeSequence[NUMBER_OF_CUSTOMERS];
struct myarg {
	int* p;
	int* q;
	int cus_num;
	//int reType;
};		
//mutex to prevent race conditions
pthread_mutex_t mutex;
//Safety Algorithm
bool isSafe();	
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
void initAvailable(int argc, char* argv[]);
void initMax();
void initAlloc();
void initNeed();
void initRequest();
void initRelease();
void initSequence();
void printAvailable();
void printNeed();
void printAllocation();
void printMax();
void printResult();
void printSequence();
//fucntions reqeusting and releaseing resources
void *c1(void *arg);
void *c2(void *arg);


void initAvailable(int argc, char* arg[]) {
	for (int i=0;i<argc-1;i++) {
                available[i]=stoi(arg[i+1]);
        }
}

void initMax() {
	time_t t;
        srand((unsigned) time(&t));
	for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
		for (int j=0;j<NUMBER_OF_RESOURCES;j++) {
			maximum[i][j]=rand()%15;
			while ((maximum[i][j]>available[j])||(maximum[i][j]==0)) {
				maximum[i][j]=rand()%15;
			//max needs can't be greater than available
			}
		}
	}
}

void initAlloc() {
	int count=0;
	for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
           	for (int j=0;j<NUMBER_OF_RESOURCES;j++) {
			if (available[j]>=2) {
				allocation[i][j]=2;
				if (allocation[i][j]>maximum[i][j]) {
					allocation[i][j]=maximum[i][j];
					available[j]-=maximum[i][j];
				}
				else {
					available[j]-=2;
				}
			}
			else {
				allocation[i][j]=0;
			}		
		}
        }
}

void initNeed() {
	for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
                for (int j=0;j<NUMBER_OF_RESOURCES;j++) {
                     	need[i][j]=maximum[i][j]-allocation[i][j];
                }
        }
}

void initRequest() {
        for (int i=0; i<NUMBER_OF_RESOURCES;i++) {
               myrequest[i]=rand()%4;
        }
}

void initRelease() {
	for (int i=0; i<NUMBER_OF_RESOURCES;i++) { 
          	myrelease[i]=rand()%4;
	}
}


void printResult() {
	char r='A';
	cout<<"Available resources now"<<endl;
        for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
		r+=i;
		cout<<r<<" ";
                cout<<available[i]<<"  ";
		r='A';
        }
	cout<<endl;
	cout<<"current maximum demand:"<<endl;
        for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
                cout<<"process "<<i<<"'s max demand: ";
                for (int j=0;j<NUMBER_OF_RESOURCES;j++) {
                        cout<<maximum[i][j]<<" ";
                }
                cout<<endl;
	}
	cout<<"current allocation:"<<endl;
        for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
                cout<<"process "<<i<<"'s allocation: ";
                for (int j=0;j<NUMBER_OF_RESOURCES;j++) {
                        cout<<allocation[i][j]<<" ";
                }
                cout<<endl;
	}
	cout<<"current need:"<<endl;
        for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
                cout<<"process "<<i<<"'s need: ";
                for (int j=0;j<NUMBER_OF_RESOURCES;j++) {
                        cout<<need[i][j]<<" ";
                }
                cout<<endl;
        }
}

void printSequence() {
	int count=0;
	 cout<<"Safe sequence: ";
	for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
		cout<<safeSequence[i]<<" ";
		if (safeSequence[i]==-1) {
			count++;
		}
	}
	 cout<<endl;
	 if (count==NUMBER_OF_CUSTOMERS) {
		 cout<<"No safe sequence found. System is unsafe."<<endl;
	 }
}

int request_resources(int customer_num, int request[]) {
//reqeust exceeds need
	char r='A';
for (size_t i=0;i<NUMBER_OF_RESOURCES;i++) {
	if ((request[i]>need[customer_num][i])) {
		cout<<"process "<<customer_num<<" request";
		r+=i;
                cout<<" resource type "<<r<<" of ";
                cout<<request[i];
                cout<<". It exceeds actual need by "<<"process ";
                cout<<customer_num<<endl;
		return -1;
	}
}
//request not available
		for (size_t i=0;i<NUMBER_OF_RESOURCES;i++) {
			if (request[i]>available[i]) {
				cout<<"process "<<customer_num<<" request";
				r+=i;
                        	cout<<" resource type "<<r<<" of ";
                        	cout<<request[i];
                        	cout<<". It is not available yet to ";
                        	cout<<"process "<<customer_num<<endl;
				return -1;
			}
		}
//pretend resource granted
		for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
			available[i]-=request[i];
			allocation[customer_num][i]+=request[i];
			need[customer_num][i]-=request[i];
		}
//determine if safe
		if (isSafe()==true) {
			cout<<"process "<<customer_num<<" request";
			cout<<" resources ";
			for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
				r+=i;
				cout<<r<<" ";
				cout<<request[i]<<" ";
				r='A';
			}
			cout<<". It is "<<"allocated to "<<"process ";
			cout<<customer_num<<endl;
			return 0;
		}
		else {//not safe
			for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
				available[i]+=request[i];
                        	allocation[customer_num][i]-=request[i];
                        	need[customer_num][i]+=request[i];
			}
			cout<<"process "<<customer_num<<" request";
			cout<<" resources: ";
                        for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
				r+=i;
				cout<<r<<" ";
                                cout<<request[i]<<" ";
				r='A';
                        }
                       	cout<<". It is available but ";
			cout<<"system is unsafe and fails "<<"process ";
                      	cout<<customer_num<<endl;
			return -1;
		}
}

int release_resources(int customer_num, int release[]) {
	for (size_t i=0;i<NUMBER_OF_RESOURCES;i++) {
		if ((release[i]>allocation[customer_num][i])||(need[customer_num][i]!=0)) {
			cout<<"release of resource "<<i;
        		cout<<" by customer "<<customer_num<<" failed ";
			cout<<endl;
			return -1;
			}
	}
	for (size_t i=0;i<NUMBER_OF_RESOURCES;i++) {
		available[i]+=release[i];
             	allocation[customer_num][i]-=release[i];
    		need[customer_num][i]+=release[i];
	}
	cout<<"release of resource type ";
	for (size_t i=0;i<NUMBER_OF_RESOURCES;i++) {
		cout<<i<<" of "<<release[i]<<" ";
	}
	cout<<" "<<customer_num<<" successful.";
	cout<<endl;
	return 0;
}

bool isSafe() {
	bool isSafe=true;
	int work[NUMBER_OF_RESOURCES];
	int count=0;
	int restore_work[NUMBER_OF_RESOURCES];
	int safeindex=0;
	//initialize work to available
	for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
		work[i]=available[i];
	}

	for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
                restore_work[i]=work[i];
        }

	//initialize finish
	bool Finish[NUMBER_OF_CUSTOMERS];
	for (int i=0; i<NUMBER_OF_CUSTOMERS;i++) {
		Finish[i]=false;
	}
	//step2and3
	for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
		if (Finish[i]==false) {
			for (int j=0; j<NUMBER_OF_RESOURCES; j++) {
		       		if (need[i][j]<=restore_work[j]) {	
					restore_work[j]+=allocation[i][j];
					count++;
				}
			}
		}
		if (count==NUMBER_OF_RESOURCES) {
                   	Finish[i]=true;
			for (int k=0;k<NUMBER_OF_RESOURCES;k++) {
				work[k]=restore_work[k];
			}
			safeSequence[safeindex++]=i;
             	}
		for (int k=0;k<NUMBER_OF_RESOURCES;k++) {
                       	restore_work[k]=work[k];
              	}
		count=0;
	}
	//check back failed requests 
	for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
		if (Finish[i]==false) {
			for (int j=0; j<NUMBER_OF_RESOURCES; j++) {
		       		if (need[i][j]<=restore_work[j]) {	
					restore_work[j]+=allocation[i][j];
					count++;
				}
			}
		}
		if (count==NUMBER_OF_RESOURCES) {
                   	Finish[i]=true;
			for (int k=0;k<NUMBER_OF_RESOURCES;k++) {
				work[k]=restore_work[k];
			}
			safeSequence[safeindex++]=i;
             	}
		for (int k=0;k<NUMBER_OF_RESOURCES;k++) {
                      	restore_work[k]=work[k];
              	}
		count=0;
	}
	//step4 no finish==false and need<work exists
	for (int i=0;i<NUMBER_OF_RESOURCES;i++) {
		if (Finish[i]==false) {
			isSafe=false;
			break;
		}
	}
	return isSafe;
}

void *c1(void *arg) {
        pthread_mutex_lock(&mutex);
        /*critical section*/
	initRequest();
	int i=request_resources(((myarg*)arg)->cus_num, ((myarg*)arg)->p);
	initRelease();
	int j=release_resources(((myarg*)arg)->cus_num, ((myarg*)arg)->q);
        pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

/*
void *c2(void *arg) {
        pthread_mutex_lock(&mutex);
        //critical section
	int i=request_resources(((myarg*)arg)->cus_num, ((myarg*)arg)->p);
	int* myp=((myarg*)arg)->p;
	int j=release_resources(((myarg*)arg)->cus_num, ((myarg*)arg)->q);
        pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}*/

void initSequence() {
	for (int i=0;i<NUMBER_OF_CUSTOMERS;i++) {
		safeSequence[i]=-1;
	}
}
#endif
