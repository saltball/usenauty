import os

import networkx as nx
from ase.build import graphene_nanoribbon
from ase.visualize import view
from fullerenedataparser.molecular.fullerene import FullereneFamily
import time

if __name__ == '__main__':
    atoms = graphene_nanoribbon(n=3, m=5)
    view(atoms)

    f = FullereneFamily(spiral=0, nospiralflag=True, atoms=atoms)
    graph6str = nx.to_graph6_bytes(f.graph, header=False).decode().split()[0]
    CAGE_SEARCH_PATH = r'path/to/cagesearch.exe' # absolute path

    #####################  The console mode  #################################################
    # print site list on console
    mode = 'console'
    addon_num = 3
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -m \"{mode}\"")

