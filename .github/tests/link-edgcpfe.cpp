// Link-only integration test: this references the real callable entry point.
// Running the front end requires application-specific arguments/environment.
#include "edgcpfe.h"

int main(int argc, char **argv)
{
  return edg::edg_main(argc, argv);
}
