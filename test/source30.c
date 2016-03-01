struct data{
  int a;
  char * b;
  double c;
};

struct data mydata;

int main()
{
  struct data *ptr;
  ptr = & mydata;

  ptr-> = 1;
  ptr->b = NULL;
  ptr->c = 0.1;
}
