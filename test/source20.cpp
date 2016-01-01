template <typename T, int kInt, bool kBool>
void foo() {};

template <>
void foo<float, 2147483649, true>();
