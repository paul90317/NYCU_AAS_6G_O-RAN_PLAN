add_executable(xapp_mac 
             	mac.c
                ../../../../src/util/alg_ds/alg/defer.c
                ../../../../src/util/time_now_us.c
                )

target_link_libraries(
						xapp_mac
 						PUBLIC
 						e42_xapp
 						-pthread
 						-lsctp
 						-ldl
 						)

add_executable(xapp_pdcp 
            	pdcp.c
                ../../../../src/util/alg_ds/alg/defer.c
                ../../../../src/util/time_now_us.c
                )

 target_link_libraries(
						xapp_pdcp
 						PUBLIC
 						e42_xapp
 						-pthread
 						-lsctp
 						-ldl
 						)

add_executable(xapp_rlc 
            	rlc.c
                ../../../../src/util/alg_ds/alg/defer.c
                ../../../../src/util/time_now_us.c
                )


 target_link_libraries(
						xapp_rlc
 						PUBLIC
 						e42_xapp
 						-pthread
 						-lsctp
 						-ldl
 						)