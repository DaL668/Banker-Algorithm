#include "banker.h"

int main(int argc, char* argv[]) {
	if (argc!=(NUMBER_OF_RESOURCES+1)) {
                cerr<<"Exact number of arguments required please."<<endl;
                exit(1);
        }
	//initial status
	initAvailable(argc, argv);
	initMax();
	initAlloc();
	initNeed();
	initRequest();
	initRelease();
	initSequence();
	printResult();
	if (isSafe()==false) {
		cout<<"No safe sequence found. System is unsafe."<<endl;
		return -1;
	}
	printSequence();
	//requesting resources
	pthread_mutex_init(&mutex, NULL); //initilize lock
	pthread_attr_t attr;
        pthread_attr_init(&attr);
	
	/*------------------------------------*/
	//process 4
        pthread_t t5;
        myarg processFifth;
        myarg* process4=&processFifth;
        process4->cus_num=4;
        process4->p=myrequest;
        process4->q=myrelease;
        int succ5=pthread_create(&t5, &attr, c1, process4);
        if (succ5!=0) {
                cerr<<"thread creating failed."<<endl;
        }
	//process 3
        pthread_t t4;
        myarg processFourth;
        myarg* process3=&processFourth;
        process3->cus_num=3;
        process3->p=myrequest;
        process3->q=myrelease;
        int succ4=pthread_create(&t4, &attr, c1, process3);
        if (succ4!=0) {
                cerr<<"thread creating failed."<<endl;
        }
	/*---------------------------------------------*/
	//process 2
        pthread_t t3;
        myarg processThird;
        myarg* process2=&processThird;
        process2->cus_num=2;
        process2->p=myrequest;
        process2->q=myrelease;
        int succ3=pthread_create(&t3, &attr, c1, process2);
        if (succ3!=0) {
                cerr<<"thread creating failed."<<endl;
        }
	//process 1
	pthread_t t2;	
	myarg processSecond;
	myarg* process1=&processSecond;
	process1->cus_num=1;
        process1->p=myrequest;
	process1->q=myrelease;
	int succ2=pthread_create(&t2, &attr, c1, process1);
        if (succ2!=0) {
                cerr<<"thread creating failed."<<endl;
        }
	//process 0
	pthread_t t1;
	myarg processFirst;
	myarg* process0=&processFirst;
	process0->cus_num=0;
	process0->p=myrequest;
	process0->q=myrelease;
	int succ1=pthread_create(&t1, &attr, c1, process0);
	if (succ1!=0) {
	       	cerr<<"thread creating failed."<<endl;
	}
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_join(t5, NULL);
	printResult();
	printSequence(); //print safe sequence
	return 0;
}
