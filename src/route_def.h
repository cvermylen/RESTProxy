#ifndef ROUTE_DEF_H
#define ROUTE_DEF_H

#include <stdio.h>

/*! A route defines a flow of data between a source and a sink. Source and sinks are implemented by connectors. In a uni-directional flow,
 *  the data will be routed from a source to one or several sinks. In a bi-directional flow, the data can move from source to sink or from
 *  sink to source.
 */ 
#define MAX_ROUTES	10

#define MAX_ASYNC_CONCUR	5

#define TYPE_SOCKET	1
#define TYPE_FILE	2
#define TYPE_WORKFLOW	4

#define FORWARD_MODE_SYNC	1
#define FORWARD_MODE_ASYNC	2
#define FORWARD_MODE_SEQ	4

#define FLOW_UNIDIRECTIONAL	1
#define FLOW_BIDIRECTIONAL	2

#define MODE_TCP	1

/*!
  \return the number of routes that can still be created
 *******************************************************/
//int available_routes();

/*! ********************************************************** 
  \brief Create a new entry in the routing table.
  \param direction_mode A route can be UNI or BI-Directional.
  \param connections Number of connectors
  \param forward_mode how the data should be fowarded to the destinations
  \return the id of the new route.
 **************************************************************/
//int alloc_route(const int connections, const int forward_mode);
//route_t* get_route(const int route_id);
//void set_file_out_connector(const int route_id, const int connector_id, const int flow, const char* filename, void (*callbacks)());

/*! ********************************************************
  \brief register a socket as an input connector and attach a set of output connectors. This part should be replaced with a ruling system
  \param route_id
  \param flow_direction
  \param connector_id id of the socket connector used for input
  \param callbacks set of functions to be called to process the output. To be replaced by a set of rules and connectors
 ***********************************************************/
//void set_socket_in_connector(const int route_id, const int mode, const int port, void (**callback)());
//void set_workflow_out_connector(const int route_id, const int connector_id, char* filename, void (**callback)());
/*! **************************************************
  \brief De-allocate all memory attached to this route. 
         This route can be re-allocated if needed.
  \param route_id identifies the route to be freed.
 ******************************************************/
//void free_route(const int route_id);

/*! ****************************************
  \brief De-allocate ALL routes in the system
 ********************************************/
//void destroy_routes();

#endif
