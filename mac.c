/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include "../../../../src/xApp/e42_xapp_api.h"
#include "../../../../src/util/alg_ds/alg/defer.h"
#include "../../../../src/util/time_now_us.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

int ifTerminate = false;
long long count = 0;

void terminate_(int signum){
  ifTerminate = true;
  printf("terminate!!!");
}

void printJsonMac(sm_ag_if_rd_t const* rd){
  mac_ue_stats_impl_t * stats = rd->ind.mac.msg.ue_stats;
  uint32_t node_number = rd->ind.mac.msg.len_ue_stats;
  int64_t now = time_now_us();
  
  char result[20000];
  char temp[20000];

  strcat(result,"{\"MAC_UE\":{");
  sprintf(temp,"\"len_ue_stats\": %u,", node_number );    strcat(result, temp);
  sprintf(temp,"\"tstamp\": %ld,", now - rd->ind.mac.msg.tstamp );    strcat(result, temp);
  strcat(result,"\"ue_stats\": [");

  for(uint32_t i = 0; i < node_number; i++){
    if(i!=0) strcat(result,",");
    strcat(result,"{");
        sprintf(temp,"\"dl_aggr_tbs\": %lu,",stats[i].dl_aggr_tbs);  strcat(result, temp);
        sprintf(temp,"\"ul_aggr_tbs\": %lu,",stats[i].ul_aggr_tbs);  strcat(result, temp);
        sprintf(temp,"\"dl_aggr_bytes_sdus\": %lu,",stats[i].dl_aggr_bytes_sdus);  strcat(result, temp);
        sprintf(temp,"\"ul_aggr_bytes_sdus\": %lu,",stats[i].ul_aggr_bytes_sdus);  strcat(result, temp);
        sprintf(temp,"\"dl_curr_tbs\": %lu,",stats[i].dl_curr_tbs);  strcat(result, temp);
        sprintf(temp,"\"ul_curr_tbs\": %lu,",stats[i].ul_curr_tbs);  strcat(result, temp);
        sprintf(temp,"\"dl_sched_rb\": %lu,",stats[i].dl_sched_rb);  strcat(result, temp);
        sprintf(temp,"\"ul_sched_rb\": %lu,",stats[i].ul_sched_rb);  strcat(result, temp);
        sprintf(temp,"\"pusch_snr\": %f,",stats[i].pusch_snr);  strcat(result, temp);
        sprintf(temp,"\"pucch_snr\": %f,",stats[i].pucch_snr);  strcat(result, temp);
        sprintf(temp,"\"dl_bler\": %f,",stats[i].dl_bler);  strcat(result, temp);
        sprintf(temp,"\"ul_bler\": %f,",stats[i].ul_bler);  strcat(result, temp);
        sprintf(temp,"\"rnti\": %u,",stats[i].rnti);  strcat(result, temp);
        sprintf(temp,"\"dl_aggr_prb\": %u,",stats[i].dl_aggr_prb);  strcat(result, temp);
        sprintf(temp,"\"ul_aggr_prb\": %u,",stats[i].ul_aggr_prb);  strcat(result, temp);
        sprintf(temp,"\"dl_aggr_sdus\": %u,",stats[i].dl_aggr_sdus);  strcat(result, temp);
        sprintf(temp,"\"ul_aggr_sdus\": %u,",stats[i].ul_aggr_sdus);  strcat(result, temp);
        sprintf(temp,"\"dl_aggr_retx_prb\": %u,",stats[i].dl_aggr_retx_prb);  strcat(result, temp);
        sprintf(temp,"\"ul_aggr_retx_prb\": %u,",stats[i].ul_aggr_retx_prb);  strcat(result, temp);
        sprintf(temp,"\"bsr\": %u,",stats[i].bsr);  strcat(result, temp);
        sprintf(temp,"\"frame\": %u,",stats[i].frame);  strcat(result, temp);
        sprintf(temp,"\"slot\": %u,",stats[i].slot);  strcat(result, temp);
        sprintf(temp,"\"wb_cqi\": %u,",stats[i].wb_cqi);  strcat(result, temp);
        sprintf(temp,"\"dl_mcs1\": %u,",stats[i].dl_mcs1);  strcat(result, temp);
        sprintf(temp,"\"ul_mcs1\": %u,",stats[i].ul_mcs1);  strcat(result, temp);
        sprintf(temp,"\"dl_mcs2\": %u,",stats[i].dl_mcs2);  strcat(result, temp);
        sprintf(temp,"\"ul_mcs2\": %u,",stats[i].ul_mcs2);  strcat(result, temp);
        sprintf(temp,"\"phr\": %d,",stats[i].phr);  strcat(result, temp);
        sprintf(temp,"\"dl_num_harq\": %u,",stats[i].dl_num_harq);  strcat(result, temp);
        sprintf(temp,"\"ul_num_harq\": %u,",stats[i].ul_num_harq);  strcat(result, temp);

        strcat(result, "\"dl_harq\": [" );
        for (uint8_t j = 0;  j < stats[i].dl_num_harq; j++){
          if(j!=0) strcat(result,",");
          sprintf(temp,"%u",stats[i].dl_harq[j]);  strcat(result, temp);
        }
        strcat(result,"],");

        strcat(result, "\"ul_harq\": [" );
        for (uint8_t j = 0;  j < stats[i].ul_num_harq; j++){
          if(j!=0) strcat(result,",");
          sprintf(temp,"%u",stats[i].ul_harq[j]);  strcat(result, temp);
        }
        strcat(result,"]");

    strcat(result,"}");
  }
  strcat(result,"]}}");

  printf("%s \n\n\n",result);
}

static
void sm_cb_mac(sm_ag_if_rd_t const* rd)
{
  assert(rd != NULL);
  assert(rd->type == MAC_STATS_V0);
  int64_t now = time_now_us();

  printf("MAC ind_msg latency = %ld \n", now - rd->ind.mac.msg.tstamp);
  printf("count: %ld",count++);
  //printJsonMac(rd);

}


int main(int argc, char *argv[])
{

  signal(SIGINT, terminate_);

  fr_args_t args = init_fr_args(argc, argv);
  //Init the xApp
  init_xapp_api(&args);
  sleep(1);
  e2_node_arr_t nodes = e2_nodes_xapp_api();
  defer({ free_e2_node_arr(&nodes); });
  assert(nodes.len > 0);

  printf("Connected E2 nodes = %d\n", nodes.len);

  // MAC indication
  const char* i_0 = "10_ms";
  sm_ans_xapp_t* mac_handle = NULL;

  if(nodes.len > 0){
    mac_handle = calloc( nodes.len, sizeof(sm_ans_xapp_t) ); 
    assert(mac_handle  != NULL);
  }

  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_t* n = &nodes.n[i];
    for (size_t j = 0; j < n->len_rf; j++)
      printf("Registered node %d ran func id = %d \n ", i, n->ack_rf[j].id);

    mac_handle[i] = report_sm_xapp_api(&nodes.n[i].id, n->ack_rf[0].id, (void *)i_0, sm_cb_mac);
    assert(mac_handle[i].success == true);
  }
  
  while(!ifTerminate){
    sleep(1);  //continously receive the data until the interrupt happen 
  }

  for(int i = 0; i < nodes.len; ++i){
    // Remove the handle previously returned
    rm_report_sm_xapp_api(mac_handle[i].u.handle);
  }

  if(nodes.len > 0){
    free(mac_handle);
  }

  //Stop the xApp
  while(try_stop_xapp_api() == false)
    usleep(1000);

  printf("Test xApp run SUCCESSFULLY\n");

}

