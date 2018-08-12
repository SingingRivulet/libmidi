#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "midi-relative.h"

int lastchannel=-1;

static void usage(const char *prog)
{
  printf("usage: %s <file.midi> <out>\n", prog);
}
static void dump_midi(int note , int channel , void * arg)
{
  FILE * o=(FILE*)arg;
  if(channel!=lastchannel){
    lastchannel=channel;
    fprintf(o,"\nchannel:%d\n",channel);
  }
  fprintf(o,"%d ",note);
}
static int parse_file(const char *path,const char *out)
{
  struct stat st;

  if (stat(path, &st)) {
    printf("stat(%s): %m\n", path);
    return 1;
  }

  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    printf("open(%s): %m\n", path);
    return 1;
  }
  
  FILE * o=NULL;
  o=fopen(out,"w");
  if(o==NULL)
    return 1;

  void *mem = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (mem == MAP_FAILED) {
    printf("mmap(%s): %m\n", path);
    close(fd);
    return 1;
  }

  struct midi_parser parser;
  parser.state = MIDI_PARSER_INIT;
  parser.size  = st.st_size;
  parser.in    = mem;

  midi_relative_parse(&parser,dump_midi,o);

  munmap(mem, st.st_size);
  close(fd);
  fclose(o);
  return 0;
}

int main(int argc, char **argv)
{
  if (argc != 3) {
    usage(argv[0]);
    return 1;
  }

  return parse_file(argv[1],argv[2]);
}
