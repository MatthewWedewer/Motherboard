//	Using the Texas Instruments (TI) Network Developement Kit (NDK) Application Programmer Interface
//
//	Strict adherence to the TI NDK API OS App layer abstraction
//
//	Getting Started NDK - (skip to page 63)		www/ti.com/lit/an/spraax4.pdf
//
//	Full NDK User Guide -						www.ti.com/lit/ug/spru523i.pdf
//
//	NDK FULL API Define -						www.ti.com/lit/ug/spru524i.pdf
//
//  RTOS TI Kernel v6.4 -						www.ti.com/lit/ug/spruex3o/spruex3o.pdf
//
//
//
//	Inline declaration stylistics and system flushing purely for developement purpose
//
//
//
//	Think of the Base Station(RED) as the Rover's Client. rove_so_tcp_handler is our internal rtos tcp ip Server
//
//
//
//	Robot Martian Logic Layer:		Created Jan.2015:		MRDT:	Owen, Connor, Judah, G_Zuess_2014
//
//
//	Last Edited:					Jan.2015				MRDT:	Judah

#include "listen_handshake_tcp_ip_proto.h"

//	macro values, the preproccessor does a find and replace in pure text before compile time

#define TCPPACKETSIZE 1024
#define LOCAL_PORT_TO_LISTEN 4500
#define MAX_NUM_CLIENTS_IN_QUEU_SIZE 3

// 	rove_so_tcp_handler fronts as rtos scheduled handler in forever loop state with prioirity and implicit blocking
//  on a preallocated Stack through the NDK API Network TASK at the RTOS init
//
// 			via TIRTOS.PRODUCTS.SYSBIOS.SCHEDULING.TASK_INSTANCE
//			and TIRTOS.PRODUCTS.NDK.NETWORK.SCHEDULING.SCHEDULING_OPTIONS
//
// 	See tcpEcho.cfg for details
//			or navigate the CCS cnfg Gui path panes (i.e. Available Products->NDK>NETWORK>SCHEDULING ..etc)
//			(via XDCTools and XConfig rtsc markup bundles installed for eclispse)

Void rove_so_tcp_handler(UArg arg0, UArg arg1){

	// File descriptor environment in this Task's private memory

		System_printf("\n");
		System_printf("Opening file descriptor environment");
		System_printf("\n");

	// System_printf buffer will only print to the console when you call flush or exit (SysMin)

		System_flush();

	// 			This is in the compile time task private stack frame. Task_Create() woud use a runtime dynamic Heap.
	// 			TaskSelf() returns a handle to rove_tcp_handler, much like a .this call in managed languages

	// 		We only use two socket structs in roveSoNet: listen_handshake_socket, and accept_recv_socket.

	// 		listen_handshake_socket passes accept this file descriptor (fd) context to accept_recv_socket
	// 				listen_handshake_socket blocks idle but will implicitly awake on client connects thru hidden calls to ndk NETCTRL and NTLTOOL lower layers scheduled by rtos
	//				TODO listen fd context is mostly persisitent, but connect listen handhsake may be reestablished by a new client connect() of the same ip/port/config/etc


	//		accept_recv_socket recieves client data in a buffer and passes to rover/rtos logic modules
	//				accept_recv_socket blocks idle in accept state if no data present but periodically polls the client for successful/keep alive vs error/reconnect reload
	// 		 		and will eventually time out and clear context and attempt to reinhereit fd contex from listen_handshake_socket at connection los


	// 		We only use two keep_alive instances of the fd environment

	// 		A single listen socket
	//		A single accept socket


	// 		Allocates ndk memory space emulating a linux file system for 16 bsd socket struct file descriptors

    fdOpenSession(TaskSelf());

    // create our listen_handshake_socket

    SOCKET listen_handshake_socket;

    // lets see how this looks intitialized. It should be init as INVALID_SOCKET (-1) state for disqualify use

    	System_printf("\n");
    	System_printf("Instantiating the listen_handshake_socket = %d",listen_handshake_socket);
    	System_printf("\n");

    // Will only print to the console when you call flush or exit (SysMin)

    System_flush();

    // AF_INET			-	defined bsd standard for ipv4 config (as opposed to ipv6)
    // SOCK_STREAM		-	defined bsd standard for connection type of socket for STREAM (as opposed to raw sockets, datagram, etc)
    // IPPROTO_TCP		- 	defined bsd standard for tcp and ip protocol (as opposed to udp, etc)

    //			If socket() succeeds, it returns a file descriptor (valued int) to represent the socket
    //			Otherwise, a value of INVALID_SOCKET is returned and the functions errno() or fdError() can be used to return the detail types of error

    listen_handshake_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    	if (listen_handshake_socket < 0) {
    		System_printf("socket() failed\n");
    		Task_exit();
    	return;
    	}//endif

    	System_printf("\n");
    	System_printf("This for listen_handshake_socket after socket() = %d", listen_handshake_socket);
    	System_printf("\n");
    	System_flush();

    // 0 is already reserved for fdStatus.return therefore we are initializing to 111 purely arbitrarily

      int fd_listen_handshake_socket_status = 111;

    // FDSTATUS_RECV	-	means what def details? TODO

    // 			fdstatus returns 0 for valid socket and on -1 writes the error for INVALID_SOCKET into our memory location
    //			We pass the &fd_listen_handshake_socket_status for reference

      	fdStatus(listen_handshake_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

      	System_printf("\n");
      	System_printf("fdStatus() for listen_handshake_socket after socket() = %d", fd_listen_handshake_socket_status);
      	System_printf("\n");
      	System_flush();

      	System_printf("\n");
      	System_printf("We have now allocated and instatiated a socket");
      	System_printf("\n");
      	System_flush();

    //reset our custom fd error flags memspace

       	fd_listen_handshake_socket_status = 111;

    //	Define our Struct for Socket Address Information on the file description Eevironment in socket 1 of the 16 frame
    //			We really only care about IP ADDRESS and PORT. Everything else is basic config for bsd socket on ip/tcp
    // 			memset is init setting this Socket Address Information structure to zero

    // AF_INET			-	defined standard for ipv4 config (as opposed to ipv6)
    // INADDR_ANY		-	defined bsd standard for 0.0.0.0 implies we listen to ANY IP at this port
    // htonl			-	'host to network long' This flips the byte order b/c the network transport layer needs them backwards (reverse the endianess)

    // hard coding the port the Client will be connecting to listen and accept from (Base Station RED Software Application

    // TODO UINT32 LOCAL_PORT_TO_LISTEN =  4500;

    struct sockaddr_in socket_local_addr;

    memset((char *)&socket_local_addr, 0, sizeof(socket_local_addr));
    socket_local_addr.sin_family = AF_INET;
    socket_local_addr.sin_len = sizeof(socket_local_addr);
    socket_local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_local_addr.sin_port = htons(LOCAL_PORT_TO_LISTEN);

    	int bindStatus = 111;

    // If bind() succeeds, this function returns 0
    //			Otherwise, a value of -1 is returned and the functions errno() or fdError() can be used to return the detail types of error

    bindStatus = bind(listen_handshake_socket, (struct sockaddr *)&socket_local_addr, sizeof(socket_local_addr));
    	if (bindStatus < 0) {
            System_printf("bind() failed\n");
            fdClose(listen_handshake_socket);
            Task_exit();
        return;
        }//endif

    	System_printf("\n");
    	System_printf("bindStatus for listen_handshake_socket after bind() = %d", bindStatus);
    	System_printf("\n");
    	System_flush();

    // lets see what fdstatus has to see about our socket now

    	fdStatus(listen_handshake_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

    	System_printf("\n");
    	System_printf("fdStatus() for listen_handshake_socket after bind() = %d", fd_listen_handshake_socket_status);
    	System_printf("\n");
    	System_flush();

    //reset our custom fd error flags memspace

    	fd_listen_handshake_socket_status = 111;

    	System_printf("\n");
    	System_printf("We have now bound our listen_handshake_socket to a ip, port, and tcp config");
    	System_printf("\n");
    	System_flush();

    // MAX_NUM_CLIENTS_IN_QUEU_SIZE	- 	this is set to 3 just b/c we want to be overcareful about the queue size right now
    //			Our App can ONLY actual handle ONE client. We just have a big queue right now for safe bounds in dev

    // TODO UINT32 MAX_NUM_CLIENTS_IN_QUEU_SIZE =  3;

    	int listenStatus = 111;

    // listen() is a blocking function call in idle state scheduled by the rtos ndk to awake upon a connect() initiated by the client (base station)

    // If listen() succeeds, this function returns 0
    //			Otherwise, a value of -1 is returned and the functions errno() or fdError() can be used to return the detail types of error

    listenStatus = listen(listen_handshake_socket, MAX_NUM_CLIENTS_IN_QUEU_SIZE);
    	if (listenStatus < 0) {
            System_printf("listen() failed\n");
            fdClose(listen_handshake_socket);
            Task_exit();
        return;
        }//endif

        System_printf("\n");
        System_printf("listenStatus for listen_handshake_socket after = %d", listenStatus);
        System_printf("\n");
        System_flush();

    // lets see what fdstatus has to see about our socket now

        fdStatus(listen_handshake_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

        System_printf("\n");
        System_printf("fdStatus() for listen_handshake_socket after listen() = %d", fd_listen_handshake_socket_status);
        System_printf("\n");
        System_flush();

     //reset our custom fd error flags memspace

        fd_listen_handshake_socket_status = 111;

        System_printf("\n");
        System_printf("Client connected, handshake success, we are now listening");
        System_printf("\n");
        System_flush();

    // setsockop() is a runtime ti ndk config call on how the listening socket should act over time in the rtos schedule

    // If setsockop() succeeds, this function returns 0
    //			Otherwise, a value of -1 is returned and the function errno() returns the type of error

        int optionStatus = 111;

    //setsocketopt() needs US to allocate a memory space for the option AND pass it in to the fnctn args

    int optval;
    int optlen = sizeof(optval);

    // SOL_SOCKET		-		at the SOCKET level, as opposed to lower layers in teh OSI/TCP heirarchy
    // SO_KEEPALIVE		-		NDK will periodically send a keep alive packet under the hood
    //			By default this sends KEEP probes every 75 seconds AFTER two hours of idle
    //			TODO change this defaultin the NDK - IP - .cfg options to a MUCH shorter idle time. pg 173 of NDK API spru524i

    // If setsockopt succeeds, this function returns 0
    //			Otherwise, a value of -1 is returned and the function errno() returns the type of error

    optionStatus = setsockopt(listen_handshake_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen);
    	if (optionStatus < 0) {
            System_printf("setsockoopt() failed\n");
            fdClose(listen_handshake_socket);
            Task_exit();
        return;
        }//endif

        System_printf("\n");
        System_printf("optionStatus for listen_handshake_socket after set sockopt() = %d", optionStatus);
        System_printf("\n");
        System_flush();

    // lets see what fdstatus has to see about our socket now

        fdStatus(listen_handshake_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

        System_printf("\n");
        System_printf("fdStatus() for listen_handshake_socket after setsockopt() = %d", fd_listen_handshake_socket_status);
        System_printf("\n");
        System_flush();

    //reset our custom fd error flags memspace

        fd_listen_handshake_socket_status = 111;

    //	Define our Struct for Socket Address Information on the file description Environment in socket 2 of the 16 frame
    //			accept_recv_socket can inherit all the context of listen_handshake_socket
    // 			This way listen_handshake_socket stays persistent and able to re-establish handhsaking
        //		accept_recv_socket is decoupled rom the handshake, and will now serve the client requests and logic and data streaming on the established connection

    //We just a need a new socket addr structs frame instance detail. We will copy all the heirarchy detail from listen_handshake_socket with accept()

    SOCKET accept_recv_socket;
    struct sockaddr_in accept_recv_socket_addr;
    int accept_recv_socket_addr_len = sizeof(accept_recv_socket);

    // If accept() succeeds, it returns a file descriptor (valued int) to represent the socket
    //			On success, accept_recv_socket now holds the identical file description environment information of the listen_handshake_socket
    //			in the ndk 'connected' state with the client
    //				Otherwise, a -1 value, INVALID_SOCKET is returned and the functions errno() or fdError() can be used to return the detail types of error

    accept_recv_socket = accept(listen_handshake_socket, (struct sockaddr*)&accept_recv_socket_addr, &accept_recv_socket_addr_len);
    if (accept_recv_socket < 0) {
                System_printf("accept() failed\n");
                fdClose(accept_recv_socket);
                fdClose(listen_handshake_socket);
                Task_exit();
            return;
            }//endif

				System_printf("\n");
				System_printf("This for listen_handshake_socket after FIRST accept() = %d", listen_handshake_socket);
				System_printf("\n");
				System_flush();

				System_printf("\n");
				System_printf("This for accept_recv_socket after FIRST accept() = %d", accept_recv_socket);
				System_printf("\n");
				System_flush();

		// lets see what fdstatus has to see about our socket now

				fdStatus(listen_handshake_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                System_printf("\n");
                System_printf("fdStatus() for listen_handshake_socket after FIRST accept() = %d", fd_listen_handshake_socket_status);
                System_printf("\n");
                System_flush();

		//reset our custom fd error flags memspace

				fd_listen_handshake_socket_status = 111;

				fdStatus(accept_recv_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                System_printf("\n");
                System_printf("fdStatus() for accept_recv_socket after FIRST accept() = %d", fd_listen_handshake_socket_status);
                System_printf("\n");
                System_flush();

		//reset our custom fd error flags memspace

				fd_listen_handshake_socket_status = 111;

		        System_printf("\n");
		        System_printf("Attempting to allocate a buffer");
		        System_printf("\n");
		        System_flush();

		 char buffer[TCPPACKETSIZE] ="BUFFERWORKS";

                 System_printf("\n");
                 System_printf("%s", buffer);
                 System_printf("\n");
                 System_flush();

         int num_bytes_recieved = -1;

         // recieve the data

         num_bytes_recieved = recv(accept_recv_socket, (char *)buffer, TCPPACKETSIZE, 0);

                 System_printf("\n");
                 System_printf("Before send() num_bytes_recieved = ");
                 System_printf("%d", num_bytes_recieved);
                 System_printf(" and the contents of the buffer = ");
                 System_printf("%s", buffer);
                 System_printf("\n");
                 System_flush();

         // lets see what fdstatus has to see about our socket now

                 fdStatus(accept_recv_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                 System_printf("\n");
                 System_printf("fdStatus() for accept_recv_socket after FIRST recv() = %d", fd_listen_handshake_socket_status);
                 System_printf("\n");
                 System_flush();

         //reset our custom fd error flags memspace

                 fd_listen_handshake_socket_status = 111;

         // Echo the data back

         send(accept_recv_socket, (char *)buffer, num_bytes_recieved, 0 );

                 System_printf("\n");
                 System_printf("After send(), we now have num_bytes_recieved = ");
                 System_printf("%d", num_bytes_recieved);
                 System_printf(" and the contents of the buffer = ");
                 System_printf("%s", buffer);
                 System_printf("\n");
                 System_flush();


         // lets see what fdstatus has to see about our socket now

                 fdStatus(accept_recv_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                 System_printf("\n");
                 System_printf("fdStatus() for accept_recv_socket after FIRST send() = %d", fd_listen_handshake_socket_status);
                 System_printf("\n");
                 System_flush();

         //reset our custom fd error flags memspace

                 fd_listen_handshake_socket_status = 111;

/*    fdClose(accept_recv_socket);

            System_printf("\n");
            System_printf("Closed FIRST accept_recv_socket");
            System_printf("\n");
            System_flush();

    accept_recv_socket = accept(listen_handshake_socket, (struct sockaddr*)&accept_recv_socket_addr, &accept_recv_socket_addr_len);
    if (accept_recv_socket < 0) {
                System_printf("accept() failed\n");
                fdClose(accept_recv_socket);
                fdClose(listen_handshake_socket);
                Task_exit();
            return;
            }//endif

				System_printf("\n");
				System_printf("This for listen_handshake_socket after SECOND accept() = %d", listen_handshake_socket);
				System_printf("\n");
				System_flush();

				System_printf("\n");
				System_printf("This for accept_recv_socket after SECOND accept() = %d", accept_recv_socket);
				System_printf("\n");
				System_flush();

    // lets see what fdstatus has to see about our socket now

                fdStatus(listen_handshake_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                System_printf("\n");
                System_printf("fdStatus() for listen_handshake_socket after SECOND accept() = %d", fd_listen_handshake_socket_status);
                System_printf("\n");
                System_flush();

        //reset our custom fd error flags memspace

                fd_listen_handshake_socket_status = 111;

                fdStatus(accept_recv_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                System_printf("\n");
                System_printf("fdStatus() for accept_recv_socket after SECOND accept() = %d", fd_listen_handshake_socket_status);
                System_printf("\n");
                System_flush();

        //reset our custom fd error flags memspace

                fd_listen_handshake_socket_status = 111;

        // recieve the data*/

        num_bytes_recieved = recv(accept_recv_socket, (char *)buffer, TCPPACKETSIZE, 0);

                 System_printf("\n");
                 System_printf("Before SECOND send() num_bytes_recieved = ");
                 System_printf("%d", num_bytes_recieved);
                 System_printf(" and the contents of the buffer = ");
                 System_printf("%s", buffer);
                 System_printf("\n");
                 System_flush();

         // lets see what fdstatus has to see about our socket now

                 fdStatus(accept_recv_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                 System_printf("\n");
                 System_printf("fdStatus() for accept_recv_socket after SECOND recv() = %d", fd_listen_handshake_socket_status);
                 System_printf("\n");
                 System_flush();

         //reset our custom fd error flags memspace

                 fd_listen_handshake_socket_status = 111;

        // Echo the data back

         send(accept_recv_socket, (char *)buffer, num_bytes_recieved, 0 );


                System_printf("\n");
                System_printf("After SECOND send(), we now have num_bytes_recieved = ");
                System_printf("%d", num_bytes_recieved);
                System_printf(" and the contents of the buffer = ");
                System_printf("%s", buffer);
                System_printf("\n");
                System_flush();


                // lets see what fdstatus has to see about our socket now

                        fdStatus(accept_recv_socket, FDSTATUS_RECV, &fd_listen_handshake_socket_status);

                        System_printf("\n");
                        System_printf("fdStatus() for accept_recv_socket after SECOND send() = %d", fd_listen_handshake_socket_status);
                        System_printf("\n");
                        System_flush();

                //reset our custom fd error flags memspace

                        fd_listen_handshake_socket_status = 111;

	fdClose(accept_recv_socket);

	fdClose(listen_handshake_socket);

    Task_exit();


}//endfnct rove_tcp_handler


//TODO : blocking -> listen(), accept(), recieve()
//
// NETRCTRL, NETTOOLS for these three functions...ie how does accept change states and socket context?



