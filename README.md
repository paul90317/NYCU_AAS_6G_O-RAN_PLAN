# NYCU_AAS_6G_O-RAN_PLAN
6G O-RAN
## patch
To run the xapp at the [flexric](https://gitlab.eurecom.fr/mosaic5g/flexric) version 1f04cc558ebc8da9de6a620762bc02f5db4ecb4a, do the following modification:
1. write off signal(SIGINT,handler) in /src/xapp/e42_xapp_api.c in line 66  
the xapps are designed to run endless and end by ctrl+c, write off this line to ensure the complete e2ap end process is conducted.
2. To make sure there is the unique database name after each time restart the xapp. i modify the function get_conf_db_name() in src/util/conf_file by add time feature after the customized name

The first modification has been add to patch. you can apply the patch in this repos to flexric repos

## build
Copy this repos to `/flexric/examples/xApp/c/aas`, then apply the patch above at flexric, so that this repos can be built with flexric by cmake at `/flexric`.