# NYCU_AAS_6G_O-RAN_PLAN
6G O-RAN
## 
to run the xapp at the flexric version 96b758f0551f29c4467d0923dd99cc08916a6ea5. do the following modify:
1.write off signal(SIGINT,handler) in /src/xapp/e42_xapp_api.c in line 66  
the xapps are designed to run endless and end by ctrl+c, write off this line to ensure the complete e2ap end process is conducted.
ii.	To make sure there is the unique database name after each time restart the xapp. i modify the function get_conf_db_name() in src/util/conf_file by add time feature after the customized name
