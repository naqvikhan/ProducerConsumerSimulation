#include <iostream>

#include <pthread.h>

#include <semaphore.h>

#include <sys/stat.h>

#include <fcntl.h>

using namespace std;

// Function Prototypes
void * Producer(void * );
void * Consumer(void * );
void Consume(double, double & );
void addTrackerHead(double);
double Produce();
double randomNumberGenerator();
double addTrackerTail();

// Global Variables and Semaphores initialized
double Array[1000000];
double ConsumerSums[10];
int head = 0, tail = 0, ProducerCount = 0, ConsumerCount = 0;
bool ProductionComplete = false, ConsumptionComplete = false;
sem_t * n;
sem_t * s;
int main() {

    // Declare producer and consumer threads
    pthread_t ProducerThread;
    pthread_t ConsumerThread1, ConsumerThread2, ConsumerThread3,
    ConsumerThread4, ConsumerThread5, ConsumerThread6,
    ConsumerThread7, ConsumerThread8, ConsumerThread9,
    ConsumerThread10;


    sem_unlink("SemN");
    sem_unlink("SemS"); // Free up the semaphore names
    unsigned int SemNValue = 0;
    unsigned int SemSValue = 1; // declare values for semaphores


    n = sem_open("SemN", O_CREAT, 0644, SemNValue);
    s = sem_open("SemS", O_CREAT, 0644, SemSValue); // Initialize semaphores

    // Create Producer Thread
    pthread_create( & ProducerThread, NULL, Producer, NULL);
    cout << endl << "Producer thread created" << endl;

    // Create Consumer threads
    pthread_create( & ConsumerThread1, NULL, Consumer, (void * ) & ConsumerSums[0]);
    cout << "Consumer thread  1 created" << endl;
    pthread_create( & ConsumerThread2, NULL, Consumer, (void * ) & ConsumerSums[1]);
    cout << "Consumer thread  2 created" << endl;
    pthread_create( & ConsumerThread3, NULL, Consumer, (void * ) & ConsumerSums[2]);
    cout << "Consumer thread  3 created" << endl;
    pthread_create( & ConsumerThread4, NULL, Consumer, (void * ) & ConsumerSums[3]);
    pthread_create( & ConsumerThread5, NULL, Consumer, (void * ) & ConsumerSums[4]);
    pthread_create( & ConsumerThread6, NULL, Consumer, (void * ) & ConsumerSums[5]);
    pthread_create( & ConsumerThread7, NULL, Consumer, (void * ) & ConsumerSums[6]);
    pthread_create( & ConsumerThread8, NULL, Consumer, (void * ) & ConsumerSums[7]);
    pthread_create( & ConsumerThread9, NULL, Consumer, (void * ) & ConsumerSums[8]);
    pthread_create( & ConsumerThread10, NULL, Consumer, (void * ) & ConsumerSums[9]);
    cout << "            *              " << endl;
    cout << "            *              " << endl;
    cout << "            *              " << endl;
    cout << "Consumer thread 10 created" << endl << endl;


    // Make the threads wait for eachother
    pthread_join(ProducerThread, NULL);
    pthread_join(ConsumerThread1, NULL);
    pthread_join(ConsumerThread2, NULL);
    pthread_join(ConsumerThread3, NULL);
    pthread_join(ConsumerThread4, NULL);
    pthread_join(ConsumerThread5, NULL);
    pthread_join(ConsumerThread6, NULL);
    pthread_join(ConsumerThread7, NULL);
    pthread_join(ConsumerThread8, NULL);
    pthread_join(ConsumerThread9, NULL);
    pthread_join(ConsumerThread10, NULL);

    sem_close(n);
    sem_close(s); // Clean up semaphores

    pthread_exit(NULL);

    return 0;
}


void * Producer(void * empty) { // Prodcer program to creat and add number to queue

    while (ProductionComplete == false) { // Keep looping until production is complete

        double x = Produce(); // generate random numbers >= 0 
        sem_wait(s); // locks queue
        addTrackerHead(x); // add number to queue
        sem_post(s); // unlocks buffer
        sem_post(n);

    }
    // All numbers produced
    cout << "----------- Production is complete. ------------" << endl << endl;

    pthread_exit(NULL);
}


double Produce() {

    double sum = 0;

    for (int i = 0; i < 1000; ++i) { // loop 1000 times
        sum += randomNumberGenerator(); // add generated number to sum
    }

    sum = (sum / 500); // divide sum by 5000

    ProducerCount++; // keep track of production

    if (ProducerCount == 1000000) { // Check for the completion of production
        ProductionComplete = true;
    }

    return sum; // return sum of numbers calculated


}


void * Consumer(void * TOTAL) {

    // Declare variables to hold total
    double sum = 0;
    double * total = (double * ) TOTAL;

    while (ConsumptionComplete == false) { // loop until consumption is complete

        sem_wait(n);
        sem_wait(s); // lock queue
        double x = addTrackerTail(); // read number from queue
        sem_post(s); // Unlock queue
        Consume(x, sum); // Call consume to add number read from queue to total
        * total += sum; // add sum to total

    }

    cout << "----------- Consumption is complete. -----------" << endl << endl;

    for (int m = 0; m < 10; m++) { // Print partial totals
        cout << "Items consumed by Consumer Thread " << m + 1 << ": " << ConsumerSums[m] << endl;
    }

    exit(0);

}



void Consume(double ConsumerSum, double & ConsumerTotal) {

    if (ConsumerSum == -1) { // if buffer is empty do not add anything
        ConsumerTotal = 0 + ConsumerTotal;
    } else {
        ConsumerTotal = ConsumerSum + ConsumerTotal; // add sum to total
    }

}


double randomNumberGenerator() {

    double randomNumber = 0;

    randomNumber = (rand() / ((double) RAND_MAX + 1)); // generate random number

    return randomNumber;

}


void addTrackerHead(double sum) {

    Array[head] = sum;
    head++;
}


double addTrackerTail() {

    if (head == tail) { // check if buffer is empty

        cout << "The Buffer is empty." << endl;
        return -1;
    }

    double next = Array[tail];
    ConsumerCount++;

    if (ConsumerCount == 1000000) {
        ConsumptionComplete = true;
    }
    tail++;

    return next;
}
