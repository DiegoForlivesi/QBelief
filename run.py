import os
from src import Simulation
from src import AdditionalFunctions as Utils
import matplotlib.pyplot as plt

#######################################################################################################################################################
####> PATH SETTINGS <####
BASE_DIR   = os.path.dirname(os.path.abspath(__file__))
inputPath = os.path.join(BASE_DIR, "data", "fileIn.txt")
#######################################################################################################################################################




#######################################################################################################################################################
####> INPUT PARAMETERS <####
generatorFilepath = "data/MatrixGen/[[144,12,12]]_grossCode.txt"            # Select your code!

FlAG_DECODING = 3                                   # =3 for Monte Carlo Simulation; =2 for average BP iterations Simulation; =1 for Error Pattern Testing; =0 for Syndrome Batch Simulation; 
chanDepo = [0.01, 0.005]                      # List of Physical Error Rates
nIterationMax = 1e9                                # Monte Carlo Iterations
maxErr = 100                                        # Errors to find before stopping the performance simulation
errorRate = "CER"                                   # CER -> Codeword Error Rates; LER -> Logical Error Rates
NumError = 4                                        # number of errors for the average BP iterations Simulation                                  
errorX = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]    # X error pattern for the Error Pattern Testing (e.g., for the [[13,1,3]] surface code)
errorZ = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]    # Z error pattern for the Error Pattern Testing (e.g., for the [[13,1,3]] surface code)
syndrome = [[1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0], [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0], [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1]]     # synrome matrix for batch syndrome simulation (e.g., for the [[13,1,3]] surface code)

###\begin{Decoder Settings}
decoderType = "RestartBelief"            # Select your decoder --> BP: Belief Propagation, Restart Belief: RestartBelief, BPOSD: Ordered Statistics Decoding, 
                                         # RelayBP: Relay Belief Propagation, BPGD: Belief Propagation Guided Decimation
nIterFirstBelief = 50                    # Iterations of the first Belief Propagation
nIterOtherBelief = 10                    # Iterations of the second to last Belief Propagations
alpha = -1                               # Scaling factor: -1 -> alpha that depends on the number of iterations; alpha > 0 -> alpha is set constant 
# Settings for Restart Belief
eta = 35                                 # Number of max restarts
# Settings for Ordered Statistics Decoding:
lambdaOSD = 10                           # Number of extra redundancy columns for OSD of order higher than 0
orderOSD = 2                             # Order of the OSD decoder: can be set between 0 and 2 
# Settings for Relay Belief Propagation: 
legRelayBP = 301                         # Maximum number of BP instances 
SRelayBP = 5                             # Maximum number of solutions to terminate
GammaWidth = 0.66                        # Gamma width: interval of the uniform distribution
GammaCenter = -0.24                      # Gamma center: center of the uniform distribution 
###\end{Decoder Settings}
# Note: for more details on the settings see README.md
#######################################################################################################################################################




#######################################################################################################################################################
####> EXECUTION <####

# Create the input file from the setting provided above
Utils.create_quantum_input_file(inputPath, generatorFilepath, decoderType, nIterationMax, maxErr, errorRate, nIterFirstBelief, alpha, eta, nIterOtherBelief, lambdaOSD, orderOSD, legRelayBP, SRelayBP, GammaWidth, GammaCenter, chanDepo)


if FlAG_DECODING == 3:                  # Monte Carlo Simulation
    errProb = Simulation.MonteCarloSimulation() 
    #[errorsX, correctionsX, errorsZ, correctionsZ, errProb, totSimulationPerPhysicalErrorRate] = Simulation.MonteCarloSimulationDebug()
    # plt.figure()
    # plt.loglog(chanDepo, errProb, marker='o')
    # plt.xlabel('Physical Error Rate')
    # if errorRate == "LER":
    #     plt.ylabel('Logical Error Rate')
    # else:
    #     plt.ylabel('Codeword Error Rate')
    # plt.show()
    print(f"The error probabilities are: {errProb}")
elif FlAG_DECODING == 2:                # Average BP iterations
    NAverageBPIterations = Simulation.AverageBPIterationsTesting(NumError)
    print(f"The average number of iterations is: {NAverageBPIterations}")
elif FlAG_DECODING == 1:                # Specific Error Pattern Testing
    [correctionX, correctionZ, FLAG_CORRECTED] = Simulation.ErrorPatternTesting(errorX, errorZ) 
    if FLAG_CORRECTED == 1:
        print("The error pattern has been corrected")
    else:
        print("The error pattern has NOT been corrected")
    print(f"The applied X correction is: {correctionX}")
    print(f"The applied Z correction is: {correctionZ}")
elif FlAG_DECODING == 0:                # syndrome batch
    [correctionMatrixX, correctionMatrixZ] = Simulation.SyndromeBatchSimulation(syndrome) 
    print(f"The applied X correction is: {correctionMatrixX}")
    print(f"The applied Z correction is: {correctionMatrixZ}")
#######################################################################################################################################################
