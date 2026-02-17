import math

def generate_qubit_file(nqubit, k, codeDistance, ngenX, ngenZ, parity_check):
    """
    Generate a TXT file containing qubit and parity check data.
    The file name is formatted as [[nqubit,k,t]].txt
    """

    # Format the filename as requested
    filename = f"data/matrixGen/[[{nqubit},{k},{codeDistance}]].txt"

    with open(filename, 'w') as f:
        f.write(f"N_QUBIT:\n{nqubit}\n\n")
        f.write(f"K:\n{k}\n\n")
        f.write(f"t:\n{math.floor((codeDistance - 1) / 2)}\n\n")
        f.write(f"N_GEN_X:\n{ngenX}\n\n")
        f.write(f"N_GEN_Z:\n{ngenZ}\n\n")

        f.write("PARITY_CHECK:\n")
        for row in parity_check:
            f.write(" ".join(map(str, row)) + "\n")

    print(f"File '{filename}' successfully created!")


# ==============================
# Define your values here
# ==============================

nqubit = 13
k = 1
codeDistance = 3
ngenX = 6
ngenZ = 6

parity_check = [
    [1,1,0,1,0,0,0,0,0,0,0,0,0],
    [0,1,1,0,1,0,0,0,0,0,0,0,0],
    [0,0,0,1,0,1,1,0,1,0,0,0,0],
    [0,0,0,0,1,0,1,1,0,1,0,0,0],
    [0,0,0,0,0,0,0,0,1,0,1,1,0],
    [0,0,0,0,0,0,0,0,0,1,0,1,1],
    [1,0,0,1,0,1,0,0,0,0,0,0,0],
    [0,1,0,1,1,0,1,0,0,0,0,0,0],
    [0,0,1,0,1,0,0,1,0,0,0,0,0],
    [0,0,0,0,0,1,0,0,1,0,1,0,0],
    [0,0,0,0,0,0,1,0,1,1,0,1,0],
    [0,0,0,0,0,0,0,1,0,1,0,0,1]
]

# ==============================
# Generate the file
# ==============================

generate_qubit_file(nqubit, k, codeDistance, ngenX, ngenZ, parity_check)
