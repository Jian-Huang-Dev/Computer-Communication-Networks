
/*
 * 
 * Call Blocking in Circuit Switched Networks
 * 
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/*******************************************************************************/

#define NUMBER_OF_ARRIVAL_RATES 1//,0.667,1,1.333,1.667,2,2.333,2.667,3,3.333  /* calls/second */
#define MEAN_CALL_DURATION 3 /* seconds */
#define RUNLENGTH 5e6 /* number of successful calls */
#define BLIPRATE 1e3
#define NUMBER_OF_CHANNELS 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19
#define MEAN_WAITING_TIME 80

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 333//, 4444, 55555, 1073196, 951321

/*******************************************************************************/

#endif /* simparameters.h */




