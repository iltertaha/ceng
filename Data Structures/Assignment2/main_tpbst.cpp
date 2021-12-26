#include <iostream>

#include "tpbst.hpp"

int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.print()
            .print("ceng213")
            .print("ceng213", "sec1")
            .insert("ceng213", "24", 24)
            .insert("ceng213", "27", 27)
            .insert("ceng213", "22", 22)
			.remove("ceng213","27")/*
            .insert("ceng213", "25", 25)
            .insert("ceng213", "26", 26)
            .remove("ceng213","25")*/

            /* .insert("ceng213", "sec1", 25)
            .insert("ceng213", "sec3", 24)
            .insert("ceng477", "sec2", 50)
            .insert("ceng477", "sec1", 49)
            .print()
            .print("ceng213")
            .print("ceng477")
            .print("ceng213", "sec1")
            .print("ceng477", "sec1")
            .remove("ceng213", "sec2")
            .remove("ceng477", "sec2")*/
            .print(); 

    //std::cout << "Number of enrolled students in ceng213 - sec3 is " << *(tpbst.find("ceng213", "sec3")) << std::endl;

    return 0;
}
