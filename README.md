# INFO FOR SETUP MAC
Open the terminal on the folder containing run.py
Requirement to start the following command
- Python 3.13
- xcode

Passage for installing xcode:
Download xcode from the command line
xcode-slect --install

After the installation close terminal and reopen it

## creating a virtual environment if needed
sudo python3 -m venv MyEnv
source MyEnv/bin/activate
 
## building project
sudo pip install -e .

## To test if the installation has been successful try running the project
python3 run.py

# INFO FOR SETUP WINDOWS
Open the powershell terminal on the folder containing run.py
Requirement to start the following command
- Python 3.13
- Visual Studio Build Tools

Passage for installing Build Tools:
Download Build Tools from the site
https://visualstudio.microsoft.com/visual-cpp-build-tools/

In the installer check
Desktop development with C++

Then on the right panel make sure these are selected:

MSVC v143 (or newer)
Windows 10/11 SDK
C++ CMake tools for Windows

After the installation close powershell and reopen it

## creating a virtual environment if needed
python -m venv MyEnv
MyEnv/Scripts/activate.ps1
 
## building project
pip install -e .

## To test if the installation has been successful try running the project
python run.py

# EXECUTE
You can now open the run.py file into Visual Studio Code (or your preferred editor), select the environment we have just created (MyEnv), and modify the parameters inside run.py to run your experiments.

# INFO SIMULATION PARAMETERS

generatorFilepath -> string corresponding to the path of the code chosen for the simulation

FlAG_DECODING -> flag for choosing the type of the simulation. "3" value for Monte Carlo simulation. "2" value for average Belief Propagation iterations simulation. "1" value for testing of a specific error pattern. "0" value for decoding a syndrome batch.

chanDepo -> array with the physical error rates to simulate

nIterationMax -> maximum number of Monte Carlo iterations

maxErr -> maximum number of errors before stopping the Monte Carlo performance simulations

errorRate -> "CER" string for evaluating codeword error rates, "LER" string for evaluating logical error rates

NumError -> number of errors for the average Belief Propagation iterations simulation

errorX -> binary array with X errors for error pattern testing. "0" value for the correct corresponding qubit, "1" value for the faulty corresponding qubit

errorZ -> binary array with Z errors for error pattern testing. "0" value for the correct corresponding qubit, "1" value for the faulty corresponding qubit

syndrome -> binary matrix. Each row corresponds to a binary array corresponding to the current syndrome

decoderType -> "BP" string for Belief Propagation decoder, "RestartBelief" string for Restart Belief decoder, "BPOSD" string for Belief Propagation Ordered Statistics decoder, "RelayBP" string for Relay Belief Propagation decoder, "BPGD" string for Belief Propagation Guided Decimation decoder

nIterFirstBelief -> number of belief propagation iterations for the initial "BP" and "BPOSD" decoders. Number of belief propagation iterations for each belief propagation instance in "BPGD" decoder. Number of iterations for the FIRST belief propagation instance for "RestartBelief" and "RelayBP" decoders

nIterOtherBelief -> number of iterations for the SECOND to LAST belief propagation instances for "RestartBelief" and "RelayBP" decoders

alpha -> scaling factor for Belief Propagation. "int > 0" for a constant value across iterations, "-1" for alpha = 1 - 2^{-currentIteration}

eta -> eta value for "RestartBelief" decoder

lambdaOSD -> lambda value for "BPOSD" decoder

orderOSD -> OSD-order for "BPOSD" decoder

legRelayBP -> number of legs for "RelayBP" decoder

SRelayBP -> number of solutions before stopping the "RelayBP" decoder 

GammaWidth -> gamma width for "RelayBP" decoder 

GammaCenter -> gamma center for "RelayBP" decoder 


# TUTORIAL

In the Additionalfunctions.py, the function create_quantum_input_file(inputPath, generatorFilepath, decoderType, nIterationMax, maxErr, errorRate, nIterFirstBelief, alpha, eta, nIterOtherBelief, lambdaOSD, orderOSD, legRelayBP, SRelayBP, GammaWidth, GammaCenter, chanDepo) is used to create the input file.txt with the desired parameters for the simulation.

You can find in data/MatrixGen/ some .txt files containing the stabilizers of some selected QLDPC codes. In case you want to perform a simulation with different quantum codes, it is sufficient to create an analogue .txt file with your stabilizers. To aid you in this procedure we provide the file generateInput.py.

There are four possible simulation settings:
3-Monte Carlo simulation -> to evaluate the codeword (or logical) error rate as a function of the physical error rate of the selected code and decoder.
2-Average BP iterations -> to evaluate the average number of required belief propagation iterations from the selected decoder given a fixed number of error occurring in the code. This is useful to have an estimate of the complexity of the decoding algorithm.
1-Specific error pattern test -> to test if a specific error pattern given as input to the decoder in a selected code is corrected or not. The function returns also the applied correction.
0-Syndrome batch simulation -> this function takes as input a matrix of syndromes and, for each one, returns the correction applied by the selected decoder. This tool can be useful when designing a circuit simulation. Moreover, this such tool reduces the overhead associated with migrating code from Python to C++ (note that, for each call to C++ all the data structures are allocated and deallocated).
 

- In case "3" we are running a Monte Carlo simulation using the function Simulation.MonteCarloSimulation() that returns as output errProb which is an array containing the codeword (or logical) error rates estimated for the input physical error rates. Also the c++ program generates a data/output.txt file with the same information.

- In case "2" we use the function Simulation.AverageBPIterationsTesting(NumError) that takes as input the number of errors for the simulation, and gives as output NAverageBPIterations, representing the average number of belief propagation iterations required for the decoding.

- In case "1" we use the function Simulation.ErrorPatternTesting(errorX, errorZ) to test if the error pattern given as input is appropriately corrected by the decoder. The function returns the binary correction applied by the decoder (correctionX, correctionZ) and a FLAG_CORRECTED with value "1" when the pattern is corrected. 

- In case "0" we use the function Simulation.SyndromeBatchSimulation(syndrome) that takes as input a matrix of syndromes and performs the decoding for each one of them. The function returns the matrices of applied X and Z corrections in binary format for each of the processed syndromes.


# CITATION
If you find this software useful consider citing our paper:

@misc{valentini2025restartbeliefgeneralquantum,\
      title={Restart Belief: A General Quantum LDPC Decoder}, \
      author={Lorenzo Valentini and Diego Forlivesi and Andrea Talarico and Marco Chiani,\
      year={2025},\
      eprint={2511.13281},\
      archivePrefix={arXiv},\
      primaryClass={quant-ph},\
      url={[https://arxiv.org/abs/2511.13281](https://arxiv.org/abs/2511.13281)}, 
}
