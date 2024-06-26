import time
import math
from dice import random_num


PI = 3.14159265358979


class Screen:
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


############## GAME OF LIFE ###############
class GameOfLife:
    def __init__(self, screen: Screen):
        self._screen = screen
        # self._screen.print_screen()
        while True:
            time.sleep(0.1)
            cell_calcs = self.calc_cells()
            self.apply_rules(cell_calcs)
            self._screen.update_screen()

    def count_neighbours(self, x: int, y: int) -> int:
        check_coords = (
            (0, 1),
            (0, -1),
            (1, 0),
            (-1, 0),
            (1, 1),
            (1, -1),
            (-1, 1),
            (-1, -1),
        )
        coords = [
            (x + cx, y + cy)
            for cx, cy in check_coords
            if x + cx >= 0
            and x + cx <= self._screen.x
            and y + cy >= 0
            and y + cy <= self._screen.y
        ]
        count = 0
        for cx, cy in coords:
            if self._screen.layout[cy][cx] == ".":
                continue
            count += 1
        return count

    def calc_cells(self) -> list[tuple[str, int, int, int]]:
        return [
            (cell, self.count_neighbours(x, y), x, y)
            for y, row in enumerate(self._screen.layout)
            for x, cell in enumerate(row)
        ]

    def apply_rules(self, cell_calcs: list[tuple[str, int]]) -> None:
        for cell, count, x, y in cell_calcs:
            if "@" == cell and count < 2:
                self._screen.draw_character(".", (x, y))
            elif "@" == cell and 2 <= count <= 3:
                pass
            elif "@" == cell and count > 3:
                self._screen.draw_character(".", (x, y))
            elif "." == cell and count == 3:
                self._screen.draw_character("@", (x, y))


def main_conways() -> None:
    screen = Screen(50, 50)
    print("\n" * (screen.y + 1))
    screen.draw_character("@", (25, 25))
    screen.draw_character("@", (25, 26))
    screen.draw_character("@", (25, 24))
    screen.draw_character("@", (24, 25))
    screen.draw_character("@", (26, 26))
    screen.update_screen()
    GameOfLife(screen)


############## BOID ###############


class object:
    def __init__(self, screen, char="@", start_coords=[0, 0]):
        self.screen = screen
        self.screen.draw_character(char, start_coords)
        self.loc = start_coords
        self.char = char


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


def main_boid(s):
    u1 = unit(s)
    print("\n" * (s.y + 1))
    mv_angle = 1
    while True:
        mv_angle += random_num(-1, 2, 0.1)
        s.update_screen()
        mv_angle = u1.move(mv_angle, speed=3, bounce=True)
        time.sleep(0.1)


if __name__ == "__main__":
    main_conways()
# main_boid(Screen(40, 40))
