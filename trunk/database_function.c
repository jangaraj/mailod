#include <stdio.h>
#include </home/vlk/bin/include/dbi/dbi.h>



int xmain() {
    dbi_conn conn;
    dbi_result result;
    double threshold = 4.333333;
    unsigned int idnumber;
    const char *fullname;
					    
    //dbi_initialize("/home/vlk/bin/libdbi-drivers/lib");
	dbi_initialize(NULL);

    conn = dbi_conn_new("mysql");

    dbi_conn_set_option(conn, "host", "localhost");
//    printf("Host: %s\n",dbi_conn_get_option(conn, "host"));
    dbi_conn_set_option(conn, "username", "mailsql");
//    printf("Username: %s\n",dbi_conn_get_option(conn, "username"));
	dbi_conn_set_option(conn, "password", "v_tec_ppc");
//    printf("Password: %s\n",dbi_conn_get_option(conn, "password"));
	dbi_conn_set_option(conn, "dbname", "vysokoskolaci");
    printf("Dbname: %s\n",dbi_conn_get_option(conn, "dbname"));
//    dbi_conn_set_option(conn, "encoding", "UTF-8");
    if (dbi_conn_connect(conn) < 0) {
      printf("Could not connect. Please check the option settings\n");
	  printf("Debug Host: %s\n",dbi_conn_get_option(conn, "host"));
	  printf("Debug Username: %s\n",dbi_conn_get_option(conn, "username"));
	 // printf("Debug Password: ****\n",dbi_conn_get_option(conn, "password"));
	  printf("Debug Dbname: %s\n",dbi_conn_get_option(conn, "dbname"));
    }
    else {
	    result = dbi_conn_query(conn, "SELECT * FROM tkeyuser");
																	      
    if (result) {
		while (dbi_result_next_row(result)) {
		  idnumber = dbi_result_get_uint(result, "id");
	 	  fullname = dbi_result_get_string(result, "name");
	  	  printf("%i. %s\n", idnumber, fullname);
	  	}
		dbi_result_free(result);
	}
	else {
		printf("query zial nic nevratil, mas to pokazene");	
	}
	dbi_conn_close(conn);
	}
	dbi_shutdown();
    return 0;
}
