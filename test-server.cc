#include <iostream>
#include <memory>

#include "skv-server.h"

using namespace std;

int main(void)
{
  auto server = make_unique<SkvServer>();

  if (!server->start("127.0.0.1", 4321))
    return -1;

  while (true) {
    string key;
    string value;

    cout << "Key to update: ";
    cin >> key;
    cout << "Value: ";
    cin >> value;

    server->set(key, (const byte *) value.c_str(), value.length());
  }

  server->stop();

  return 0;
}
