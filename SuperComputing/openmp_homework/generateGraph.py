from random import randint
from typing import List


def generate_graph(v: int, max_distance: int) -> List[List[int]]:
    g = []

    for i in range(v):
        g.append([])

    for i in range(v):
        for j in range(v):
            g[i].append(randint(0, max_distance))

    return g


def graph_to_text(g: List[List[int]], file: str='') -> str:
    # s = str(g).replace('], [', '],\n[').replace('[', '{').replace(']', '}')
    s = str(g).replace('], [', '\n').replace('[', '').replace(']', 'z')

    if file != '':
        with open(file, 'w') as f:
            # f.write(f'int graph[{len(g)}][{len(g[0])}] = ' + s + ';')
            f.write(s)

    return s


def generate_graph2(v: int, max_distance: int, file: str):
    with open(file, 'w') as f:
        for i in range(v):
            g = []
            for j in range(v):
                g.append(str(randint(0, max_distance)))
            f.write(' '.join(g) + '\n')


if __name__ == '__main__':
    v = int(input('V:\n'))
    max_distance = int(input('Max Distance:\n'))
    file = 'data.txt'
    generate_graph2(v, max_distance, file)
