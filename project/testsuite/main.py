import os
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def apply_tactic(tactic: str, graph_path: str) -> list:

    bash_res = os.system(f"../get_eccentricities {graph_path} {tactic}")

    if bash_res:
        print("We encountered some errors")
        exit(bash_res)

    with open("eccentricities_teexgraph.txt", 'r') as f:
        lines = f.readlines()

        res = lines[0].split(" ")

    # cleanup
    os.remove("eccentricities_teexgraph.txt")

    return res


def main():

    tactics = [#"RANDOM",
    #     "HIGH_DEGREE",
    #     "LOW_DEGREE",
    #     "BIG_DELTA",
    #     "COMMUNITY_SIZE_ASC_RANDOM",
    #     "COMMUNITY_SIZE_ASC_HIGH_DEGREE",
    #     "COMMUNITY_SIZE_ASC_LOW_DEGREE",
    #     "COMMUNITY_SIZE_ASC_BIG_DELTA",
    #     "COMMUNITY_SIZE_DSC_RANDOM",
    #     "COMMUNITY_SIZE_DSC_HIGH_DEGREE",
    #     "COMMUNITY_SIZE_DSC_LOW_DEGREE",
    #     "COMMUNITY_SIZE_DSC_BIG_DELTA",
        "COMMUNITY_RANDOM_RANDOM",
        "COMMUNITY_RANDOM_HIGH_DEGREE",
        "COMMUNITY_RANDOM_LOW_DEGREE",
        "COMMUNITY_RANDOM_BIG_DELTA"]

    if len(sys.argv) != 2:
        print("We need the path of the graph we would like to test with")

    graph_path = sys.argv[1]

    igraph_res = apply_tactic("IGRAPH", graph_path)

    for tactic in tactics:
        res = apply_tactic(tactic, graph_path)

        if igraph_res != res:
            print(f"{tactic}: {bcolors.FAIL}KO{bcolors.ENDC}\n")
        else:
            print(f"{tactic}: {bcolors.OKGREEN}OK{bcolors.ENDC}\n")




if __name__ == "__main__":
    main()
