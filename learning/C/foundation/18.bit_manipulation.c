// Bitwise operator reference
//
// &  (AND)         : 1 if both bits are 1
// |  (OR)          : 1 if either bit is 1
// ^  (XOR)         : 1 if bits are different
// ~  (NOT)         : Flip all bits
// << (Left Shift)  : Shitfs bits left (multiple by 2^n)
// >> (Right Shift) : Shifts bits right (divives by 2^n)

#include <stdio.h>

void printBinary8(unsigned char num);

// pratical use-case
struct StandardFlags {
    unsigned int isRead;
    unsigned int isWrite;
    unsigned int isExecute;
};

struct CompactFlags {
    unsigned int isRead : 1;
    unsigned int isWrite : 1;
    unsigned int isExecute: 1;
    unsigned int mode : 3;
};


int main() {
    unsigned char flags = 0;

    // set a bit -> OR operator
    int pos = 3;
    flags |= (1 << pos);
    printf("after setting bit 3: ");
    printBinary8(flags);
    printf("\n");

    // check a bit -> AND operator
    if (flags & (1 << pos)) {
        printf("bit %d is ON.\n", pos);
    }

    // clear a bit -> AND with inverted mask
    flags &= ~(1 << pos);
    printf("after clearing bit 3: ");
    printBinary8(flags);
    printf("\n");

    // toggle a bit -> XOR operator
    flags ^= (1 << 2);
    printf("after toggling bit 2: ");
    printBinary8(flags);
    printf("\n");

    // use-case: bit-fields in structs
    printf("Size of StandardFlags: %zu bytes\n", sizeof(struct StandardFlags));
    printf("Size of CompactFlags:  %zu bytes\n", sizeof(struct CompactFlags));

    struct CompactFlags permissions = {1, 0, 1, 5};
    if (permissions.isRead) {
        printf("read permission is granted (mode: %d)\n",permissions.mode);
    }

    return 0;
}

void printBinary8(unsigned char num) {
    for (int i = 0; i < num; i++) {
        printf("%d", (num >> i) & 1);
    }
}