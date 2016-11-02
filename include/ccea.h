/*********************************************************************
*  ccea.h
*
*  CCEA provides an API for a Cooperative Coevolutionary Algorithm based on
*    the FANN neural network library.
*
*  Copyright (C) 2016 Eric Klinkhammer
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef _CCEA_H
#define _CCEA_H

#include <random>
#include <iostream>
#include <vector>

#include <fann.h>
#include <fann_cpp.h>

class CCEA {
  
 private:

  std::vector<std::vector<FANN::neural_net*> > population;
  
  /*
    Creates a mutated copy of the inputed neural network.

    Args:
      The network to be mutated
      The percentage of weights that will be tweaked
      The maximum magnitude of the change, as a percentage of the value being changed

    Example Usage:
      mutate(net, 0.1, 0.2)
      This will return a new neural network, with 10% of the weights changed. All changed weights
        will be between 80% and 120% of their original values.
   */
  FANN::neural_net* mutate(FANN::neural_net*, double, double);

  /*
    Creates a 2x pool of policies from an original pool, with each original member contributing 
      to one mutated policy.

    Args:
      The pool of networks to be mutated.
      The percentage of weights to be mutated.
      The maximum magnitude (as a percentage of the current weight), that a weight can mutate.
   */
  std::vector<FANN::neural_net*> createSuccessors(std::vector<FANN::neural_net*>, double, double);
  /*
    Creates an initial population of policies.

    Args:
      The number of pools in the population. One network from each pool will be used in the
        evaluation step in runGeneration.
      The number of members per pool. 
   */
  void init_population(int,int);
  
 public:
  /*
    Default constructor.
   */
  CCEA ();

  /*
    Initializes a CCEA with a population made up of pools of neural networks, initialized to
      random weights.

    Args:
      The number of pools.
      The number of networks per pool.
   */
  CCEA (int,int);

  /*
    Given a selection criteria, creates a mutated pool of networks and selects from them
      the best performing.
    
    Args:
      A function to evaluate neural networks. It accepts a vector of networks and an equal sized
        array of doubles and populated the array of doubles with the score of the network.
      The percentage of the time the best performing neural network is selected, versus a random 
        one.
   */
  void runGeneration(void(*evalNet)(std::vector<FANN::neural_net*>, double[]),double);

  /*
    Returns the population of neural networks being trained.
   */
  std::vector<std::vector<FANN::neural_net*> > getPopulation();

  /*
    Trains the population for N Generations.

    Args:
      The number of generations to train the population.
      A function to evaluate neural networks. See runGeneration.
   */
  void trainForNGenerations(int,void(*evalNet)(std::vector<FANN::neural_net*>, double[]),double);

  /*
    Trains the population until the difference in the average scores between two successive runs
      is less than an error value.

    Args:
      The error value.
      A function to evaluate neural networks. See runGeneration.
   */
  void trainUntil(double,void(*evalNet)(std::vector<FANN::neural_net*>, double[]),double);
};

#endif
