#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * N (m,n) Queens Problem:
 * Placing N queens (chess pieces) on an mxn board so that
 * no queen threatens another.
 *
 * Limits:
 * m, n are constrained to within 32
 * When the board is m x n in size
 * colmask must be at least n bits wide
 * diagmasks at least m + n - 1 bits wide
 */

#define check_col(J) (!(colmask & (1<<(J))))
#define check_diag_a(I, J) (!(diagmask_a & (1<<((I) - (J) + n - 1))))
#define check_diag_b(I, J) (!(diagmask_b & (1<<((I) + (J)))))
#define place(I,J)   board[(I)] |= (1<<(J)); colmask |= (1<<(J)); \
        diagmask_a |= 1<<((I) - (J) + n - 1); diagmask_b |= (1<<((I) + (J))); placed++
#define unplace(I,J) board[(I)] ^= (1<<(J)); colmask ^= (1<<(J)); \
        diagmask_a ^= 1<<((I) - (J) + n - 1); diagmask_b ^= (1<<((I) + (J))); placed--

typedef unsigned int uint;
typedef unsigned long ulong;

uint board[32];
uint N, m, n;
uint colmask;
ulong diagmask_a;
ulong diagmask_b;
uint placed;

void
init (uint queens, uint rows, uint cols) {
    N = queens;
    m = rows;
    n = cols;
    colmask = 0;
    diagmask_a = 0;
    diagmask_b = 0;
    placed = 0;
    memset (board, 0, sizeof (board));
}

void
pretty_print () {
    uint i, j;
    static char header[1024] = "";
    for ( i = 0; i < m; ++i ) {
        // print header
        if (!strlen (header)) { 
            j = n;
            while (j--)
                strcpy (header + (n - j - 1) * 4, "+---");
            strcpy (header + (n - j - 1) * 4, "+\n");
        }
        printf ("%s", header);

        // print row
        for ( j = 0; j < n; ++j )
            board[i] & (1 << j) ? printf("| Q "): printf("|   ");
        printf ("|\n");
    }
    printf ("%s\n", header);
}

void
state_print () {
    uint i;
    for ( i = 0; i < m; ++i )
        printf ("%d:", board[i]);
    printf ("\n");
}

uint
backtrack (uint i) {
    uint ret = 0, j, t;
    // if N queens have been placed
    if (placed == N) { 
        state_print ();
        return 1;
    }

    // else try placing a queen in row "t" beginning with row "i"
    for ( t = i; m - t >= N - placed ; t++ ) {
        for ( j = 0; j < n; ++j ) {
            if (check_col (j) && check_diag_a (t, j) && check_diag_b (t, j)) {
                place (t, j);
                ret += backtrack (t + 1);
                unplace (t, j);
            }
        }
    }
    return ret;
}

int
main (int argc, char **argv) {

    // Example: For an 8x8 board:
    // init board with 8 rows and 8 columns
    // Of the 32 bits the last 8 are used in every row
    // and only the top 8 rows are used of the 32 total
    // 000...000[00000000] //used
    // 000...000[00000000] //used
    // ...5 more rows...
    // 000...000[00000000] //used
    // 000...00000000000 //unused
    // 000...00000000000 //unused
    // ...
    // 000...00000000000 //unused

    uint a, b, c;
    switch (argc) {
        case 1 :
            init (8, 8, 8);
            printf ("%d non-attacking queens can be placed in a %dx%d board"
                    " in %d distinct ways\n", 8, 8, 8, backtrack (0));
        break;

        case 2 :
            a = atoi (argv[1]);
            init (a, a, a);
            printf ("%d non-attacking queens can be placed in a %dx%d board"
                    " in %d distinct ways\n", a, a, a, backtrack (0));
        break;

        case 4 :
            a = atoi (argv[1]);
            b = atoi (argv[2]);
            c = atoi (argv[3]);
            init (a, b, c);
            printf ("%d non-attacking queens can be placed in a %dx%d board"
                    " in %d distinct ways\n", a, b, c, backtrack (0));
            break;
    
        default:
            printf ("Try passing <queens> <rows> <columns>"
                    "or just <queens> or nothing\n");
    }

    return 0;
}
