#ifndef SKV_CLIENT_H
#define SKV_CLIENT_H

#include "common.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

using UpdateCallback = std::function<void(const std::string)>;

class SkvClient {
  public:
    SkvClient();
    ~SkvClient();

    /* Starts a thread listening for updates */
    bool connect(const std::string addr, int port);
    void disconnect();

    /* The underlying pointer is going to be a continuous array, but using
     * a vector lets us return the array and size at one go. */
    std::shared_ptr<const std::vector<byte>> get(const std::string key);

    void setCallback(UpdateCallback cb);
};

#endif /* SKV_SERVER_H */
