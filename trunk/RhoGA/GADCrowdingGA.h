// $Header: /cvs/hep/rho/RhoGA/GADCrowdingGA.h,v 1.1.1.1 2001-05-04 16:14:19 marcel Exp $
/* ----------------------------------------------------------------------------
  dcrowdingga.h
  mbwall 29mar99
  Copyright (c) 1999 Matthew Wall, all rights reserved

  Header file for the steady-state genetic algorithm class.
---------------------------------------------------------------------------- */
#ifndef _ga_deterministic_crowding_ga_h_
#define _ga_deterministic_crowding_ga_h_

#include <RhoGA/GABaseGA.h>

class GADCrowdingGA : public GAGeneticAlgorithm {
public:
  GADefineIdentity("GADeterministicCrowdingGA", 241);

  GADCrowdingGA(const GAGenome& g) : GAGeneticAlgorithm(g) {}
  virtual ~GADCrowdingGA() {}

  virtual void initialize(unsigned int seed=0);
  virtual void step();
  GADCrowdingGA& operator++() { step(); return *this; }
};

#endif
