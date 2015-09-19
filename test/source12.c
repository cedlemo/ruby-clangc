// bit field box properties
struct box_props
{
     unsigned int opaque       : 1;
     unsigned int fill_color   : 3;
     unsigned int              : 4; // fill to 8 bits
     unsigned int show_border  : 1;
     unsigned int border_color : 3;
     unsigned int border_style : 2;
     unsigned int              : 0; // fill to nearest byte (16 bits)
     unsigned char width       : 4, // Split a byte into 2 fields of 4 bits
                   height      : 4;
};
