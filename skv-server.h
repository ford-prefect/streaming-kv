#ifndef SKV_SERVER_H
#define SKV_SERVER_H

#include <string>

class SkvServer {
  public:
    SkvServer(const std::string addr, int port);
    ~SkvServer();

    /* Starts a thread to listen for clients */
    bool start();
    void stop();

    /* Takes ownership of the value. Not using C++ array type or unique ptr to
     * allow caller to be able to send a raw byte stream from whatever source
     * without jumping through hoops */
    void set(const std::string key, void *value, size_t size);
};

#endif /* SKV_SERVER_H */
