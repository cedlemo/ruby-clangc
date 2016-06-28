int fn (char a)
{
  return 1;
}

#define TOTO 3

#if TOTO == 3
/* not skipped*/
#else
  /* skipped */
  int b;
#endif

#ifndef TOTO
 /* skipped */
  char *c;
#endif
