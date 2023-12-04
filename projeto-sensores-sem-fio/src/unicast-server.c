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
static uint32_t total_messages_counter = 0;

/*---------------------------------------------------------------------------*/
#define MAX_SENDERS 50 // Número maximo de transmissores

// struct para a contagem das mensagens
struct SenderCounter
{
  uip_ipaddr_t sender_addr;
  int counter;
};

// Array para armazenar as contagens
struct SenderCounter sender_counters[MAX_SENDERS];

// função que incrementa a contagem de mensagens de um dado transmissor
int sender_index = 0;
int increment_sender_counter(const uip_ipaddr_t *sender_addr)
{
  for (int i = 0; i < MAX_SENDERS; i++)
  {
    // Compara o endereço do transmissor com o endereço na posição i da array
    if (uip_ipaddr_cmp(sender_addr, &sender_counters[i].sender_addr) == 1)
    {
      // incrementa o contador para o endereço e retorna a contagem
      sender_counters[i].counter++;
      return sender_counters[i].counter;
    }
  }

  // se o endereço do transmissor não estiver na array, adicione-o 
  uip_ipaddr_copy(&sender_counters[sender_index].sender_addr, sender_addr);
  sender_counters[sender_index].counter = 1;
  sender_index++;
  return sender_counters[sender_index - 1].counter;
}
/*---------------------------------------------------------------------------*/
PROCESS(unicast_server_process, "UDP server");
AUTOSTART_PROCESSES(&unicast_server_process);
/*---------------------------------------------------------------------------*/
int sender_messages_counter;

static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
  total_messages_counter++;
  sender_messages_counter = increment_sender_counter(sender_addr);

  LOG_INFO_("recebida mensagem: '%.*s' do endereco ", datalen, (char *)data);
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");

  LOG_INFO_("NODECOUNT--");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("--%d", sender_messages_counter);
  LOG_INFO_("\n");

  LOG_INFO_("TOTALCOUNT--%u\n", total_messages_counter);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_server_process, ev, data)
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
