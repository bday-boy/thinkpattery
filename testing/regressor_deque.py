"""
A very simple program for finding the best-fit line for a set of data points.
Intended to only be used for variables that are known to have a correlation
already. Useful for things like predicting future progress.
"""
import random
from collections import deque
from typing import List, Tuple


START = 3300000
SAMPLE_SIZE = 1024
STEP_SIZE = 0.5


def DRAIN_BAT():
    return 1000 if random.random() < 0.75 else 0


def get_random_y(size: int) -> List[int]:
    Y = deque([START], maxlen=size)
    while len(Y) < size:
        Y.append(Y[-1] - DRAIN_BAT())
    return Y


class SimpleRegressor:
    def __init__(self) -> None:
        self.N = SAMPLE_SIZE
        self.X = STEP_SIZE * self.N
        self.Y = get_random_y(self.N)
        self.x_mean = self.X - (SAMPLE_SIZE * STEP_SIZE) / 2
        self.y_mean = sum(self.Y) / self.N

    @property
    def x_y_cov(self) -> float:
        cov = 0
        x_mean = self.x_mean
        y_mean = self.y_mean
        x = self.X - STEP_SIZE * self.N
        for y in self.Y:
            x += STEP_SIZE
            cov += (x - x_mean) * (y - y_mean)
        return cov

    @property
    def x_var(self) -> float:
        var = 0
        x_mean = self.x_mean
        x = self.X - STEP_SIZE * self.N
        for n in range(self.N):
            x += STEP_SIZE
            var += (x - x_mean) ** 2
        return var

    def update(self, p_new: int) -> None:
        p_old = self.Y.popleft()
        self.Y.append(p_new)
        self.X += STEP_SIZE
        self.x_mean = self.X - (SAMPLE_SIZE * STEP_SIZE) / 2
        self.y_mean += (p_new - p_old) / self.N

    def find_best_fit(self) -> Tuple[int, int]:
        m = self.x_y_cov / self.x_var
        b = self.y_mean - m * self.x_mean
        print(f'y = {m:.2f}x + {b:.2f}')
        return m, b

    def find_x_zero(self, y: int) -> int:
        m, b = self.find_best_fit()
        if m == 0:
            return -1
        return (y - b) / m

    def find_remaining_time(self, y: int = 0) -> int:
        remaining_time = int(self.find_x_zero(y) - self.X)
        print(f'{remaining_time} seconds left')
        self.print_info()

    def print_info(self) -> None:
        # print(self.Y)
        print(f'Avg diff: {(sum(self.Y) / self.N) - self.y_mean}')


if __name__ == '__main__':
    lms = SimpleRegressor()
    while True:
        lms.find_remaining_time()
        lms.update(lms.Y[-1] - DRAIN_BAT())
        if input('Quit?'):
            exit(0)
