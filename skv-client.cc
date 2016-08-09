#include "skv-client.h"

using namespace std;

SkvClient::SkvClient() :
  fd(-1),
  values_lock(),
  values(),
  listener_thread()
{
}

SkvClient::~SkvClient()
{
  disconnect();
}

void SkvClient::listener(SkvClient *server)
{
  /*
   * Read from socket in a loop
   * Parse message header
   * Read key and value into buffers
   * Construct string and vector
   * values_lock.lock()
   *   insert into map
   * values_lock.unlock()
   * Call UpdateCallback()
   */
}

bool SkvClient::connect(const std::string addr, int port)
{
  /*
   * Set up a socket, connect
   *
   * Fire up a listener thread
   */

  return false;
}
void SkvClient::disconnect()
{
}

shared_ptr<const ByteVector> SkvClient::get(const string key)
{
  /*
   * values_lock.lock()
   *   ptr = Look up map
   * values_lock.unlock()
   * Return ptr
   *
   * The shared_ptr ensures that the underlying data is valid
   * until released, and if the key is updated, it won't affect
   * this lookup and will turn up in the next one.
   */
}
