#include "contiki.h"
#include "sys/node-id.h"
#include "sys/log.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "net/mac/tsch/tsch.h"
#include "net/routing/routing.h"

PROCESS(tsch_rpl_project, "TSCH RPL NODE");
AUTOSTART_PROCESSES(&tsch_rpl_project);

PROCESS_THREAD(tsch_rpl_project, ev, data)
{
    PROCESS_BEGIN();
    if (node_id == 1)
    { /* Coordinator node. */
        NETSTACK_ROUTING.root_start();
    }
    NETSTACK_MAC.on();
    PROCESS_END();
}
