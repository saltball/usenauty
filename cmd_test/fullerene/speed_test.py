import os

import networkx as nx
from ase.build.molecule import molecule
from fullerenedataparser.molecular.fullerene import FullereneFamily
import time

if __name__ == '__main__':
    fullerene = molecule(name='C60')
    f = FullereneFamily(spiral=0, nospiralflag=True, atoms=fullerene)
    graph6str = nx.to_graph6_bytes(f.graph, header=False).decode().split()[0]
    CAGE_SEARCH_PATH = r'path/to/cagesearch.exe' # absolute path

    #####################  The time to file test  #################################################
    mode = 'base'
    filename = 'C60_000001812opted'
    # For test of other numbers, put them into the list below.
    for addon_num in [3]:
        start = time.time()
        path_out = f'{mode}_{filename}_{addon_num}addons.out'
        os.system(
            f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -o \"{path_out}\" -m \"{mode}\"")
        stop = time.time()
        time_cost = stop - start
        print(f'Total time cost is {time_cost} s.')







