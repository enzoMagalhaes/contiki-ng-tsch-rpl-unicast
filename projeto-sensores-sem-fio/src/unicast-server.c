#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/mac/tsch/tsch.h"
#include "net/ipv6/simple-udp.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

static struct simple_udp_connection udp_conn;
static uint32_t recieved_messages_counter = 0;

/*---------------------------------------------------------------------------*/
#define MAX_SENDERS 50 // Maximum number of senders to track

// Structure to represent sender address and associated counter
struct SenderCounter
{
  uip_ipaddr_t sender_addr;
  int counter;
};

// Array to store sender counters
struct SenderCounter sender_counters[MAX_SENDERS];

// Function to increment the counter for a specific sender address
int sender_index = 0;
void increment_sender_counter(const uip_ipaddr_t *sender_addr)
{

  // // printf("sender addr: ");
  // // LOG_INFO_6ADDR(sender_addr);
  // // LOG_INFO_("\n");

  // for (int i = 0; i < MAX_SENDERS; i++)
  // {
  //   // printf("iteration addr: ");
  //   // LOG_INFO_6ADDR(&sender_counters[i].sender_addr);

  //   // Compare sender address with the one stored in the array
  //   if (uip_ipaddr_cmp(sender_addr, &sender_counters[i].sender_addr) == 1)
  //   {
  //     // printf("COMPARE SUCCESS");
  //     // LOG_INFO_("\n");

  //     // int test = memcmp(sender_addr, sender_addr, sizeof(uip_ip6addr_t)) == 0;
  //     // printf("test %d", test);

  //     // LOG_INFO_("\n");

  //     // LOG_INFO_6ADDR(&sender_counters[i].sender_addr);

  //     // LOG_INFO_("\n");

  //     // LOG_INFO_6ADDR(sender_addr);
  //     // LOG_INFO_("\n");

  //     // LOG_INFO_6ADDR(&sender_counters[i].sender_addr);

  //     // Address matches, increment the counter
  //     sender_counters[i].counter++;
  //     // Print all sender counters
  //     printf("Sender Counters:\n");
  //     for (int j = 0; j < MAX_SENDERS; j++)
  //     {
  //       printf("Counter for ");
  //       LOG_INFO_6ADDR(&sender_counters[j].sender_addr);
  //       printf(": %d\n", sender_counters[j].counter);
  //     }
  //     printf("sender index: %d", sender_index);
  //     return;
  //   }
  // }

  for (int i = 0; i < MAX_SENDERS; i++)
  {
    // Compare sender address with the one stored in the array
    if (uip_ipaddr_cmp(sender_addr, &sender_counters[i].sender_addr) == 1)
    {
      // Address matches, increment the counter and return
      sender_counters[i].counter++;
      return;
    }
  }

  // If the sender address is not found in the array, add it and set the counter to 1
  uip_ipaddr_copy(&sender_counters[sender_index].sender_addr, sender_addr);
  sender_counters[sender_index].counter = 1;
  sender_index++;
  return;
}

int get_sender_counter(const uip_ipaddr_t *sender_addr)
{
  for (int i = 0; i < MAX_SENDERS; i++)
  {
    // Compare sender address with the one stored in the array
    if (uip_ipaddr_cmp(sender_addr, &sender_counters[i].sender_addr) == 1)
    {
      // Address matches, return the counter
      return sender_counters[i].counter;
    }
  }

  // If the sender address is not found, return 0 (or any appropriate default value)
  return 0;
}
/*---------------------------------------------------------------------------*/

PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
  recieved_messages_counter++;
  increment_sender_counter(sender_addr);
  int sender_messages_counter = get_sender_counter(sender_addr);

  LOG_INFO_("recebida mensagem: '%.*s' do endereco ", datalen, (char *)data);
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");
  LOG_INFO_("mensagens recebidas desse transmissor: %d\n", sender_messages_counter);
  LOG_INFO_("contagem total de mensagens recebidas: %u\n", recieved_messages_counter);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize DAG root */
  NETSTACK_ROUTING.root_start();

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  /* Initialize TSCH */
  NETSTACK_MAC.on();

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
