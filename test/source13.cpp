template <typename T, int kInt, bool kBool>
void foo() {};

template <>
void foo<float, -7, true>();
