#include <Smartinizer.h>

Smartinizer* sm;

void setup() {
    sm = new Smartinizer();
    sm->setup();
}


void loop() {
  sm->loop();
}
