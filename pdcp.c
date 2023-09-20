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
void sm_cb_pdcp(sm_ag_if_rd_t const* rd)
{
  assert(rd != NULL);
  assert(rd->type == PDCP_STATS_V0);
  int64_t now = time_now_us();

  char result[20000];
  char temp[20000];

  printf("PDCP ind_msg latency = %ld \n", now - rd->pdcp_stats.msg.tstamp);

  uint32_t node_number = rd->pdcp_stats.msg.len;
  pdcp_radio_bearer_stats_t * stats = rd->pdcp_stats.msg.rb;

  strcat(result,"{\"PDCP_bearer\":{");
  sprintf(temp,"\"len\": %u,", node_number );    strcat(result, temp);
  sprintf(temp,"\"tstamp\": %ld,", now - rd->pdcp_stats.msg.tstamp );    strcat(result, temp);
  strcat(result,"\"rb\": [");

  for(uint32_t i = 0; i < node_number; i++){
    if(i!=0) strcat(result,",");
    strcat(result,"{");
        sprintf(temp,"\"txpdu_pkts\": %u,",stats[i].txpdu_pkts);      strcat(result, temp);
        sprintf(temp,"\"txpdu_bytes\": %u,",stats[i].txpdu_bytes);      strcat(result, temp);
        sprintf(temp,"\"txpdu_sn\": %u,",stats[i].txpdu_sn);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_pkts\": %u,",stats[i].rxpdu_pkts);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_bytes\": %u,",stats[i].rxpdu_bytes);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_sn\": %u,",stats[i].rxpdu_sn);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_oo_pkts\": %u,",stats[i].rxpdu_oo_pkts);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_oo_bytes\": %u,",stats[i].rxpdu_oo_bytes);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_dd_pkts\": %u,",stats[i].rxpdu_dd_pkts);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_dd_bytes\": %u,",stats[i].rxpdu_dd_bytes);      strcat(result, temp);
        sprintf(temp,"\"rxpdu_ro_count\": %u,",stats[i].rxpdu_ro_count);      strcat(result, temp);
        sprintf(temp,"\"txsdu_pkts\": %u,",stats[i].txsdu_pkts);      strcat(result, temp);
        sprintf(temp,"\"txsdu_bytes\": %u,",stats[i].txsdu_bytes);      strcat(result, temp);
        sprintf(temp,"\"rxsdu_pkts\": %u,",stats[i].rxsdu_pkts);      strcat(result, temp);
        sprintf(temp,"\"rxsdu_bytes\": %u,",stats[i].rxsdu_bytes);      strcat(result, temp);
        sprintf(temp,"\"rnti\": %u,",stats[i].rnti);      strcat(result, temp);
        sprintf(temp,"\"mode\": %u,",stats[i].mode);      strcat(result, temp);
        sprintf(temp,"\"rbid\": %u",stats[i].rbid);      strcat(result, temp);
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

  // PDCP indication
  inter_xapp_e i_2 = ms_10;
  sm_ans_xapp_t* pdcp_handle = NULL;

  if(nodes.len > 0){
    pdcp_handle = calloc( nodes.len, sizeof(sm_ans_xapp_t) ); 
    assert(pdcp_handle  != NULL);
  }

  for (int i = 0; i < nodes.len; i++) {
    e2_node_connected_t* n = &nodes.n[i];
    for (size_t j = 0; j < n->len_rf; j++)
      printf("Registered node %d ran func id = %d \n ", i, n->ack_rf[j].id);

    pdcp_handle[i] = report_sm_xapp_api(&nodes.n[i].id, n->ack_rf[2].id, i_2, sm_cb_pdcp);
    assert(pdcp_handle[i].success == true);
  }

  sleep(1);


  for(int i = 0; i < nodes.len; ++i){
    // Remove the handle previously returned
    rm_report_sm_xapp_api(pdcp_handle[i].u.handle);
  }

  if(nodes.len > 0){
    free(pdcp_handle);
  }

  //Stop the xApp
  while(try_stop_xapp_api() == false)
    usleep(1000);

  printf("Test xApp run SUCCESSFULLY\n");
}

