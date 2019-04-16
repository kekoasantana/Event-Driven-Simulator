#include <iostream>
#include "rv.h"
#include "event.h"

// Simulates an M/M/1 queueing system.  The simulation terminates
// once 200000 customers depart from the system.
int main()
{
  using namespace std;
  EventList Elist;                // Create event list
  enum {ARR,DEP};                 // Define the event types

  double lambda = 0.8;            // Arrival rate
  double mu = 3.0;                // Service rate jobs/second

  double clock = 0.0;             // System clock
  int N = 0;                      // Number of customers in system
  int Ndep = 0;                   // Number of departures from system
  double EN = 0.0;                // For calculating E[N]
  
  double block =0.0;              // Calculate how many events are blocked
  int K = 4;                      // Capacity of machine
  int M = 2;                      // Number of processors 
  int done = 0;                   // End condition satisfied?
  bool process1 = true;           // Processor 1 available?
  bool process2 = true;           // Processor 2 available?

  Event* CurrentEvent;

  Elist.insert(exp_rv(lambda),ARR); // Generate first arrival event

  while (!done)
  {
    CurrentEvent = Elist.get();               // Get next Event from list
    double prev = clock;                      // Store old clock value
    clock=CurrentEvent->time;                 // Update system clock 

    switch (CurrentEvent->type) {
    case ARR:                                 		// If arrival 
        EN += N*(clock-prev);						// update system statistics
        Elist.insert(clock+exp_rv(lambda),ARR);		// generate next arrival 
		if(N<K){                                    // check that 
			N++;                                    // update system size
        } else {
			block++;
		}			
        if (N==1) {                             // If this is the only customer
          Elist.insert(clock+exp_rv(mu),DEP);   // generate its departure event
        }
        // if both true, set process1 to false and run
        // if process1 is false, set process2 to false and run
        // if both false, wait until available
      break;
    case DEP:                                 // If departure
      EN += N*(clock-prev);                   // update system statistics
      N--;                                    // decrement system size
      Ndep++;                                 // increment num. of departures
      if (N > 0) {                            // If customers remain
        Elist.insert(clock+exp_rv(mu),DEP);   // generate next departure
      } 
      // if process 1 is false, set it to true
      // if process 2 is false, set it to true
      break;
    }
    delete CurrentEvent;
    if (Ndep > 200000) done=1;        // End condition
  }
  // output simulation results for N, E[N] 
  cout << "Current number of customers in system: " << N << endl;
  cout << "Expected number of customers (simulation): " << EN/clock << endl;
  cout << "Number of blocked events: " << block << endl;
  cout << "Probability of events blocked: " << (block/200000)*100 << endl;
  
  // output derived value for E[N]
  double rho = lambda/(mu*M); 
  cout << "Expected number of customers (analysis): " << rho/(1-rho) << endl;
} 
  
  