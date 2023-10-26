#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

#include "sys/node-id.h"

#include <string.h>
#include <stdio.h> /* For printf() */

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (8 * CLOCK_SECOND)
static linkaddr_t dest_addr = {{0x01, 0x01, 0x01, 0x00, 0x01, 0x74, 0x12, 0x00}};

#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr = {{0x01, 0x01, 0x01, 0x00, 0x01, 0x74, 0x12, 0x00}};

/*---------------------------------------------------------------------------*/
PROCESS(unicast_project, "NullNet Unicast");
AUTOSTART_PROCESSES(&unicast_project);

/*---------------------------------------------------------------------------*/
#define MESSAGE_LEN 20
void input_callback(const void *data, uint16_t len,
                    const linkaddr_t *src, const linkaddr_t *dest)
{
  if (len == MESSAGE_LEN)
  {
    char message[MESSAGE_LEN];
    memcpy(&message, data, sizeof(message));
    LOG_INFO("Received \"%s\" from ", message);
    LOG_INFO_LLADDR(src);
    LOG_INFO_("\n");
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_project, ev, data)
{
  static struct etimer periodic_timer;
  static unsigned count = 0;

  char message[MESSAGE_LEN];
  sprintf(message, "Hello from %u!", node_id);

  PROCESS_BEGIN();
  tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&message;
  nullnet_len = sizeof(message);
  nullnet_set_input_callback(input_callback);

  if (!linkaddr_cmp(&dest_addr, &linkaddr_node_addr))
  {
    etimer_set(&periodic_timer, SEND_INTERVAL);
    while (1)
    {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
      LOG_INFO("Sending \"%s\" to ", message);
      LOG_INFO_LLADDR(&dest_addr);
      LOG_INFO_("\n");

      NETSTACK_NETWORK.output(&dest_addr);
      count++;
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
