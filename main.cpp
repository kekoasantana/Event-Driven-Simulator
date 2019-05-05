#include <iostream>
#include "rv.h"
#include "event.h"

// Simulates an M/M/1 queueing system.  The simulation terminates
// once 200000 events depart from the system.
int main()
{
  EventList Elist;                                  // Create event list
  enum {ARR,DEP};                                   // Define the event types

  double lambda = 6.0;                              // Arrival rate, changes cause of rho
  // 4.2
  double mu = 3.0;                                  // Departure rate jobs/second

  double clock = 0.0;                               // System clock
  int N = 0;                                        // Number of events in system
  int Ndep = 0;                                     // Number of departures from system
  double EN = 0.0;                                  // For calculating E[N]
  
  double block =0.0;                                // Calculate how many events are blocked
  int K = 4;                                        // Capacity of machine
  int M = 2;                                        // Number of processors 
  int done = 0;                                     // End condition satisfied?
  int totalDep = 200000;                            // Number of total events to hit done
  int arr = 0;

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
      arr++;
      if (N<K) {
        N++;                                        // Increment system size if not at capacity
      } else {
        block++;                                    // Block event if at capacity
      }
      if (M > 0 && M <=2) {                         // Check if available processor
        Elist.insert(clock+exp_rv(mu),DEP);         // Generate next departure
        M--;  
      }
      Elist.insert(clock+exp_rv(lambda),ARR);       // Generate next arrival
    break;
    case DEP:                                       // If departure
      Ndep++;                                       // Increase number of departures
      EN += N*(clock-prev);                         // update system statistics
        M++;
      if (N>0) {                                    // If there is an event, generate departure and decrease size
        N--;
        Elist.insert(clock+exp_rv(mu),DEP);
        M--;
      }
      break;
    }
    delete CurrentEvent;
    if (Ndep > totalDep) done=1;                 // End condition
  }
  
  // output simulation results for N, E[N] 
  std::cout << "Expected number of events (simulation): " << EN/clock << std::endl;
  std::cout << "Average time an event is in the system: " << (EN/clock)/lambda << std::endl;
  std::cout << "Probability of events blocked: " << block/arr << std::endl;

}    
  
  