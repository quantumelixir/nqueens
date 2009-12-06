#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define check_col(J) (!(colmask & (1<<(J))))
#define place(I,J)   board[(I)] |= (1<<(J)); colmask |= (1<<(J)); placed++;
#define unplace(I,J) board[(I)] ^= (1<<(J)); colmask ^= (1<<(J)); placed--;

int board[32];
int N, m, n;
int colmask;
int placed;

void
init (int queens, int rows, int cols) {
    N = queens;
    m = rows;
    n = cols;
    colmask = 0;
    placed = 0;
    memset (board, 0, sizeof (board));
}

void
pretty_print () {
    int i, j;
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
    int i;
    for ( i = 0; i < m; ++i )
        printf ("%d:", board[i]);
    printf ("\n");
}

/*
 * Checks along diagonals (from rows 0 to i)
 * for empty squares (non-queens)
 */
int
check_diag (int i, int j) {
    int p, q, k;
    for ( k = 0; k < i; ++k ) {
        p = j - i + k;
        q = j + i - k;
        if ((p >= 0 && (board[k] & (1<<p))) ||
            (q < n && (board[k] & (1<<q)))) return 0;
    }
    return 1;
}

unsigned int
backtrack (int i) {
    int ret = 0, j, t;
    // if N queens have been placed
    if (placed == N) { 
        state_print ();
        return 1;
    }

    // else try placing a queen in row "t" beginning with row "i"
    for ( t = i; m - t >= N - placed ; t++ ) {
        for ( j = 0; j < n; ++j ) {
            if (check_col (j) && check_diag (t, j)) { 
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

    int a, b, c;
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
