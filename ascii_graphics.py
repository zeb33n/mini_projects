import sys
import time
import math
from dice import random_num


PI = 3.14159265358979


class screen:
    def __init__(self, x=10, y=10):
        self.x = x - 1
        self.y = y - 1
        self.layout = [["."] * x for _ in range(y)]

    def print_screen(self):
        self.layout.reverse()
        print("\n".join(["  ".join(row) for row in self.layout]))
        self.layout.reverse()

    def update_screen(self):
        self.layout.reverse()
        magic = "\033[F"
        screen = "\n".join(["  ".join(row) for row in self.layout])
        magic = magic * screen.count("\n")
        print(f"{magic}{screen}", end="", flush=True)
        self.layout.reverse()

    def draw_character(self, char, coords):
        x, y = coords
        if x > self.x or y > self.y or y < 0 or x < 0:
            raise ValueError("x or y is out of bounds")
        self.layout[y][x] = char


class object:
    def __init__(self, screen, char="@", start_coords=[0, 0]):
        self.screen = screen
        self.screen.draw_character(char, start_coords)
        self.loc = start_coords
        self.char = char




############## BOID ###############

class unit(object):
    def move(self, direction, speed, bounce=False):
        self.screen.draw_character(".", self.loc)
        x, y = self.loc
        self.loc = [
            int(x + math.cos(direction) * speed),
            int(y + math.sin(direction) * speed),
        ]

        if (
            0 > self.loc[0]
            or self.loc[0] > self.screen.x
            or 0 > self.loc[1]
            or self.loc[1] > self.screen.y
        ):

            if bounce:
                if 0 > self.loc[1] or self.loc[1] > self.screen.y:
                    direction = -direction
                else:
                    direction = PI - direction

                self.loc = [
                    int(x + math.cos(direction) * speed),
                    int(y + math.sin(direction) * speed),
                ]

            else:
                self.loc = [c if c < self.screen.x else 0 for c in self.loc]
                self.loc = [c if c > -1 else self.screen.x for c in self.loc]

        self.screen.draw_character(self.char, self.loc)

        return direction

    def move_wiggly(self, var=1, speed=4):
        mv_angle = 0 + random_num(-var, var, 0.1)
        self.move(mv_angle, speed)



def main(s):
    u1 = unit(s)
    print("\n" * (s.y + 1))
    mv_angle = 1
    while True:
        mv_angle += random_num(-1, 2, 0.1)
        s.update_screen()
        mv_angle = u1.move(mv_angle, speed=3, bounce=True)
        time.sleep(0.1)

main(screen(40, 40))
