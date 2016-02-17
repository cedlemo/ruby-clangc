//http://stackoverflow.com/questions/5510922/template-class-specialization
template<typename T>
class stack
{
  //implementation goes here
};

template<>
class stack<int>
{
 public:
  std::vector<int> stackVector;
};
