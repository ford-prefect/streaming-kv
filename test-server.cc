#include <memory>

#include "skv-server.h"

using namespace std;

int main(void)
{
  SkvServer *server = new SkvServer("localhost", 4321);

  return 0;
}
