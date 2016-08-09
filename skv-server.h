#ifndef SKV_SERVER_H
#define SKV_SERVER_H

#include "common.h"

#include <list>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class SkvServer {
  public:
    SkvServer();
    ~SkvServer();

    /* Starts a thread to listen for clients */
    bool start(const std::string addr, int port);
    void stop();

    /* Makes a copy of the value. Not using C++ array type or unique ptr to
     * without jumping through hoops */
    void set(const std::string key, const byte *value, size_t size);

  private:
    int fd;

    std::mutex values_lock;
    std::map<std::string, std::shared_ptr<ByteVector>> values;

    std::mutex clients_lock;
    std::list<int> clients; /* Note: use something other than list to scale */

    std::thread listener_thread;
    static void listener(SkvServer *server);

    bool notify(int clientfd, const std::string key, const std::shared_ptr<ByteVector> value);
    void notify_all(const std::string key, const std::shared_ptr<ByteVector> value);
};

#endif /* SKV_SERVER_H */
