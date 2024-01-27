import time

class rng():
    def __init__(self, start = 0b10000001) -> None:
        self.binary_string = ""
        self._generate(start)


    def generate_random_number(self, seed = None, max = 100):
        if not seed:
            seed = int(str(time.time())[-3:]) # find a better way to generate seeds
        if len(bin(max)) - 2 > 4:
            self._generate(binary_str_len = len(bin(max)) - 1)

        low_bin_str_indexer = seed 
        high_bin_str_indexer = seed + len(bin(max)) - 2
        bin_str = self.binary_string[low_bin_str_indexer: high_bin_str_indexer]

        time.sleep(0.0000001)

        return (int(bin_str, 2) % max) + 1


    def _generate(self, i = 0b10000001, binary_str_len = 1000):
        bin_len = len(bin(i)) - 2     
        for _ in range(binary_str_len):
            n = (i ^ (i>>1)) & 1
            i = (i>>1) | (n<<bin_len)
            self.binary_string += str(i & 1)

def roll_D(max):
    die = rng()
    print(die.generate_random_number(max = max))


def random_num(min, max, step = 1, seed = None):
    generator = rng()
    min = min - step 
    gmax = round((max - min) / step)
    num = generator.generate_random_number(max = gmax, seed = seed)
    return min + (step * num)