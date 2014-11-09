
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "call_duration.h"
#include "simparameters.h"
#include "call_departure.h"
#include "call_arrival.h"


/*******************************************************************************/

/*
 * Function to schedule a call arrival event. 
 */

long int
schedule_call_arrival_event(Simulation_Run_Ptr simulation_run, 
			    double event_time)
{
  Event new_event;

  new_event.description = "Packet Arrival";
  new_event.function = call_arrival_event;
  new_event.attachment = NULL;

  return simulation_run_schedule_event(simulation_run, new_event, event_time);
}

/*******************************************************************************/

/*
 * Call arrival event function.
 */

void
call_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Call_Ptr new_call;
  Call_Ptr call_arrived;
  Channel_Ptr free_channel;
  Simulation_Run_Data_Ptr sim_data;
  double now;
  //bool got_a_call_from_queue;

  //got_a_call_from_queue = true;

  now = simulation_run_get_time(simulation_run);

  sim_data = simulation_run_data(simulation_run);
  sim_data->call_arrival_count++;

      /* Start the call. */
    new_call = (Call_Ptr) xmalloc(sizeof(Call));
    new_call->arrive_time = now;
    new_call->call_duration = get_call_duration();
	new_call->waiting_time = exponential_generator((double) MEAN_WAITING_TIME);
	//new_call->got_call_from_queue = 0;

  if((free_channel = get_free_channel(simulation_run)) != NULL && fifoqueue_size(sim_data->fifo_queue) == 0) {
	  /*channel is free and queue size is 0*/


    server_put(free_channel, (void*) new_call);
    new_call->channel = free_channel;

    schedule_end_call_on_channel_event(simulation_run,
				       now + new_call->call_duration,
				       (void *) free_channel);
  }
  //else if (((free_channel = get_free_channel(simulation_run)) != NULL && fifoqueue_size(sim_data->fifo_queue) > 0)){
	/*channel is free and queue size is greater than 0*/

	//  call_arrived = new_call;

	//  /*take a call out of the queue to the free channel*/
	//  new_call = (Call_Ptr) fifoqueue_get(sim_data->fifo_queue);
	//  new_call->got_call_from_queue = 1;
	//  
 // while(new_call->arrive_time + new_call->waiting_time <= now){
	//  sim_data->number_of_calls_hanged_up += 1;

	//  if(fifoqueue_size(sim_data->fifo_queue) > 0){
	//  
	//	  new_call = (Call_Ptr) fifoqueue_get(sim_data->fifo_queue);
	//	  new_call->got_call_from_queue = 1;
	//	  

	//  }else{
	//	  /*found out all the calls in the queue are dropped*/
	//	  /*so schedule the next arrival event*/
	//	  new_call = call_arrived;
	//	  
	//  }
	//}

 // if(new_call->got_call_from_queue == 1){
	//  fifoqueue_put(sim_data->fifo_queue, (void*) call_arrived);
	//  sim_data->number_of_calls_in_queue++;
 // }


	//sim_data->accumulated_waiting_time += now - new_call->arrive_time;
 //   server_put(free_channel, (void*) new_call);
 //   new_call->channel = free_channel;

 //   schedule_end_call_on_channel_event(simulation_run,
	//			       now + new_call->call_duration,
	//			       (void *) free_channel);
	
  //}
  else {
    /* The call was blocked. */
	/*put the call in queue*/
	fifoqueue_put(sim_data->fifo_queue, (void*) new_call);
    sim_data->number_of_calls_in_queue++;
  }

  /* Schedule the next call arrival. */
 // if (sim_data->call_arrival_count < RUNLENGTH) {

  if (sim_data->call_arrival_count < RUNLENGTH) {
	  schedule_call_arrival_event(simulation_run,
		  now + exponential_generator((double)1 / sim_data->arrival_rate));
  }


  //}
}

/*******************************************************************************/

/*
 * Scan through the channels and return a free one, if possible. Otherwise
 * return NULL.
 */

Channel_Ptr get_free_channel(Simulation_Run_Ptr simulation_run)
{
  Channel_Ptr *channels;
  int i;
  int number_of_ch;

  channels = ((Simulation_Run_Data_Ptr) simulation_run->data)->channels;
  number_of_ch = ((Simulation_Run_Data_Ptr) simulation_run->data)->number_of_channels; 
  
  for (i=0; i<number_of_ch; i++) {
    if (server_state(*(channels+i)) == FREE)
      return *(channels+i);
  }
  return (Channel_Ptr) NULL;
}

