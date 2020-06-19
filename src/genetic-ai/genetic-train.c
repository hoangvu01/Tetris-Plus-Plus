#include <train.h>

int main(int argc, char **argv) {
  train(argc == 2 ? argv[1] : NULL);

  return EXIT_SUCCESS;
}
