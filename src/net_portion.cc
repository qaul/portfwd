/*
  net_portion.cc

  $Id: net_portion.cc,v 1.2 2002/05/05 03:30:56 evertonm Exp $
 */

#include <syslog.h>
#include "util.h"
#include "addr.h"
#include "net_portion.h"

net_portion::net_portion(struct ip_addr addr, short m_len)
{
  address  = addr;
  mask_len = m_len;

  /*
   * Fix last significant byte of mask here so it is ready on compare time.
   */
  char last = mask_len >> 3; /* last = mask_len / 8 */
  if (last < address.len)
    address.addr[last] &= (char) (0xFF << (8 - (mask_len & 7)));
}

void net_portion::show() const
{
  show_addr(&address);
  syslog(LOG_INFO, "/%d", mask_len);
}

/*
 * Verifica se um endereco IP pertence `a rede.
 */
int net_portion::owns(const struct ip_addr *ip) const
{
  char *net_a, *net_pastend;
  char *ip_a;
  char len, left;

  /* 
   * Compara comprimento de endereco.
   */
  if (address.len != ip->len)
    return 0;

  /*
   * Compara todos os bytes, exceto o ultimo.
   */
  len = mask_len >> 3; /* mask_len / 8 */
  net_a = address.addr;
  net_pastend = net_a + len;
  ip_a = ip->addr;
  for (; net_a < net_pastend; ++net_a, ++ip_a)
    if (*net_a != *ip_a)
      return 0;

  /*
   * left = mask_len % 8 = numero de uns `a esquerda
   */
  left = mask_len & 7;
  if (!left)
    return -1;

  /*
   * Compara ultimo byte.
   *
   * 8 - ()       => numero de zeros `a direta
   * 0xFF << ()   => insere zeros `a direta
   * *ip_a & ()   => desconsidera bits que sejam zero
   */
  return *net_a == (*ip_a & (char) (0xFF << (8 - left)));
}

/* Eof: net_portion.cc */
