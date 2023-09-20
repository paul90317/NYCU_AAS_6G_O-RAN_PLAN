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

#include "../../../../../src/xApp/e42_xapp_api.h"
#include "../../../../../src/util/alg_ds/alg/defer.h"
#include "../../../../../src/util/time_now_us.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static
void sm_cb_rlc(sm_ag_if_rd_t const* rd)
{
  assert(rd != NULL);
  assert(rd->type == RLC_STATS_V0);
  int64_t now = time_now_us();

  char result[20000];
  char temp[20000];

  printf("RLC ind_msg latency = %ld \n", now - rd->rlc_stats.msg.tstamp);

  rlc_radio_bearer_stats_t * stats = rd->rlc_stats.msg.rb;
  uint32_t node_number = rd->rlc_stats.msg.len;

  strcat(result,"{\"RLC_bearer\":{");
  sprintf(temp,"\"len\": %u,", node_number );    strcat(result, temp);
  sprintf(temp,"\"tstamp\": %ld,", now - rd->rlc_stats.msg.tstamp );    strcat(result, temp);
  strcat(result,"\"rb\": [");

  for(uint32_t i = 0; i < node_number; i++){
    if(i!=0) strcat(result,",");
    strcat(result,"{");
        sprintf(temp,"\"txpdu_pkts\": %u,",stats[i].txpdu_pkts);  strcat(result, temp);
        sprintf(temp,"\"txpdu_bytes\": %u,",stats[i].txpdu_bytes);    strcat(result, temp);  
        sprintf(temp,"\"txpdu_wt_ms\": %u,",stats[i].txpdu_wt_ms);    strcat(result, temp);
        sprintf(temp,"\"txpdu_dd_pkts\": %u,",stats[i].txpdu_dd_pkts);    strcat(result, temp);
        sprintf(temp,"\"txpdu_dd_bytes\": %u,",stats[i].txpdu_dd_bytes);    strcat(result, temp);
        sprintf(temp,"\"txpdu_retx_pkts\": %u,",stats[i].txpdu_retx_pkts);    strcat(result, temp);
        sprintf(temp,"\"txpdu_retx_bytes\": %u," ,stats[i].txpdu_retx_bytes);   strcat(result, temp);
        sprintf(temp,"\"txpdu_segmented\": %u,",stats[i].txpdu_segmented);    strcat(result, temp);
        sprintf(temp,"\"txpdu_status_pkts\": %u,",stats[i].txpdu_status_pkts);    strcat(result, temp);
        sprintf(temp,"\"txpdu_status_bytes\": %u,",stats[i].txpdu_status_bytes);    strcat(result, temp);
        sprintf(temp,"\"txbuf_occ_bytes\": %u,",stats[i].txbuf_occ_bytes);    strcat(result, temp);
        sprintf(temp,"\"txbuf_occ_pkts\": %u,",stats[i].txbuf_occ_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_pkts\": %u,",stats[i].rxpdu_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_bytes\": %u,",stats[i].rxpdu_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_dup_pkts\": %u,",stats[i].rxpdu_dup_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_dup_bytes\": %u,",stats[i].rxpdu_dup_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_dd_pkts\": %u,",stats[i].rxpdu_dd_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_dd_bytes\": %u,",stats[i].rxpdu_dd_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_ow_pkts\": %u,",stats[i].rxpdu_ow_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_ow_bytes\": %u,",stats[i].rxpdu_ow_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_status_pkts\": %u,",stats[i].rxpdu_status_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxpdu_status_bytes\": %u,",stats[i].rxpdu_status_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxbuf_occ_bytes\": %u,",stats[i].rxbuf_occ_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxbuf_occ_pkts\": %u,",stats[i].rxbuf_occ_pkts);    strcat(result, temp);
        sprintf(temp,"\"txsdu_pkts\": %u,",stats[i].txsdu_pkts);    strcat(result, temp);
        sprintf(temp,"\"txsdu_bytes\": %u,",stats[i].txsdu_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxsdu_pkts\": %u,",stats[i].rxsdu_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxsdu_bytes\": %u,",stats[i].rxsdu_bytes);    strcat(result, temp);
        sprintf(temp,"\"rxsdu_dd_pkts\": %u,",stats[i].rxsdu_dd_pkts);    strcat(result, temp);
        sprintf(temp,"\"rxsdu_dd_bytes\": %u,",stats[i].rxsdu_dd_bytes);    strcat(result, temp);
        sprintf(temp,"\"rnti\": %u,",stats[i].rnti);    strcat(result, temp);
        sprintf(temp,"\"mode\": %u,",stats[i].mode);    strcat(result, temp);
        sprintf(temp,"\"rbid\": %u",stats[i].rbid);   strcat(result, temp);
    strcat(result,"}");
  }
  strcat(result,"]}}");

  printf("%s \n\n\n",result);

}


int main(int argc, char *argv[])
{
  fr_args_t args = init_fr_args(argc, argv);
  //Init the xApp
  init_xapp_api(&args);
  sleep(1);
  e2_node_arr_t nodes = e2_nodes_xapp_api();
  defer({ free_e2_node_arr(&nodes); });
  assert(nodes.len > 0);

  printf("Connected E2 nodes = %d\n", nodes.len);


  // RLC indication
  inter_xapp_e i_1 = ms_10;
  sm_ans_xapp_t* rlc_handle = NULL;

  if(nodes.len > 0){
    rlc_handle = calloc( nodes.len, sizeof(sm_ans_xapp_t) ); 
    assert(rlc_handle  != NULL);
  }
  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_t* n = &nodes.n[i];
    for (size_t j = 0; j < n->len_rf; j++)
      printf("Registered node %d ran func id = %d \n ", i, n->ack_rf[j].id);

    rlc_handle[i] = report_sm_xapp_api(&nodes.n[i].id, n->ack_rf[1].id, i_1, sm_cb_rlc);
    assert(rlc_handle[i].success == true);
  }

  sleep(1);


  for(int i = 0; i < nodes.len; ++i){
    // Remove the handle previously returned
    rm_report_sm_xapp_api(rlc_handle[i].u.handle);
  }

  if(nodes.len > 0){
    free(rlc_handle);
  }

  //Stop the xApp
  while(try_stop_xapp_api() == false)
    usleep(1000);

  printf("Test xApp run SUCCESSFULLY\n");
}

