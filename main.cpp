#include <iostream>
#include "rv.h"
#include "event.h"

// Simulates an M/M/1 queueing system.  The simulation terminates
// once 200000 events depart from the system.
int main()
{
  EventList Elist;                                  // Create event list
  enum {ARR,DEP};                                   // Define the event types

  double lambda = 0.6;                              // Arrival rate
  double mu = 3.0;                                  // Service rate jobs/second

  double clock = 0.0;                               // System clock
  int N = 0;                                        // Number of events in system
  int Ndep = 0;                                     // Number of departures from system
  double EN = 0.0;                                  // For calculating E[N]
  
  double block =0.0;                                // Calculate how many events are blocked
  int K = 4;                                        // Capacity of machine
  int M = 2;                                        // Number of processors 
  int done = 0;                                     // End condition satisfied?
  int totalEvents = 200000;                         // Number of total events to hit done

  Event* CurrentEvent;

  Elist.insert(exp_rv(lambda),ARR);                 // Generate first arrival event

  while (!done)
  {
    CurrentEvent = Elist.get();                     // Get next Event from list
    double prev = clock;                            // Store old clock value
    clock=CurrentEvent->time;                       // Update system clock 
        
    switch (CurrentEvent->type) {                   // Check the type of event
    case ARR:                                 		// If arrival 
      EN += N*(clock-prev);						    // update system statistics
      if (N<K) {
        N++;                                        // Increment system size if not at capacity
      } else {
        block++;                                    // Block event if at capacity
      }
      if (M > 0 && M <=2) {                         // Check if available processor
        Elist.insert(clock+exp_rv(mu),DEP);         // Generate next departure
      }
      Elist.insert(clock+exp_rv(lambda),ARR);       // Generate next arrival
      M--;                                          // Decrease processor because generating arrival event
    break;
    case DEP:                                       // If departure
      Ndep++;                                       // Increase number of departures
      EN += N*(clock-prev);                         // update system statistics
      if (M < 2) {                                  // if processes is not at 2, increase processors
        M++;
      }
      if (N>0) {                                    // If there is an event, generate departure and decrease size
        N--;
        Elist.insert(clock+exp_rv(mu),DEP);
      }
      break;
    }
    delete CurrentEvent;
    if (Ndep > totalEvents) done=1;                 // End condition
  }
  // output simulation results for N, E[N] 
  std::cout << "Current number of customers in system: " << N << std::endl;
  std::cout << "Expected number of customers (simulation): " << EN/clock << std::endl;
  std::cout << "Number of blocked events: " << block << std::endl;
  std::cout << "Probability of events blocked: " << (block/totalEvents)*100 << "%" << std::endl;
  
  // output derived value for E[N]
  double rho = lambda/(mu*M); 
  std::cout << "Expected number of customers (analysis): " << rho/(1-rho) << std::endl;
} 
  
  