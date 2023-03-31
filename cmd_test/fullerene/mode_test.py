import os

import networkx as nx
from ase.build.molecule import molecule
from fullerenedataparser.molecular.fullerene import FullereneFamily

if __name__ == '__main__':
    fullerene = molecule(name='C60')
    f = FullereneFamily(spiral=0, nospiralflag=True, atoms=fullerene)
    graph6str = nx.to_graph6_bytes(f.graph, header=False).decode().split()[0]
    CAGE_SEARCH_PATH = r'path/to/cagesearch.exe' # absolute path

    #####################  The random mode test  #################################################
    random_num = 100
    addon_num = 4
    mode = 'random'
    path_out = f'{mode}_{addon_num}addons.out'
    os.system(
        f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -o \"{path_out}\" -m \"{mode}\" -r \"{random_num}\"")

    #####################  The base mode test  #################################################
    mode = 'base'
    path_out = f'{mode}_{addon_num}addons.out'
    os.system(
        f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -o \"{path_out}\" -m \"{mode}\"")

    #####################  The step mode test  #################################################
    old_sites = [4, 12]
    mode = 'step'
    path_out = f'{mode}_{addon_num}addons.out'
    cmd_line = f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -o \"{path_out}\" -m \"{mode}\" "
    for a_site in old_sites:
        cmd_line = cmd_line + str(a_site) + '_'
    os.system(cmd_line)  # Normal

    #####################  The console mode test  #################################################
    # print site list on console
    mode = 'console'
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -m \"{mode}\"")

    #####################  The random mode test  #################################################
    # Enumerate all patterns with nothing but the number of patterns outputed.
    mode = 'stdvec'
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -m \"{mode}\"")

