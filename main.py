from sys import argv
import psyco
psyco.full()

def print_board(b):
    for i in b:
        print '%s%s' % ('+---' * len(b[0]), '+')
        for j in i:
            if j:
                print '| Q',
            else:
                print '|  ',
        print '|'
    print '%s%s\n' % ('+---' * len(b[0]), '+')

def queen_count(b):
    return sum((sum(i) for i in b))

# can place a queen at (i, j) in board?
# only checks for column and diag consistency
# (since row consistency is ensured by try_backtrack)
def is_valid(board, i, j):
    if sum(zip(*board)[j]):
        return False
    l = len(board)
    for k in range(i):
        m, n = j - i + k, j + i - k
        if (-1 < m < l and board[k][m]) or (-1 < n < l and board[k][n]):
            return False
    return True
    
def try_backtrack(board, placed = 0):

    l = len(board)
    if placed == l:
        print_board(board)
        return 1

    #place a queen in row = placed
    ret = 0
    for j in range(l):
        if is_valid(board, placed, j):
            board[placed][j] = 1
            ret = ret + try_backtrack(board, placed + 1)
            board[placed][j] = 0
    return ret

if __name__ == "__main__":
    N = 8
    if len(argv) == 2 :
        N = int(argv[1])
    print try_backtrack([[0 for i in range(N)] for j in range(N)])
