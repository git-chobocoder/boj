import sys
input = sys.stdin.readline

def mex(S):
    for num, i in zip(sorted(S), range(len(S))):
       if num != i:
           return i
    return len(S)

N, M = map(int, input().split())

MAP = [[0 for _ in range(25)] for _ in range(25)]
for i in range(N):
    row = input().rstrip()
    for j in range(M):
        MAP[i][j] = row[j]

game1, game2 = [[0 for _ in range(25)] for _ in range(25)], [[0 for _ in range(25)] for _ in range(25)]
for s in range(49):
    new_row = s//2
    for col in range(s+1):
        row = s-col
        if row < 0 or row >= 25: continue
        if col < 0 or col >= 25: continue
        if s % 2 == 0:
            game1[new_row][12-new_row+col] = MAP[row][col]
        else:
            game2[new_row][12-new_row+col] = MAP[row][col]

minr, minc, maxr, maxc = 26, 26, -1, -1
for r in range(25):
    for c in range(25):
        if game1[r][c] != 0:
            minr, maxr = min(minr, r), max(maxr, r)
            minc, maxc = min(minc, c), max(maxc, c)
game1 = [row[minc:maxc+1] for row in game1[minr:maxr+1]]
minr, minc, maxr, maxc = 26, 26, -1, -1
for r in range(25):
    for c in range(25):
        if game2[r][c] != 0:
            minr, maxr = min(minr, r), max(maxr, r)
            minc, maxc = min(minc, c), max(maxc, c)
game2 = [row[minc:maxc+1] for row in game2[minr:maxr+1]]

R = len(game1)
try:
    C = len(game1[0])
except IndexError:
    C = 0
    
DP1 = [[[[-1 for _ in range(C)]
         for _ in range(C)]
         for _ in range(R)]
         for _ in range(R)]

for r1 in range(R):
    for r2 in range(r1):
        for c1 in range(C):
            for c2 in range(C):
                DP1[r1][r2][c1][c2] = 0
for r1 in range(R):
    for r2 in range(R):
        for c1 in range(C):
            for c2 in range(c1):
                DP1[r1][r2][c1][c2] = 0

def dfs1(r1, r2, c1, c2):
    if r1 > r2 or c1 > c2:
        return 0
    if DP1[r1][r2][c1][c2] != -1:
        return DP1[r1][r2][c1][c2]
    if r1 == r2 and c1 == c2:
        if game1[r1][c1] == 0:
            DP1[r1][r2][c1][c2] = 0
        else:
            DP1[r1][r2][c1][c2] = 1
        return DP1[r1][r2][c1][c2]
  
    ret = set()
    for r in range(r1, r2+1):
        for c in range(c1, c2+1):
            if game1[r][c] == 0:
                continue
            if game1[r][c] == 'L':
                ret.add(dfs1(r1, r-1, c1, c2) ^ dfs1(r+1, r2, c1, c2))
            elif game1[r][c] == 'R':
                ret.add(dfs1(r1, r2, c1, c-1) ^ dfs1(r1, r2, c+1, c2))
            else:
                ret.add(dfs1(r1, r-1, c1, c-1) ^ dfs1(r+1, r2, c1, c-1) ^ dfs1(r1, r-1, c+1, c2) ^ dfs1(r+1, r2, c+1, c2))
    DP1[r1][r2][c1][c2] = mex(ret)
    return DP1[r1][r2][c1][c2]
game1_grundy = dfs1(0, R-1, 0, C-1)

R = len(game2)
try:
    C = len(game2[0])
except IndexError:
    C = 0

DP2 = [[[[-1 for _ in range(C)]
         for _ in range(C)]
         for _ in range(R)]
         for _ in range(R)]

for r1 in range(R):
    for r2 in range(r1):
        for c1 in range(C):
            for c2 in range(C):
                DP2[r1][r2][c1][c2] = 0
for r1 in range(R):
    for r2 in range(R):
        for c1 in range(C):
            for c2 in range(c1):
                DP2[r1][r2][c1][c2] = 0

def dfs2(r1, r2, c1, c2):
    if r1 > r2 or c1 > c2:
        return 0
    # [-] 이미 존재하는 경우
    if DP2[r1][r2][c1][c2] != -1:
        return DP2[r1][r2][c1][c2]
    # [-] 존재하지 않지만 같은 경우
    if r1 == r2 and c1 == c2:
        if game2[r1][c1] == 0:
            DP2[r1][r2][c1][c2] = 0
        else:
            DP2[r1][r2][c1][c2] = 1
        return DP2[r1][r2][c1][c2]
    # [-] 존재하지도 않고 같지도 않은 경우
    ret = set()
    for r in range(r1, r2+1):
        for c in range(c1, c2+1):
            if game2[r][c] == 0:
                continue
            if game2[r][c] == 'L':
                ret.add(dfs2(r1, r-1, c1, c2) ^ dfs2(r+1, r2, c1, c2))
            elif game2[r][c] == 'R':
                ret.add(dfs2(r1, r2, c1, c-1) ^ dfs2(r1, r2, c+1, c2))
            else:
                ret.add(dfs2(r1, r-1, c1, c-1) ^ dfs2(r+1, r2, c1, c-1) ^ dfs2(r1, r-1, c+1, c2) ^ dfs2(r+1, r2, c+1, c2))
    DP2[r1][r2][c1][c2] = mex(ret)
    return DP2[r1][r2][c1][c2]
game2_grundy = dfs2(0, R-1, 0, C-1)

# [-] 정답 출력
ans = game1_grundy^game2_grundy
if ans > 0:
    print('koosaga')
else:
    print('cubelover')