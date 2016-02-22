struct data{
  int a;
  char * b;
  double c;
};

struct data *ptr, mydata;
ptr = &mydata;

ptr->a = 1;
ptr->b = NULL;
ptr->c = 0.1;
