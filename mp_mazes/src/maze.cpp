/* Your code here! */

#include "maze.h"
#include "dsets.h"

#include <queue>
#include <vector>
#include <cstdlib>

using namespace std;

SquareMaze::SquareMaze() {
    w_ = 0;
    h_ = 0;
}

void SquareMaze::makeMaze(int width, int height) {
    w_ = width;
    h_ = height;

    int n = w_ * h_;

    rw_.clear();
    dw_.clear();

    rw_.resize(n, true);
    dw_.resize(n, true);

    DisjointSets dsets;
    dsets.addElements(n);

    int removed = 0;

    while (removed < n - 1) {
        int cell = rand() % n;
        int dir = rand() % 2; 

        int x = cell % w_;
        int y = cell / w_;

        if (dir == 0) { 
            if (x == w_ - 1) {
                continue;
            }

            int next = cell + 1;
            if (dsets.find(cell) != dsets.find(next)) {
                dsets.setUnion(cell, next);
                rw_[cell] = false;
                removed++;
            }
        } else {
            if (y == h_ - 1) {
                continue;
            }

            int next = cell + w_;
            if (dsets.find(cell) != dsets.find(next)) {
                dsets.setUnion(cell, next);
                dw_[cell] = false;
                removed++;
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, Direction dir) const {
    int idx = y * w_ + x;

    if (dir == RIGHT) {
        if (x >= w_ - 1) {
            return false;
        }
        return !rw_[idx];
    }

    if (dir == DOWN) {
        if (y >= h_ - 1) {
            return false;
        }
        return !dw_[idx];
    }

    if (dir == LEFT) {
        if (x <= 0) {
            return false;
        }
        return !rw_[idx - 1];
    }

    if (dir == UP) {
        if (y <= 0) {
            return false;
        }
        return !dw_[idx - w_];
    }

    return false;
}

void SquareMaze::setWall(int x, int y, Direction dir, bool exists) {
    int idx = y * w_ + x;

    if (dir == RIGHT) {
        rw_[idx] = exists;
    } else if (dir == DOWN) {
        dw_[idx] = exists;
    }
}

vector<Direction> SquareMaze::solveMaze(int startX) {
    int n = w_ * h_;

    vector<bool> v(n, false);
    vector<int> dist(n, 0);
    vector<int> parent(n, -1);
    vector<Direction> parentDir(n);

    queue<int> q;

    int start = startX;
    v[start] = true;
    q.push(start);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        int x = curr % w_;
        int y = curr / w_;

        // r
        if (canTravel(x, y, RIGHT)) {
            int next = curr + 1;
            if (!v[next]) {
                v[next] = true;
                dist[next] = dist[curr] + 1;
                parent[next] = curr;
                parentDir[next] = RIGHT;
                q.push(next);
            }
        }

        // d
        if (canTravel(x, y, DOWN)) {
            int next = curr + w_;
            if (!v[next]) {
                v[next] = true;
                dist[next] = dist[curr] + 1;
                parent[next] = curr;
                parentDir[next] = DOWN;
                q.push(next);
            }
        }

        // l
        if (canTravel(x, y, LEFT)) {
            int next = curr - 1;
            if (!v[next]) {
                v[next] = true;
                dist[next] = dist[curr] + 1;
                parent[next] = curr;
                parentDir[next] = LEFT;
                q.push(next);
            }
        }

        // u
        if (canTravel(x, y, UP)) {
            int next = curr - w_;
            if (!v[next]) {
                v[next] = true;
                dist[next] = dist[curr] + 1;
                parent[next] = curr;
                parentDir[next] = UP;
                q.push(next);
            }
        }
    }

    int best = (h_ - 1) * w_;
    for (int x = 0; x < w_; x++) {
        int idx = (h_ - 1) * w_ + x;
        if (dist[idx] > dist[best]) {
            best = idx;
        } else if (dist[idx] == dist[best] && x < (best % w_)) {
            best = idx;
        }
    }

    vector<Direction> path;
    int curr = best;

    while (curr != start) {
        path.push_back(parentDir[curr]);
        curr = parent[curr];
    }

    for (int i = 0, j = path.size() - 1; i < j; i++, j--) {
        Direction t = path[i];
        path[i] = path[j];
        path[j] = t;
    }
    return path;
}

cs225::PNG* SquareMaze::drawMaze(int start) const {
    cs225::PNG* png = new cs225::PNG(w_ * 10 + 1, h_ * 10 + 1);

    for (int x = 0; x < w_ * 10 + 1; x++) {
        if (x >= start * 10 + 1 && x <= (start + 1) * 10 - 1) {
            continue;
        }
        cs225::HSLAPixel& pixel = png->getPixel(x, 0);
        pixel.l = 0;
    }

    for (int y = 0; y < h_ * 10 + 1; y++) {
        cs225::HSLAPixel& pixel = png->getPixel(0, y);
        pixel.l = 0;
    }

    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            int idx = y * w_ + x;

            if (rw_[idx]) {
                for (int k = 0; k <= 10; k++) {
                    cs225::HSLAPixel& pixel = png->getPixel((x + 1) * 10, y * 10 + k);
                    pixel.l = 0;
                }
            }

            if (dw_[idx]) {
                for (int k = 0; k <= 10; k++) {
                    cs225::HSLAPixel& pixel = png->getPixel(x * 10 + k, (y + 1) * 10);
                    pixel.l = 0;
                }
            }
        }
    }

    return png;
}

cs225::PNG* SquareMaze::drawMazeWithSolution(int start) {
    cs225::PNG* png = drawMaze(start);
    vector<Direction> sol = solveMaze(start);

    int px = start * 10 + 5;
    int py = 5;

    int cx = start;
    int cy = 0;

    for (size_t i = 0; i < sol.size(); i++) {
        Direction d = sol[i];

        for (int k = 0; k <= 10; k++) {
            cs225::HSLAPixel& pixel = png->getPixel(px, py);
            pixel.h = 0;
            pixel.s = 1;
            pixel.l = 0.5;
            pixel.a = 1;

            if (k < 10) {
                if (d == RIGHT) {
                    px++;
                } else if (d == DOWN) {
                    py++;
                } else if (d == LEFT) {
                    px--;
                } else if (d == UP) {
                    py--;
                }
            }
        }

        if (d == RIGHT) {
            cx++;
        } else if (d == DOWN) {
            cy++;
        } else if (d == LEFT) {
            cx--;
        } else if (d == UP) {
            cy--;
        }
    }

    for (int k = 1; k <= 9; k++) {
        cs225::HSLAPixel& pixel = png->getPixel(cx * 10 + k, (cy + 1) * 10);
        pixel.l = 1;
        pixel.s = 0;
    }

    return png;
}