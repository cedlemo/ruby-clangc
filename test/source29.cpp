template <typename T>
class optional
{
  // ...
  T&       value() &;
  T&&      value() &&;
  T const& value() const&;
};
