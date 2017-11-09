/* port.c */

#include "erl_comm.hpp"
#include "bg_sub.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

typedef unsigned char byte;

int main() {
  int fn, arg, res;
  float trigger;
  byte buf[100];

  // We just read commands from standard input as long as we can...
  while (read_cmd(buf) > 0) {
    // Once we've gotten a command, we know the function is determined by the
    // first byte and its argument is the second byte
    fn = buf[0];
    arg = buf[1];

    if (fn == 1) {
      // We said that 1 was foo
      res = startup();
      // We set the response value in the first byte of our buffer
      buf[0] = fn;
      // and we write that byte back to the elixir program over standard out
      write_cmd(buf, 1);
    } else if (fn == 2) {
      // and 2 was bar
      res = shutdown();
      // We set the response value in the first byte of our buffer
      buf[0] = fn;
      // and we write that byte back to the elixir program over standard out
      write_cmd(buf, 1);
    } else if (fn == 3) {
      // and 2 was bar

      trigger = processVideo();
      // We set the response value in the first byte of our buffer
      //byte img_buf[img.size() + 4];
      byte img_buf[4];

      img_buf[0] = 3;
      img_buf[1] = trigger*100;
      // and we write that byte back to the elixir program over standard out
/*      int i=3;
      for (std::vector<uchar>::iterator it = img.begin() ; it != img.end(); ++it){
          img_buf[++i] = *it;
      }*/
      write_cmd(img_buf, 4);
    } else if (fn == 4) {
      // and 2 was bar
      res = setVarThreshold(arg);
      // We set the response value in the first byte of our buffer
      // and we write that byte back to the elixir program over standard out
      buf[0] = fn;
      write_cmd(buf, 1);


    }
  }
}
