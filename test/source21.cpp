/*
 * source https://en.wikipedia.org/wiki/Method_overriding
 * */
#include <iostream>

//---------------------------------------------------------------------------
class TRectangle
{
public:
    TRectangle(double l, double w) : length(l), width(w) {}
    virtual void print() const;
 
private:
    double length;
    double width;
};
 
//---------------------------------------------------------------------------
void TRectangle::print() const
{
   // print() method of base class.
   std::cout << "Length = " << this->length << "; Width = " << this->width;
}
 
//---------------------------------------------------------------------------
class TBox : public TRectangle
{
public:
    TBox(double l, double w, double h) : TRectangle(l, w), height(h) {}
    // virtual is optional here, but it is a good practice to remind it to the developer.
    virtual void print() const;
private:
    double height;
};
 
//---------------------------------------------------------------------------
// print() method of derived class.
void TBox::print() const
{
   // Invoke parent print() method.
   TRectangle::print();
   std::cout << "; Height = " << this->height;
}
