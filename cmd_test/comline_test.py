import networkx as nx
from fullerenedatapraser.molecular.fullerene import FullereneFamily
from fullerenedatapraser.io.xyz import simple_read_xyz_xtb
from fullerenedatapraser.io.recursion import recursion_files
import os


SOURCE_ROOT = r'./'
path_cage = r'./'
addon_num = 4

CAGE_SEARCH_PATH = os.path.abspath(r'../bin/cagesearch.exe')


def get_one_atoms(source_root_path):
    for item in recursion_files(source_root_path, ignore_mode=True):
        s_atoms = list(simple_read_xyz_xtb(item, read_comment=False))[0]
        yield {"atoms": s_atoms, "name": os.path.splitext(os.path.basename(item))[0]}


for result in get_one_atoms(source_root_path=SOURCE_ROOT):
    s_atoms = result["atoms"]
    filename = result["name"]
    f = FullereneFamily(spiral=0, atoms=s_atoms)
    graph6str = nx.to_graph6_bytes(f.graph, header=False).decode().split()[0]

    mode = 'random'
    path_out = os.path.join(path_cage, f'{mode}_{filename}_{addon_num}addons.out')
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -o \"{path_out}\" -m \"{mode}\" -r 100")

    mode = 'base'
    path_out = os.path.join(path_cage, f'{mode}_{filename}_{addon_num}addons.out')
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -o \"{path_out}\" -m \"{mode}\"")

    mode = 'step'
    path_out = os.path.join(path_cage, f'{mode}_normal_{filename}_{addon_num}addons.out')
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -o \"{path_out}\" -m \"{mode}\" 17 12") # Normal
    addon_num = 6  # There are no patterns in addon_num=4, for 1812_C60, 59 58
    path_out = os.path.join(path_cage, f'{mode}_caution_abnormal_{filename}_{addon_num}addons.out')
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum  \"{addon_num}\"  -o \"{path_out}\" -m \"{mode}\" 59 58") # The situation of old color[i] > n-3, the completeness is ensured.

    mode = 'console'
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -m \"{mode}\"")

    mode = 'stdvec'
    os.system(f"{CAGE_SEARCH_PATH} --graph6str \"{graph6str}\" --addnum \"{addon_num}\" -m \"{mode}\"")
