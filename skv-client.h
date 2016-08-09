#ifndef SKV_CLIENT_H
#define SKV_CLIENT_H

#include "common.h"

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using UpdateCallback = std::function<void(const std::string, const std::shared_ptr<ByteVector>)>;

class SkvClient {
  public:
    SkvClient();
    ~SkvClient();

    /* Starts a thread listening for updates */
    bool connect(const std::string addr, int port);
    void disconnect();

    /* The underlying pointer is going to be a continuous array, but using
     * a vector lets us return the array and size at one go. */
    std::shared_ptr<const ByteVector> get(const std::string key);

    void setCallback(UpdateCallback cb);

  private:
    int fd;

    std::mutex values_lock;
    std::map<std::string, std::shared_ptr<ByteVector>> values;

    std::thread listener_thread;
    static void listener(SkvClient *client);
};

#endif /* SKV_SERVER_H */
