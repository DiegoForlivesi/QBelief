import os

def create_quantum_input_file(
    inputPath,
    generatorFilepath,
    decoderType,
    nIterationMax,
    maxErr,
    errorRate,
    nIterFirstBelief,
    alpha,
    eta,
    nIterOtherBelief,
    lambdaOSD,
    orderOSD,
    legRelayBP,
    SRelayBP,
    GammaWidth,
    GammaCenter,
    chanDepo
):
    """
    Create a quantum input file with the given parameters.
    """
    
    # Ensure folder exists
    os.makedirs(os.path.dirname(inputPath), exist_ok=True)

    # Convert chanDepo list to space-separated string
    chanDepo_str = " ".join(str(x) for x in chanDepo)

    # Write the file
    with open(inputPath, "w") as f:
        f.write("//                           QBelief V1.0                           //\n\n")
        
        f.write(">> QUANTUM_CODE\n")
        f.write(f"\tGENERATOR_FILEPATH: {generatorFilepath}\n\n")
        
        f.write(">> DECODER\n")
        f.write(f"\tDECODER_TYPE: {decoderType}\n\n")
        
        f.write(">> MONTECARLO\n")
        f.write(f"\tnIterationMax: {nIterationMax}\n")
        f.write(f"\tmaxErr: {maxErr}\n")
        f.write(f"\terrorRate: {errorRate}\n\n")

        f.write(">> BELIEF PROPAGATION\n")
        f.write(f"\tnIterFirstBelief: {nIterFirstBelief}\n")
        f.write(f"\tnIterOtherBelief: {nIterOtherBelief}\n")
        f.write(f"\talpha: {alpha}\n")
        f.write(f"\teta: {eta}\n")
        f.write(f"\tlambdaOSD: {lambdaOSD}\n")
        f.write(f"\torderOSD: {orderOSD}\n")
        f.write(f"\tlegRelayBP: {legRelayBP}\n")
        f.write(f"\tSRelayBP: {SRelayBP}\n")
        f.write(f"\tGammaWidth: {GammaWidth}\n")
        f.write(f"\tGammaCenter: {GammaCenter}\n")
        
        f.write(">> CHANNEL\n")
        f.write(f"\tnInputParam: {len(chanDepo)}\n")
        f.write(f"\tchanDepo: {chanDepo_str} EndParams\n\n")
        
        f.write(">> EOF\n")

    print(f"Input file successfully created at {inputPath}")

