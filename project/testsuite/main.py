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


def apply_tactic(tactic: str, delta: int, graph_path: str) -> list:

    bash_res = os.system(f"../get_eccentricities {graph_path} {tactic} {delta}")

    if bash_res:
        print("We encountered some errors")
        exit(bash_res)

    with open("eccentricities.txt", 'r') as f:
        lines = f.readlines()

        res = lines[0].split(" ")

    # cleanup
    os.remove("eccentricities.txt")

    return res

def get_percentage_likelyhood(igraph_res, res):
    if len(igraph_res) != len(res):
        print("We did not count the same amount of eccentricities")
        return 0

    num_same = 0

    for i in range(len(igraph_res)):
        if igraph_res[i] == res[i]:
            num_same += 1

    return (num_same / len(igraph_res)) * 100


def main():
    delta = 2
    tactics = [
        "RANDOM",
        "HIGH_DEGREE",
        "LOW_DEGREE",
        "BIG_DELTA",
        "COMMUNITY_SIZE_ASC_RANDOM",
        "COMMUNITY_SIZE_ASC_HIGH_DEGREE",
        "COMMUNITY_SIZE_ASC_LOW_DEGREE",
        "COMMUNITY_SIZE_ASC_BIG_DELTA",
        "COMMUNITY_SIZE_DSC_RANDOM",
        "COMMUNITY_SIZE_DSC_HIGH_DEGREE",
        "COMMUNITY_SIZE_DSC_LOW_DEGREE",
        "COMMUNITY_SIZE_DSC_BIG_DELTA",
        "COMMUNITY_RANDOM_RANDOM",
        "COMMUNITY_RANDOM_HIGH_DEGREE",
        "COMMUNITY_RANDOM_LOW_DEGREE",
        "COMMUNITY_RANDOM_BIG_DELTA"]

    if len(sys.argv) != 2:
        print("We need the path of the graph we would like to test with")

    graph_path = sys.argv[1]

    print("We try IGRAPH method")
    igraph_res = [] #apply_tactic("IGRAPH", delta, graph_path)

    print("We now try all our tactics:")

    num_success = 0
    for tactic in tactics:
        res = apply_tactic(tactic, delta, graph_path)

        percentage_likelyhood = get_percentage_likelyhood(igraph_res, res)

        if percentage_likelyhood != 100:
            print(f"{tactic}: {bcolors.FAIL}KO{bcolors.ENDC}")
            print("We got: {:.2f}% of good eccentricities\n".format(percentage_likelyhood))
        else:
            print(f"{tactic}: {bcolors.OKGREEN}OK{bcolors.ENDC}\n")
            num_success += 1

    if num_success != len(tactics):
        print(f"SUMMARY: We have some {bcolors.FAIL}KO{bcolors.ENDC}\n")




if __name__ == "__main__":
    main()
