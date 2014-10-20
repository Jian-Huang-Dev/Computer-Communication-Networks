
/*
 * 
 * Simulation_Run of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
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

/******************************************************************************/

#include <stdio.h>
#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. At that time the function "end_packet_transmission" (defined
 * in packet_transmissionl.c) is executed. A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
				       double event_time, void* packet)
{
  Event_Type this_event = {"Packet Xmt End", end_packet_transmission_event};
  long int event_id;

  event_id = simulation_run_schedule_event(simulation_run, this_event,
					   event_time, (void*) packet);
  return event_id;
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void* ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(data->link);

  /* Collect statistics. */
  // total number of packets processed
  data->number_of_packets_processed++;
  // different packet type has different delay and accumulated delay
  if(this_packet->packet_type == DATA){
	data->number_of_data_packets_processed++;
	data->data_packet_accumulated_delay += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
  }else if (this_packet->packet_type == VOICE){
	data->number_of_voice_packets_processed++;
	data->voice_packet_accumulated_delay += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
  }else{
	  data->number_of_voice_2_packets_processed++;
	  data->voice_2_packet_accumulated_delay += simulation_run_get_time(simulation_run) -  this_packet->arrive_time;
  }
  
  data->accumulated_delay += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;

  /* Output activity blip every so often. */
  output_progress_msg_to_screen(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void*) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->voice_buffer) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->voice_buffer);
    start_packet_transmission(simulation_run, next_packet);
  }else if(fifoqueue_size(data->buffer) > 0){
	next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
    start_packet_transmission(simulation_run, next_packet);
  }
}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_packet_transmission(Simulation_Run_Ptr simulation_run, 
			  Packet_Ptr this_packet)
{
  Simulation_Run_Data_Ptr data;
  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  TRACE(printf("Start Of Packet.\n");)

  server_put(data->link, (void*) this_packet);
  this_packet->status = XMTTING;


  /* Schedule the end of packet transmission event. */
	schedule_end_packet_transmission_event(simulation_run,
		simulation_run_get_time(simulation_run) + this_packet->service_time,(void*) this_packet);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double
get_packet_transmission_time(void)
{
  return ((double) PACKET_XMT_TIME);
}

double
get_voice_packet_transmission_time(void){
	return ((double) VOICE_XMT_TIME);
}
double
get_voice_2_packet_transmission_time(void){
	return ((double) VOICE_2_XMT_TIME);
}

