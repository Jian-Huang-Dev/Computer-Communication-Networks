
/*
 * Simulation of the ALOHA Protocol
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/*******************************************************************************/

#define EPSILON 0.0001

#define NUMBER_OF_STATIONS 10
//#define PACKET_ARRIVAL_RATE 0.1
#define MEAN_PACKET_DURATION 1
//#define MEAN_BACKOFF_DURATION 50
#define RUNLENGTH 5000000
//#define RUNLENGTH 2000000
#define BLIPRATE 10000

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 1061556, 1069639

/*******************************************************************************/

#endif /* simparameters.h */






