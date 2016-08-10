#include "skv-server.h"

#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

using namespace std;

SkvServer::SkvServer() :
  fd(-1),
  clients_lock(),
  clients(),
  values_lock(),
  values(),
  listener_thread()
{
}

SkvServer::~SkvServer()
{
  stop();

  values.clear();
}

void SkvServer::listener(SkvServer *server)
{
  struct sockaddr_in saddr;
  socklen_t socklen;
  int clientfd;

  while (true) {
    socklen = sizeof(saddr);
    clientfd = accept(server->fd, (struct sockaddr *) &saddr, &socklen);

    if (server->fd < 0)
      return;

    if (clientfd < 0) {
      cerr << "Error while accepting client connection" << endl;
      return;
    }

    server->clients_lock.lock();
    server->clients.push_back(clientfd);
    server->clients_lock.unlock();

    server->values_lock.lock();

    for (auto kv : server->values)
      server->notify(clientfd, kv.first, kv.second);

    server->values_lock.unlock();

  }
}

bool SkvServer::start(const string addr, int port)
{
  struct sockaddr_in saddr;
  int reuse = 1;

  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);

  if (!inet_pton(AF_INET, addr.c_str(), &saddr.sin_addr)) {
    cerr << "Invalid address" << endl;
    goto error;
  }

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    cerr << "Unable to create socket" << endl;
    goto error;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    cerr << "Could not enable SO_REUSEADDR" << endl;
    goto error;
  }

  if (::bind(fd, (const sockaddr *) &saddr, sizeof(saddr)) < 0) {
    cerr << "Unable to bind socket" << endl;
    goto error;
  }

  if (listen(fd, 20 /* backlog */) < 0) {
    cerr << "Unable to bind socket" << endl;
    goto error;
  }

  listener_thread = thread(SkvServer::listener, this);

  return true;

error:
  if (fd >= 0)
    close(fd);

  return false;
}

void SkvServer::stop()
{
  if (fd >= 0) {
    close(fd);
    fd = -1;
  }

  if (listener_thread.joinable())
    listener_thread.join();

  clients_lock.lock();
  clients.clear();
  clients_lock.unlock();
}

void SkvServer::set(const std::string key, const byte *value, size_t size)
{
  auto v = make_shared<ByteVector>(size);

  for (int i = 0; i < size; i++)
    (*v)[i] = value[i];

  values_lock.lock();
  values.emplace(key, v);
  values_lock.unlock();

  notify_all(key, v);
}

bool SkvServer::notify(int clientfd, const std::string key, const std::shared_ptr<ByteVector> value)
{
  /* FIXME: this should refactored to be dispatched from a different thread,
   * so we don't make blocking I/O during API calls. */

  /* Can we make this lock more granular? */
  MessageHeader hdr;

  hdr.version = 1;
  hdr.key_size = key.length();
  hdr.value_size = value->size();

  if (send(clientfd, &hdr, sizeof(hdr), MSG_NOSIGNAL) < 0) {
    cerr << "Failed to send message header" << endl;
    goto error;
  }

  if (send(clientfd, key.c_str(), key.length(), MSG_NOSIGNAL) < 0) {
    cerr << "Failed to send key" << endl;
    goto error;
  }

  if (send(clientfd, value->data(), value->size(), MSG_NOSIGNAL) < 0) {
    cerr << "Failed to send key" << endl;
    goto error;
  }

  return true;

error:
  return false;
}

void SkvServer::notify_all(const std::string key, const std::shared_ptr<ByteVector> value)
{
  clients_lock.lock();

  for (auto client = clients.begin(); client != clients.end(); client++) {
    if (!notify(*client, key, value)) {
      client = clients.erase(client);
    }
  }

  clients_lock.unlock();
}
