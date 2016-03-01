struct data{
  int a;
  char * b;
  double c;
};

struct data *ptr, mydata;
int main ()
{
  ptr = &mydata;

  ptr->a = 1;
  ptr->b = NULL;
  ptr->c = 0.1;
}
/*
 * Comments
 * */
void __assert_rtn(const char *, const char *, int, const char *) __attribute__((__noreturn__)); //@Annotation ?
