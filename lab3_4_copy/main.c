
/*
 * 
 * Call Blocking in Circuit Switched Networks
 * 
 * Copyright (C) 20014 Terence D. Todd
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "trace.h"
#include "simparameters.h"
#include "cleanup.h"
#include "call_arrival.h"
#include "main.h"
#include<windows.h>

/*******************************************************************************/

int main(void)
{
  int i;
  int j=0;
  int k = 0;
  int l = 0;
  int is_all_ch_free = 0;

  FILE *fp = fopen("D:/Dropbox/Dropbox/_ECE Level 4/4DK4/Lab 3/result/Part 4/result.txt", "w+");

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data; /* Simulation_Run_Data is defined in main.h. */
  Channel_Ptr free_channel;

  /* 
   * Get the list of random number generator seeds defined in simparameters.h.
   */
  double SET_OF_NUMBER_OF_ARRIVAL_RATES[] = {NUMBER_OF_ARRIVAL_RATES,0};
  unsigned SET_OF_NUMBER_OF_CHANNELS[] = {NUMBER_OF_CHANNELS,0};
  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;

  int number_of_channels;
  double arrival_rate;
  /* 
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */

  while ((number_of_channels = SET_OF_NUMBER_OF_CHANNELS[k++]) != 0){
	  fprintf(fp,"number of channel = %d \n", number_of_channels);
	  l=0;
	  while((arrival_rate = SET_OF_NUMBER_OF_ARRIVAL_RATES[l++]) != 0){
		  fprintf(fp,"arrival rate = %.3f \n", arrival_rate);
		  j=0;
		  while ((random_seed = RANDOM_SEEDS[j++]) != 0) {
			  fprintf(fp,"random seed = %d \n", random_seed);

    /* Create a new simulation_run. This gives a clock and eventlist. */
    simulation_run = simulation_run_new();

    /* Add our data definitions to the simulation_run. */
    simulation_run_set_data(simulation_run, (void *) & data);

    /* Initialize our simulation_run data variables. */
    data.blip_counter = 0;
    data.call_arrival_count = 0;
    data.calls_processed = 0;
    data.number_of_calls_in_queue = 0;
    data.number_of_calls_processed = 0;
    data.accumulated_call_time = 0.0;
    data.random_seed = random_seed;

	data.number_of_channels= number_of_channels;
	data.arrival_rate = arrival_rate;

	data.fifo_queue = fifoqueue_new();

	data.number_of_calls_hanged_up = 0;
	data.accumulated_waiting_time = 0.0;

    /* Create the channels. */
    data.channels = (Channel_Ptr *) xcalloc((int) number_of_channels,
					    sizeof(Channel_Ptr));

    /* Initialize the channels. */
    for (i=0; i<number_of_channels; i++) {
      *(data.channels+i) = server_new(); 
    }

    /* Set the random number generator seed. */
    random_generator_initialize((unsigned) random_seed);

    /* Schedule the initial call arrival. */
    schedule_call_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +
			exponential_generator((double) 1/arrival_rate));
    
    /* Execute events until we are finished. */
	while (data.call_arrival_count < RUNLENGTH || fifoqueue_size(data.fifo_queue) > 0 || (is_all_ch_free = is_all_channel_free(simulation_run)) == 0) {
      simulation_run_execute_event(simulation_run);
    }



    
    /* Print out some results. */
    output_results(simulation_run);


	fprintf(fp,"%ld \n", data.number_of_calls_processed); 
	fprintf(fp, "%ld \n", data.number_of_calls_hanged_up); /*total number hung up*/
	fprintf(fp, "%ld \n", data.number_of_calls_in_queue); /*total calls being put in queue*/
	fprintf(fp,"%.5f \n", data.accumulated_waiting_time); /*accumulated waiting time*/
	fprintf(fp, "%ld \n", fifoqueue_size(data.fifo_queue)); /*fifo size*/
	fprintf(fp, "\n");

    /* Clean up memory. */
    cleanup(simulation_run);
		  
		  }/*end of while loop for random seed*/
	  }/*end of while loop for arrival rate*/
 
  }/*end of while channel*/

  /* Pause before finishing. */
  fclose(fp);
  MessageBox(0, "DONE", "", 1);
  getchar();
  return 0;
}

int is_all_channel_free(Simulation_Run_Ptr simulation_run)
{
	Channel_Ptr *channels;
	int i;
	int number_of_ch;

	channels = ((Simulation_Run_Data_Ptr)simulation_run->data)->channels;
	number_of_ch = ((Simulation_Run_Data_Ptr)simulation_run->data)->number_of_channels;

	for (i = 0; i<number_of_ch; i++) {
		if (server_state(*(channels + i)) != FREE)
			return 0;
	}
	return 1;
}















