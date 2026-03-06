#include "DecodersBP.h"

namespace py = pybind11;

DecoderSimulation::DecoderSimulation(const char* fileName) {

    std::string tmpStr;
    double tmpDouble;

    std::ifstream fileIn;
    fileIn.open(fileName, std::ios::in);
    if (!fileIn.is_open()) {
        std::cout << "[ERROR]: The application is unable to open the file " << fileName << " (DecoderSimulation Init).\nExiting..." << std::endl;
        exit(0);
    }

    std::cout << std::endl << std::endl;
    std::cout << "// ******************************************** //" << std::endl;
    std::cout << "//            DecoderSimulation            //" << std::endl;
    std::cout << "// ******************************************** //" << std::endl;
    std::cout << std::endl;
    std::cout << "[INFO]: Reading Input File...\n";

    // Generators and FT circuit
    while (!fileIn.eof()) {
        std::getline(fileIn, tmpStr);
        if (tmpStr == ">> QUANTUM_CODE") {
            break;
        }
    }
    fileIn >> tmpStr;
    if (tmpStr == "GENERATOR_FILEPATH:") {
        fileIn >> m_generatorFileName;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (GENERATOR_FILEPATH field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    
    // Decoding parameters
    while (!fileIn.eof()) {
        std::getline(fileIn, tmpStr);
        if (tmpStr == ">> DECODER") {
            break;
        }
    }
    fileIn >> tmpStr;
    if (tmpStr == "DECODER_TYPE:") {
        fileIn >> tmpStr; 
        if (tmpStr == "RestartBelief") {
            decType = DecoderType::RestartBelief;
		} else if (tmpStr == "BP") {
            decType = DecoderType::BP;
        } else if (tmpStr == "BPOSD") {
            decType = DecoderType::BPOSD;
        } else if (tmpStr == "RelayBP") {
            decType = DecoderType::RelayBP;
        } else if (tmpStr == "BPGD") {
            decType = DecoderType::BPGD;
        } else {
            std::cout << "[ERROR] fileIn for initialization not valid (DECODER_TYPE flag not exists).\nExiting..." << std::endl << std::endl;
            exit(0);
        }
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (DECODER_TYPE field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }

    // MonteCarlo parameters
    while (!fileIn.eof()) {
        std::getline(fileIn, tmpStr);
        if (tmpStr == ">> MONTECARLO") {
            break;
        }
    }
    fileIn >> tmpStr;
    if (tmpStr == "nIterationMax:") {
        fileIn >> tmpDouble; 
        m_maxIter = static_cast<int>(tmpDouble);
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (nIterationMax field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "maxErr:") {
        fileIn >> m_maxErrMC;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (maxErr field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "errorRate:") {
        fileIn >> tmpStr; 
        if (tmpStr == "CER") {
            errType = ErrorRateType::CER;
		} else if (tmpStr == "LER") {
            errType = ErrorRateType::LER;
        }
        else {
            std::cout << "[ERROR] fileIn for initialization not valid (errorRate flag not exists).\nExiting..." << std::endl << std::endl;
            exit(0);
        }
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (errorRate field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }

	// Belief propagation parameters
    while (!fileIn.eof()) {
        std::getline(fileIn, tmpStr);
        if (tmpStr == ">> BELIEF PROPAGATION") {
            break;
        }
    }
    fileIn >> tmpStr;
    if (tmpStr == "nIterFirstBelief:") {
        fileIn >> m_maxIterFirstBP;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (nIterFirstBelief field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "nIterOtherBelief:") {
        fileIn >> m_maxIterOtherBP;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (nIterOtherBelief field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "alpha:") {
        fileIn >> m_alphaValue;
        if ((m_alphaValue < 0) && (m_alphaValue != -1))  {
            std::cout << "[ERROR]: alpha must be larger than 0.\nExiting..." << std::endl << std::endl;
            exit(0);
        }
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (alpha field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "eta:") {
        fileIn >> m_eta;
        if (m_eta < 0) {
            std::cout << "[ERROR]: eta must be larger than 0.\nExiting..." << std::endl << std::endl;
            exit(0);
        }
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (eta field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "lambdaOSD:") {
        fileIn >> m_lambdaOSD;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (lambdaOSD field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "orderOSD:") {
        fileIn >> m_orderOSD;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (orderOSD field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "legRelayBP:") {
        fileIn >> m_legRelayBP;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (legRelayBP field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "SRelayBP:") {
        fileIn >> m_SRelayBP;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (SRelayBP field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "GammaWidth:") {
        fileIn >> m_gammaWidth;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (GammaWidth field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "GammaCenter:") {
        fileIn >> m_gammaCenter;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (GammaCenter field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }

    // Channel parameters
    while (!fileIn.eof()) {
        std::getline(fileIn, tmpStr);
        if (tmpStr == ">> CHANNEL") {
            break;
        }
    }
    fileIn >> tmpStr;
    if (tmpStr == "nInputParam:") {
        fileIn >> m_nPhysicalErrRate;
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (nInputParam field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn >> tmpStr;
    if (tmpStr == "chanDepo:") {
        for (int i = 0; i < m_nPhysicalErrRate; ++i) {
            fileIn >> tmpDouble;
            m_initErrProb.push_back(tmpDouble);
        }
        fileIn >> tmpStr;
        if (tmpStr != "EndParams") {
            std::cout << "[ERROR]: fileIn for initialization not valid (EndParams not found). chanDepo has too many or few arguments.\nExiting..." << std::endl << std::endl;
            exit(0);
        }
    }
    else {
        std::cout << "[ERROR]: fileIn for initialization not valid (chanDepo field).\nExiting..." << std::endl << std::endl;
        exit(0);
    }
    fileIn.close();
        
    // Read and Allocation
    readFileBeforeAllocation();
    firstAllocation();
    readFileAfterFirstAllocation();
	secondAllocation();

    pivotsGenX();
	pivotsGenZ();

    std::cout << "[INFO]: Done.\n\n";
}

DecoderSimulation::~DecoderSimulation() {
}

void DecoderSimulation::simulate(void) {

    std::cout << "[INFO]: Simulation Start.\n";
    m_rngEngine.seed(static_cast<unsigned int>(time(0)));
    m_dist01 = std::uniform_real_distribution<double>(0.0, 1.0);
    int countErr;
    unsigned long long int nIter;

    initBP();

    for (int idxP = 0; idxP < m_nPhysicalErrRate; ++idxP) {         //Simulate for all error in input
        m_nErrorSim[idxP] = 0; 
        nIter = 0;
		m_iterError = 0;
        countErr = 0;
        m_iterBP = 0;
		while ((nIter < m_maxIter) && (m_iterError < static_cast<int>(m_maxErrMC))) {     //Main MonteCarlo loop

            m_flagErrorDetected = 0;
            errorGenerationAndSyndrome(idxP);        // simulate only the channel noise
            
            if (decType == DecoderType::BP) {
				errorCorrectionBP(idxP);             // err correction with BP
            } else if (decType == DecoderType::BPOSD) {
                errorCorrectionBPOSD(idxP);          // err correction with BP + OSD
            }
            else if (decType == DecoderType::BPGD) {
                errorCorrectionBPGD(idxP);           // err correction with BPGD
            }
            else if (decType == DecoderType::RelayBP) {
                errorCorrectionRelayBP(idxP);        // err correction with Relay BP
            }
            else if (decType == DecoderType::RestartBelief) {
                errorCorrectionRestartBelief(idxP);  // err correction with Restart Belief
            }
            
            checkSyndrome();                         // check if the correction is in accordance with the syndrome
            if (errType == ErrorRateType::CER) {     // depending on the desired output we compute the error rate
                evaluateCodewordError(idxP);         
            } else if (errType == ErrorRateType::LER) {
                evaluateError(idxP);
            }
            
            if (m_flagErrorDetected == 1) {
				std::cout << nIter << " iterations.\n" << std::endl;   // show iteration montecarlo after an error
                // int wX = 0;
                // int wZ = 0;
                // for (int i = 0; i < m_nCode; i++) {
                //     wX += m_errVectX[i];
                //     wZ += m_errVectZ[i];
                // }
                // if (wX > 0) {
                //     std::cout << "Error weight X: " << wX << std::endl;
                // }
                // if (wZ > 0) {
                //     std::cout << "Error weight Z: " << wZ << std::endl;
                // }
                for (int i = 0; i < m_nCode; i++) {
                    m_errorsX[countErr].push_back(m_errVectX[i]);
                    m_errorsZ[countErr].push_back(m_errVectZ[i]);
                    m_correctionsX[countErr].push_back(m_errVectXCopy[i] ^ m_errVectX[i]);
                    m_correctionsZ[countErr].push_back(m_errVectZCopy[i] ^ m_errVectZ[i]);
                }
                countErr++;
                if (errType == ErrorRateType::CER) {
                    m_errorRateSim[idxP] = m_nErrorSim[idxP] / static_cast<double>(nIter);
                }
                else if (errType == ErrorRateType::LER) {
                    m_errorRateSim[idxP] = m_nErrorSim[idxP] / static_cast<double>(nIter * m_kCode);
                }
                printOutput(); // print output file
            }
            nIter++;
            if (nIter % 100000 == 0) {
				std::cout << "Iter Count: " << nIter << "\n" << std::endl;   // show iteration montecarlo every 1M iterations
                if (PyErr_CheckSignals() != 0) {   // to terminate the execution when launched from python 
                   throw py::error_already_set();  
                }
            }
        }
		
        if (errType == ErrorRateType::CER) {
            m_errorRateSim[idxP] = m_nErrorSim[idxP] / static_cast<double>(nIter);
        }
        else if (errType == ErrorRateType::LER) {
            m_errorRateSim[idxP] = m_nErrorSim[idxP] / static_cast<double>(nIter * m_kCode);
        }
       
        std::cout << "SIM Error rate: " << m_errorRateSim[idxP] << " , with prob error depolarization: " << m_initErrProb[idxP] << std::endl;
        std::cout << std::endl;
        m_totalSimForPhysicalErrorRate[idxP] = m_iterError;
    }

    printOutput(); // print output file

    std::cout << "[INFO]: Simulation End.\n";
}

void DecoderSimulation::simulateAverageBPIterationsTesting(int NumError) {

    std::cout << "[INFO]: Simulation Start.\n";
    m_rngEngine.seed(static_cast<unsigned int>(time(0)));
    m_dist01 = std::uniform_real_distribution<double>(0.0, 1.0);
    unsigned long long int nIter;

    initBP();

    m_nErrorSim[0] = 0; 
    nIter = 0;
    m_iterError = 0;
    m_iterBP = 0;
    m_flagPrintErrors = 1;
    while (nIter < m_maxIter) {     //Main MonteCarlo loop

        m_flagErrorDetected = 0;
        for (int i = 0; i < m_nCode; ++i) {
            m_errVectX[i] = 0;
        }
        std::uniform_real_distribution<double> errDist;
        errDist = std::uniform_real_distribution<double>(0, m_nCode);
        for (int i = 0; i < NumError; ++i) {
            m_posErr[i] = static_cast<int>(errDist(m_rngEngine));
            for (int j = 0; j < i; ++j) {
                if (m_posErr[i] == m_posErr[j]) {
                    --i;
                    break;
                }
            }
        }
        for (int i = 0; i < NumError; i++) {
            m_errVectX[m_posErr[i]] = 1;
            m_errVectXCopy[m_posErr[i]] = 1;
        }

        for (int i = 0; i < m_nGenZ; ++i) {
            m_syndromes[i + m_nGenX] = 0;
            for (int j = 0; j < m_nCode; ++j) {
                m_syndromes[i + m_nGenX] += m_parityCheck[i + m_nGenX][j] * m_errVectX[j];
            }
            m_syndromes[i + m_nGenX] = m_syndromes[i + m_nGenX] % 2;
        	m_initialSyndrome[i + m_nGenX] = m_syndromes[i + m_nGenX];
        }
        
        if (decType == DecoderType::BP) {
            errorCorrectionBP(0);    //errcorrection with BP
        } else if (decType == DecoderType::BPOSD) {
            errorCorrectionBPOSD(0);    //errcorrection with BP + OSD
        }
        else if (decType == DecoderType::BPGD) {
            errorCorrectionBPGD(0);    //errcorrection with BPGD
        }
        else if (decType == DecoderType::RelayBP) {
            errorCorrectionRelayBP(0);    //errcorrection with Relay BP
        }
        else if (decType == DecoderType::RestartBelief) {
            errorCorrectionRestartBelief(0);    //errcorrection with Restart Belief
        }
        
        checkSyndrome();        // check if the correction is in accordance with the syndrome
        if (errType == ErrorRateType::CER) {
            evaluateCodewordError(0);    // evaluate codeword error rate
        } else if (errType == ErrorRateType::LER) {
            evaluateError(0);
        }
        
        if (m_flagErrorDetected == 1) {
            // int wX = 0;
            // int wZ = 0;
            // for (int i = 0; i < m_nCode; i++) {
            //     wX += m_errVectX[i];
            //     wZ += m_errVectZ[i];
            // }
            // if (wX > 0) {
            //     std::cout << "Error weight X: " << wX << std::endl;
            // }
            // if (wZ > 0) {
            //     std::cout << "Error weight Z: " << wZ << std::endl;
            // }
            std::cout << nIter << " iterations.\n" << std::endl;   //show iteration montecarlo after an error
        }
        nIter++;
        if (nIter % 100000 == 0) {
            std::cout << "Iter Count: " << nIter << "\n" << std::endl;   //show iteration montecarlo every 1M iterations
            if (PyErr_CheckSignals() != 0) {
                throw py::error_already_set();  // propagates KeyboardInterrupt to Python
            }
        }
    }
    
    std::cout << "Average BP Iteration number: " << static_cast<double>(m_iterBP) / static_cast<double>(nIter) << std::endl;
    std::cout << std::endl;
    
    std::cout << "[INFO]: Simulation End.\n";    

    m_NAverageBPIterations = static_cast<double>(m_iterBP) / static_cast<double>(nIter);

}

void DecoderSimulation::simulateErrorPatternTesting(std::vector<int>& errVectXPrec, std::vector<int>& errVectZPrec) {

    // m_errVectXCopy = errVectXPrec;
    // m_errVectZCopy = errVectZPrec;

    m_errVectX = errVectXPrec;
    m_errVectZ = errVectZPrec;
    std::fill(m_errVectXCopy.begin(), m_errVectXCopy.end(), 0); // LLR input
    std::fill(m_errVectZCopy.begin(), m_errVectZCopy.end(), 0); // LLR input
    m_rngEngine.seed(static_cast<unsigned int>(time(0)));
    m_dist01 = std::uniform_real_distribution<double>(0.0, 1.0);
    int countErr;

    initBP();
	FindLogicalOperators();

    for (int i = 0; i < m_nGenX; ++i) {
        m_syndromes[i] = 0;
        for (int j = 0; j < m_nCode; ++j) {
            m_syndromes[i] += m_parityCheck[i][j] * m_errVectZ[j];
        }
        m_syndromes[i] = m_syndromes[i] % 2;
        m_initialSyndrome[i] = m_syndromes[i];
    }
    for (int i = 0; i < m_nGenZ; ++i) {
        m_syndromes[i + m_nGenX] = 0;
        for (int j = 0; j < m_nCode; ++j) {
            m_syndromes[i + m_nGenX] += m_parityCheck[i + m_nGenX][j] * m_errVectX[j];
        }
        m_syndromes[i + m_nGenX] = m_syndromes[i + m_nGenX] % 2;
        m_initialSyndrome[i + m_nGenX] = m_syndromes[i + m_nGenX];
    }

    countErr = 0;
    
    if (decType == DecoderType::BP) {
        errorCorrectionBP(0);    //errcorrection with BP
    } else if (decType == DecoderType::BPOSD) {
        errorCorrectionBPOSD(0);    //errcorrection with BP + OSD
    }
    else if (decType == DecoderType::BPGD) {
        errorCorrectionBPGD(0);    //errcorrection with BPGD
    }
    else if (decType == DecoderType::RelayBP) {
        errorCorrectionRelayBP(0);    //errcorrection with Relay BP
    }
    else if (decType == DecoderType::RestartBelief) {
        errorCorrectionRestartBelief(0);    //errcorrection with Restart Belief
    }

    checkSyndrome();        // check if the correction is in accordance with the syndrome
    if (m_nOnesInIdealSyndrome > 0) {      
        countErr++;
    } else if (m_nOnesInIdealSyndrome == 0) {
        // check commutation with logicals
        int nAnticommutation = 0;
        int numErrors = 0;
        for (int j = 0; j < m_kCode; ++j) {
            nAnticommutation = 0;
            for (int i = 0; i < m_nCode; ++i) {
                if ((m_errVectXCopy[i] == 1) && (m_logicalZ[j][i] == 1)) {
                    nAnticommutation++;
                }
            }
            if (nAnticommutation % 2 != 0) { //if anticommuting with logical X -> errorX
                numErrors++;
                nAnticommutation = 0;
            }
            else {
                nAnticommutation = 0;
                for (int i = 0; i < m_nCode; ++i) {
                    if ((m_errVectZCopy[i] == 1) && (m_logicalX[j][i] == 1)) {
                        nAnticommutation++;
                    }
                }
                if (nAnticommutation % 2 != 0) { //if anticommuting with logical Z -> errorZ
                    numErrors++;
                }
            }
        }
        
        if (numErrors != 0) {
            countErr++;
        }
    }

    m_flagCorrected = 0;  // not corrected
    if (countErr == 0) {
        m_flagCorrected = 1; // corrected
    } 

}

void DecoderSimulation::simulateSyndromeBatch(std::vector<std::vector<int>> syndromeMatrix) {

    m_dimensionSyndromeBatch = syndromeMatrix.size();
    m_correctionMatrixX.resize(m_dimensionSyndromeBatch, std::vector<int>(m_nCode, 0));
    m_correctionMatrixZ.resize(m_dimensionSyndromeBatch, std::vector<int>(m_nCode, 0));

    initBP();

    for (int s  = 0; s < m_dimensionSyndromeBatch; s++) {
        for (int i = 0; i < m_nGenX + m_nGenZ; ++i) {
            m_syndromes[i] = syndromeMatrix[s][i];
        }
        
        if (decType == DecoderType::BP) {
            errorCorrectionBP(0);    //errcorrection with BP
        } else if (decType == DecoderType::BPOSD) {
            errorCorrectionBPOSD(0);    //errcorrection with BP + OSD
        }
        else if (decType == DecoderType::BPGD) {
            errorCorrectionBPGD(0);    //errcorrection with BPGD
        }
        else if (decType == DecoderType::RelayBP) {
            errorCorrectionRelayBP(0);    //errcorrection with Relay BP
        }
        else if (decType == DecoderType::RestartBelief) {
            errorCorrectionRestartBelief(0);    //errcorrection with Restart Belief
        }
        for (int i = 0; i < m_nCode; ++i) {
            m_correctionMatrixX[s][i] = m_errVectXCopy[i];
            m_correctionMatrixZ[s][i] = m_errVectZCopy[i];
        }
    }

}

void DecoderSimulation::readFileBeforeAllocation(void) {

    // Open the input file
    std::ifstream inputFile(m_generatorFileName); // Replace with the actual file name
    if (!inputFile.is_open()) {
        std::cerr << "[ERROR]: Unable to open the generator file." << std::endl;
        exit(0);
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line == "N_QUBIT:") {
            if (std::getline(inputFile, line)) {
                m_nCode = std::stoi(line);
            }
        }
        else if (line == "K:") {
            if (std::getline(inputFile, line)) {
                m_kCode = std::stoi(line);
            }
        }
        else if (line == "t:") {
            if (std::getline(inputFile, line)) {
                m_tCode = std::stoi(line);
            }
        }
        else if (line == "N_GEN_X:") {
            if (std::getline(inputFile, line)) {
                m_nGenX = std::stoi(line);
            }
        }
        else if (line == "N_GEN_Z:") {
            if (std::getline(inputFile, line)) {
                m_nGenZ = std::stoi(line);
            }
        }
    }

    // Close the file
    inputFile.close();

    // Output the extracted values for verification
    // std::cout << "N qubit: " << m_nCode << std::endl;

}

void DecoderSimulation::firstAllocation(void) {

    // Vectors for quantum code and simulation
    m_errVectX.resize(m_nCode, 0); // channel error
    m_errVectXCopy.resize(m_nCode, 0); // iterative correction
    m_errVectZ.resize(m_nCode, 0);
    m_errVectZCopy.resize(m_nCode, 0);
    m_syndromes.resize(m_nGenX + m_nGenZ, 0);   // syndrome vector
    m_errorRateSim.resize(m_nPhysicalErrRate, 0);   //Error rate vector for all physical error rates
    m_nErrorSim.resize(m_nPhysicalErrRate, 0);   //Total Error vector for all physical error rates
    m_totalSimForPhysicalErrorRate.resize(m_nPhysicalErrRate, 0); // Total simulations for physical error rate
    m_mappingColumnSwap.resize(m_nCode, -1); // mapping for logical operators calculation

    // vector for Decoders
    m_correctionX.resize(m_nCode, 0);   // correction vector for relay BP
    m_correctionZ.resize(m_nCode, 0);
    m_indicesUpToNcode.resize(m_nCode);
    for (int i = 0; i < m_nCode; ++i) {
        m_indicesUpToNcode[i] = i;
    }

    // vector for BP
    m_errBPGenX.resize(m_nCode, 0);    // error vector find by BP
    m_errBPGenZ.resize(m_nCode, 0);
    m_LLRoutGenX.resize(m_nCode, 0);       // probability vector out of the BP
    m_LLRoutGenZ.resize(m_nCode, 0);
    m_checkSyndBPGenX.resize(m_nCode, 0);  // check syndrome after every iteration of BP
    m_checkSyndBPGenZ.resize(m_nCode, 0);
    m_LLRin.resize(m_nCode, 0);                // LLR vector

    // vector for BPOSD
    m_finalErrorGenX.resize(m_nCode, 0);
    m_finalErrorGenZ.resize(m_nCode, 0);

    // vector for Restart Belief
    m_qubitErroneous.resize(m_tCode, 0);   // Qubit that can be erroneous in the branch search of Restart Belief
    m_initialSyndrome.resize(m_nGenX + m_nGenZ, 0);

    // matrices for quantum code and simulation
    m_HxWithErr.resize(m_nGenX + 1, std::vector<int>(m_nCode, 0));  // Matrix Generators for commutation check with error
    m_HzWithErr.resize(m_nGenZ + 1, std::vector<int>(m_nCode, 0));
    m_logicalX.resize(m_kCode, std::vector<int>(m_nCode, 0));   //matrix of logical operators
    m_logicalZ.resize(m_kCode, std::vector<int>(m_nCode, 0));
    m_parityCheck.resize(m_nGenX + m_nGenZ, std::vector<int>(m_nCode, 0));  // Generators X and Z type
    m_HxTransv.resize(m_nCode, std::vector<int>(m_nGenX, 0));   //Transverse of the matrix of Generators for pivot search
    m_HzTransv.resize(m_nCode, std::vector<int>(m_nGenZ, 0));
    m_errorsX.resize(m_maxErrMC * m_nPhysicalErrRate, std::vector<int>(0)); // matrix of errors X for all montecarlo iterations
    m_errorsZ.resize(m_maxErrMC * m_nPhysicalErrRate, std::vector<int>(0)); // matrix of errors Z for all montecarlo iterations
    m_correctionsX.resize(m_maxErrMC * m_nPhysicalErrRate, std::vector<int>(0));   // correction vector
    m_correctionsZ.resize(m_maxErrMC * m_nPhysicalErrRate, std::vector<int>(0));   // correction vector
    m_posErr.resize(m_nCode, 0); // position vector

    // matrices for BP
    m_messFromQToSyndGenX.resize(m_nCode, std::vector<double>(m_nGenX, -1));  // messages from qubits to syndromes for X generators
    m_messFromSyndToQGenX.resize(m_nGenX, std::vector<double>(m_nCode, -1));  // messages from Syndromes to qubits for X generators
    m_messFromQToSyndGenZ.resize(m_nCode, std::vector<double>(m_nGenZ, -1));  // messages from qubits to syndromes for Z generators
    m_messFromSyndToQGenZ.resize(m_nGenZ, std::vector<double>(m_nCode, -1));  // messages from Syndromes to qubits for Z generators
    m_linksBPQToSyndGenX.resize(m_nCode);   // list of Qubit connected to each syndrome X
    m_linksBPSyndToQGenX.resize(m_nGenX);   // list of Syndrome X connected to each qubit
    m_linksBPQToSyndGenZ.resize(m_nCode);   // list of Qubit connected to each syndrome Z
    m_linksBPSyndToQGenZ.resize(m_nGenZ);   // list of Syndrome Z connected to each qubit
	
}

void DecoderSimulation::readFileAfterFirstAllocation(void) {

    std::string line;
    // Open the input file
    std::ifstream inputFile(m_generatorFileName); // Replace with the actual file name
    if (!inputFile.is_open()) {
        std::cerr << "[ERROR]: Unable to open the generator file." << std::endl;
        exit(0);
    }

    while (std::getline(inputFile, line)) {
        if (line == "PARITY_CHECK:") {
            break;
        }
    }
    for (int i = 0; i < m_nGenX + m_nGenZ; i++) {
        for (int j = 0; j < m_nCode; j++) {
            inputFile >> m_parityCheck[i][j];
        }
    }

    inputFile.close();

    for (int i = 0; i < m_nGenX; i++) {
        for (int j = 0; j < m_nCode; j++) {
            m_HxWithErr[i][j] = m_parityCheck[i][j];
            m_HxTransv[j][i] = m_parityCheck[i][j];
        }
    }for (int i = 0; i < m_nGenZ; i++) {
        for (int j = 0; j < m_nCode; j++) {
            m_HzWithErr[i][j] = m_parityCheck[i + m_nGenX][j];      //Define matrix Hx and Hz for codewords error rate
            m_HzTransv[j][i] = m_parityCheck[i + m_nGenX][j];
        }
    }
    m_rankGenX = rank(m_HxWithErr);     // rank of generators
    m_rankGenZ = rank(m_HzWithErr);
}

void DecoderSimulation::secondAllocation(void) {

    // vector for quantum code and simulation
    m_pivotHx.resize(m_rankGenX, 0);    // pivot of generators
    m_pivotHz.resize(m_rankGenZ, 0);

    //vectors for BPOSD
    m_mapEstGenX.resize(m_nCode, -1);    // mapping vector and for OSD
    m_esGenX.resize(m_rankGenX, 0);
    m_estGenX.resize(m_nCode, 0);
    m_bitmaskGenX.resize(1, true);   // 1 ones
    m_bitmaskGenX.resize(m_nCode - m_rankGenX, false); // n-rankX-1 zeros
    m_mapEstGenZ.resize(m_nCode, -1);
    m_esGenZ.resize(m_rankGenZ, 0);
    m_estGenZ.resize(m_nCode, 0);
    m_bitmaskGenZ.resize(1, true);   // 1 ones
    m_bitmaskGenZ.resize(m_nCode - m_rankGenZ, false); // n-rankZ-1 zeros

    //matrices for quantum code and simulation
    m_HxReduced.resize(m_rankGenX, std::vector<int>(m_nCode, 0));   // reduces generators matrix due to pivots
    m_HzReduced.resize(m_rankGenZ, std::vector<int>(m_nCode, 0));

    // matrices for BPOSD
    m_HsGenX.resize(m_rankGenX);
    m_HsInvGenX.resize(m_rankGenX, std::vector<int>(m_rankGenX, 0));
    m_HtGenX.resize(m_rankGenX, std::vector<int>(m_nCode - m_rankGenX, 0));
    m_HsInvtGenX.resize(m_rankGenX, std::vector<int>(m_nCode - m_rankGenX, 0));
    m_HsGenZ.resize(m_rankGenZ);
    m_HsInvGenZ.resize(m_rankGenZ, std::vector<int>(m_rankGenZ, 0));
    m_HtGenZ.resize(m_rankGenZ, std::vector<int>(m_nCode - m_rankGenZ, 0));
    m_HsInvtGenZ.resize(m_rankGenZ, std::vector<int>(m_nCode - m_rankGenZ, 0));

    // utility functions
    m_G1.resize(m_nCode - m_kCode, std::vector<int>(m_nCode, 0)); // Parity check X matrix
    m_G2.resize(m_nCode - m_kCode, std::vector<int>(m_nCode, 0)); // Parity check Z matrix
    m_I.resize(m_rankGenX, std::vector<int>(m_rankGenX, 0)); // Identity matrix for G1
    m_A.resize(m_rankGenX, std::vector<int>(m_nCode - m_rankGenX, 0)); // A matrix of G1
    m_B.resize(m_rankGenX, std::vector<int>(m_rankGenX, 0)); // B matrix of G2
    m_C.resize(m_rankGenX, std::vector<int>(m_nCode - m_rankGenX, 0)); // C matrix of G2
    m_D.resize(m_nCode - m_kCode - m_rankGenX, std::vector<int>(m_rankGenX, 0)); // D matrix of G2
    m_E.resize(m_nCode - m_kCode - m_rankGenX, std::vector<int>(m_nCode - m_rankGenX, 0)); // E matrix of G2
    m_Lx.resize(m_kCode, std::vector<int>(m_nCode, 0)); // Logical X reduced matrix before rearrangement
    m_Lz.resize(m_kCode, std::vector<int>(m_nCode, 0)); // Logical Z reduced matrix before rearrangement
}

void DecoderSimulation::errorGenerationAndSyndrome(int idxP) {

    for (int i = 0; i < m_nCode; ++i) {
        noise(m_initErrProb[idxP], i);  // add random noise due to depolarizing channel
    }

    for (int i = 0; i < m_nGenX; ++i) {
        m_syndromes[i] = 0;
        for (int j = 0; j < m_nCode; ++j) {
            m_syndromes[i] += m_parityCheck[i][j] * m_errVectZ[j];
        }
        m_syndromes[i] = m_syndromes[i] % 2;
        m_initialSyndrome[i] = m_syndromes[i];
    }

    for (int i = 0; i < m_nGenZ; ++i) {
        m_syndromes[i + m_nGenX] = 0;
        for (int j = 0; j < m_nCode; ++j) {
            m_syndromes[i + m_nGenX] += m_parityCheck[i + m_nGenX][j] * m_errVectX[j];
        }
        m_syndromes[i + m_nGenX] = m_syndromes[i + m_nGenX] % 2;
        m_initialSyndrome[i + m_nGenX] = m_syndromes[i + m_nGenX];
    }

    

    std::fill(m_errVectXCopy.begin(), m_errVectXCopy.end(), 0); // LLR input
    std::fill(m_errVectZCopy.begin(), m_errVectZCopy.end(), 0); // LLR input
}

void DecoderSimulation::noise(double prob_error, int pos) {

    double a = m_dist01(m_rngEngine); // Generate random number between 0 and 1
    if (a < prob_error / 3) {
        m_errVectX[pos] = 1; // X error 
        m_errVectZ[pos] = 0;
    } 
    else if (a < 2 * prob_error / 3) {
        m_errVectX[pos] = 0;
        m_errVectZ[pos] = 1; // Z error
    }
    else if (a < prob_error) {
        m_errVectX[pos] = 1;
        m_errVectZ[pos] = 1; // Y error
    } else {
        m_errVectX[pos] = 0;
        m_errVectZ[pos] = 0;
    }
}

void DecoderSimulation::checkSyndrome(void) {

    int nAnticommutation;
    m_nOnesInIdealSyndrome = 0;
    for (int i = 0; i < m_nGenX; i++) {
        nAnticommutation = 0;
        for (int j = 0; j < m_nCode; ++j) {
            if ((m_errVectZCopy[j] == 1) && (m_parityCheck[i][j] == 1)) {
                nAnticommutation++;
            }
        }
        if (nAnticommutation % 2 != 0) {
            m_nOnesInIdealSyndrome++;
        }
    }

    for (int i = 0; i < m_nGenZ; i++) {
        nAnticommutation = 0;
        for (int j = 0; j < m_nCode; ++j) {
            if ((m_errVectXCopy[j] == 1) && (m_parityCheck[i + m_nGenX][j] == 1)) {
                nAnticommutation++;
            }
        }
        if (nAnticommutation % 2 != 0) {
            m_nOnesInIdealSyndrome++;
        }
    }

}

void DecoderSimulation::evaluateError(int idxP) {

    if (m_nOnesInIdealSyndrome > 0) { // error plus correction does not match the syndrome
        m_nErrorSim[idxP] += m_kCode;
        m_iterError++;
		m_flagErrorDetected = 1;
        if (m_flagPrintErrors == 0) {
            std::cout << "Error correction not found, ";
            std::cout << "Error: " << m_iterError << "/" << m_maxErrMC << std::endl;
            std::cout << "Total logical errors: " << m_nErrorSim[idxP] << std::endl;
        } else {
            std::cout << "Error correction not found, ";
            std::cout << "Error: " << m_iterError << std::endl;
            std::cout << "Total logical errors: " << m_nErrorSim[idxP] << std::endl;
        }
    } else if (m_nOnesInIdealSyndrome == 0) {
        // check commutation with logicals
        int nAnticommutation = 0;
		int numErrors = 0;
        for (int j = 0; j < m_kCode; ++j) {
            nAnticommutation = 0;
            for (int i = 0; i < m_nCode; ++i) {
                if ((m_errVectXCopy[i] == 1) && (m_logicalZ[j][i] == 1)) {
                    nAnticommutation++;
                }
            }
            if (nAnticommutation % 2 != 0) { // if anticommuting with logical Z -> errorX
				numErrors++;
                nAnticommutation = 0;
            }
            else {
                nAnticommutation = 0;
                for (int i = 0; i < m_nCode; ++i) {
                    if ((m_errVectZCopy[i] == 1) && (m_logicalX[j][i] == 1)) {
                        nAnticommutation++;
                    }
                }
                if (nAnticommutation % 2 != 0) { // if anticommuting with logical X -> errorZ
                  //  numErrors++;
                }
            }
		}
        
        if (numErrors != 0) {
            m_iterError ++;
            m_nErrorSim[idxP] += numErrors;
            m_flagErrorDetected = 1;
            if (m_flagPrintErrors == 0) {
                std::cout << "Error: " << m_iterError << "/" << m_maxErrMC << std::endl;
                std::cout << "Total logical errors: " << m_nErrorSim[idxP] << std::endl;
            } else {
                std::cout << "Error: " << m_iterError << std::endl;
                std::cout << "Total logical errors: " << m_nErrorSim[idxP] << std::endl;
            }
        }
    }
}

void DecoderSimulation::evaluateCodewordError(int idxP) {

	if (m_nOnesInIdealSyndrome > 0) {   // if BP didnt find a solution
        m_nErrorSim[idxP] += 1;
        m_iterError++;
        if (m_flagPrintErrors == 0) {
            std::cout << "Error correction not found, ";
            std::cout << "Error: " << m_iterError << "/" << m_maxErrMC << std::endl;
        } else {
            std::cout << "Error correction not found, ";
            std::cout << "Error: " << m_iterError << std::endl;
        }
		m_flagErrorDetected = 1;
    } else if (m_nOnesInIdealSyndrome == 0) { // if BP found a solution
        for (int i = 0; i < m_nCode; i++) {
            m_HxWithErr[m_nGenX][i] = m_errVectXCopy[i];
            m_HzWithErr[m_nGenZ][i] = m_errVectZCopy[i];
        }
        // check commutation with Generators
        int rankX = rank(m_HxWithErr);
        int rankZ = rank(m_HzWithErr);

        if ((rankX == m_rankGenX + 1) || (rankZ == m_rankGenZ + 1)) {
            m_nErrorSim[idxP] += 1;
            m_iterError++;
            if (m_flagPrintErrors == 0) {
                std::cout << "Error: " << m_iterError << "/" << m_maxErrMC << std::endl;
            } else {
                std::cout << "Error: " << m_iterError << std::endl;
            }
			m_flagErrorDetected = 1;
        }
    }
}

void DecoderSimulation::initBP(void) {

    for (int i = 0; i < m_rankGenX; ++i) {
        for (int j = 0; j < m_nCode; j++) {
            m_HxReduced[i][j] = m_parityCheck[m_pivotHx[i]][j];
        }
    }
    for (int i = 0; i < m_rankGenZ; ++i) {
        for (int j = 0; j < m_nCode; j++) {
            m_HzReduced[i][j] = m_parityCheck[m_pivotHz[i] + m_nGenX][j];
        }
    }

    for (int i = 0; i < m_nGenX; ++i) {
        for (int j = 0; j < m_nCode; ++j) {
            if (m_parityCheck[i][j] == 1) {
                m_linksBPSyndToQGenX[i].push_back(j); //Select the qubit linked to every syndrome 
            }
        }
    }
    for (int j = 0; j < m_nCode; ++j) {
        for (int i = 0; i < m_nGenX; ++i) {
            if (m_parityCheck[i][j] == 1) {
                m_linksBPQToSyndGenX[j].push_back(i); // Select the syndrome linked to every qubit 
            }
        }
        if (m_maxQubitToGenX < static_cast<int>(m_linksBPQToSyndGenX[j].size())) {
			m_maxQubitToGenX = static_cast<int>(m_linksBPQToSyndGenX[j].size()); // Find the max number of syndromes linked to a qubit for GenX
		}
    }

    if (errType == ErrorRateType::LER) {
        FindLogicalOperators(); // generate logical operators
    }

    for (int i = 0; i < m_nGenZ; ++i) {
        for (int j = 0; j < m_nCode; ++j) {
            if (m_parityCheck[i + m_nGenX][j] == 1) {
                m_linksBPSyndToQGenZ[i].push_back(j); // Select the qubit linked to every syndrome 
            }
        }
    }
    for (int j = 0; j < m_nCode; ++j) {
        for (int i = 0; i < m_nGenZ; ++i) {
            if (m_parityCheck[i + m_nGenX][j] == 1) {
                m_linksBPQToSyndGenZ[j].push_back(i); // Select the syndrome linked to every qubit 
            }
        }
        if (m_maxQubitToGenZ < static_cast<int>(m_linksBPQToSyndGenZ[j].size())) {
            m_maxQubitToGenZ = static_cast<int>(m_linksBPQToSyndGenZ[j].size()); // Find the max number of syndromes linked to a qubit for GenZ
        }
    }

}

void DecoderSimulation::errorCorrectionBP(int idxP) {
    
    m_naGenX = 0;
    m_naGenZ = 0;
    for (int i = 0; i < m_nGenX; ++i) {
        m_naGenX += m_syndromes[i];
    }   //Evaluate number of syndrome on
    for (int i = 0; i < m_nGenZ; ++i) {
        m_naGenZ += m_syndromes[i + m_nGenX];
    }   //Evaluate number of syndrome on

	std::fill(m_correctionX.begin(), m_correctionX.end(), 0);
	std::fill(m_correctionZ.begin(), m_correctionZ.end(), 0);

    if (m_naGenX != 0) {
		std::fill(m_LLRin.begin(), m_LLRin.end(), log((1.0 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3)));

        BeliefPropagationGenX(m_maxIterFirstBP); // Belief Propagation for Z errors

        if (m_convergenceCheckGenX) {
            m_correctionZ = m_errBPGenX;
        }
   
    }

    if (m_naGenZ != 0) {
		std::fill(m_LLRin.begin(), m_LLRin.end(), log((1.0 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3)));

        BeliefPropagationGenZ(m_maxIterFirstBP); // Belief Propagation for X errors
        
        if (m_convergenceCheckGenZ) {
            m_correctionX = m_errBPGenZ;
        }
    } 

    // Apply corrections
    for (int i = 0; i < m_nCode; ++i) {
        m_errVectXCopy[i] = m_correctionX[i] ^ m_errVectX[i];
        m_errVectZCopy[i] = m_correctionZ[i] ^ m_errVectZ[i];
    }
}

void DecoderSimulation::BeliefPropagationGenX(int maxIterBP) {

    double alpha; // scaling factor
    double minValue; 
    int checkBPGenX;

    for (int i = 0; i < m_nGenX; i++) {
        for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenX[i].size()); j++) {// initialize to 1 messages from syndrome node to qubit node
            m_messFromSyndToQGenX[i][m_linksBPSyndToQGenX[i][j]] = 1;
        }
    }

    for (int j = 0; j < m_nCode; j++) {// initialize to LLR value messages from qubit node to syndrome node
        for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenX[j].size()); i++) {
            m_messFromQToSyndGenX[j][m_linksBPQToSyndGenX[j][i]] = m_LLRin[j];
        }
    }    

    for (int iterBelief = 1; iterBelief < maxIterBP; iterBelief++) {// initliaze messages from qubit node to syndrom node
        if (m_alphaValue == -1) {
            alpha = 1 - static_cast<double>(pow(2, -iterBelief)); // scaling factor
        }
        else {
            alpha = m_alphaValue;
        }
        for (int i = 0; i < m_nGenX; i++) { // for each syndrome node
            for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenX[i].size()); j++) { // for each qubit node
                minValue = std::numeric_limits<double>::max(); // initialize minValue to a large value
                double productSign = 1;
                for (int jNeighbour = 0; jNeighbour < static_cast<int>(m_linksBPSyndToQGenX[i].size()); jNeighbour++) { // for all the neighbours of the syndrome node i exluded j-th qubit node
                    if (m_linksBPSyndToQGenX[i][j] != m_linksBPSyndToQGenX[i][jNeighbour]) {
                        minValue = std::min(minValue, abs(m_messFromQToSyndGenX[m_linksBPSyndToQGenX[i][jNeighbour]][i]));
                        productSign *= ((m_messFromQToSyndGenX[m_linksBPSyndToQGenX[i][jNeighbour]][i] > 0) - (m_messFromQToSyndGenX[m_linksBPSyndToQGenX[i][jNeighbour]][i] < 0));
                    }
                }
                m_messFromSyndToQGenX[i][m_linksBPSyndToQGenX[i][j]] = static_cast<double>(pow(-1, m_syndromes[i])) * alpha * minValue * productSign;
            }
        }
        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenX[j].size()); i++) { // for each syndrome node
                double sumMessagePrecedent = 0;
                for (int iNeighbour = 0; iNeighbour < static_cast<int>(m_linksBPQToSyndGenX[j].size()); iNeighbour++) { // for all the neighbours of the qubit node j exluded i-th syndrome node
                    if (m_linksBPQToSyndGenX[j][iNeighbour] != m_linksBPQToSyndGenX[j][i]) {
                        sumMessagePrecedent += m_messFromSyndToQGenX[m_linksBPQToSyndGenX[j][iNeighbour]][j];
                    }
                }
                m_messFromQToSyndGenX[j][m_linksBPQToSyndGenX[j][i]] = m_LLRin[j] + sumMessagePrecedent; // add the initial message
            }
        }

        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            m_LLRoutGenX[j] = 0;
            for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenX[j].size()); i++) { // for each syndrome node
                m_LLRoutGenX[j] += m_messFromSyndToQGenX[m_linksBPQToSyndGenX[j][i]][j];
            }
            m_LLRoutGenX[j] += m_LLRin[j];
            m_errBPGenX[j] = (m_LLRoutGenX[j] < 0); // calculate the error vector
        }

        checkBPGenX = 0;
        for (int i = 0; i < m_nGenX; i++) { // for each syndrome node
            m_checkSyndBPGenX[i] = 0;
            for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenX[i].size()); j++) { // for each qubit node
                m_checkSyndBPGenX[i] += m_errBPGenX[m_linksBPSyndToQGenX[i][j]];
            }
            if (m_checkSyndBPGenX[i] % 2 == m_syndromes[i]) {
                checkBPGenX++;
            }
        }

        m_iterBP++;

        if (checkBPGenX == m_nGenX) { // if all the syndromes are satisfied
            m_convergenceCheckGenX = true; // set the convergence flag
            break; // exit the loop
        }
        else if (iterBelief == maxIterBP - 1) { // if maximum iterations reached
            m_convergenceCheckGenX = false; // set the convergence flag to false
        }
    }
}

void DecoderSimulation::BeliefPropagationGenZ(int maxIterBP) {

    double alpha;
    double minValue;
    int checkBPGenZ;

    for (int i = 0; i < m_nGenZ; i++) {
        for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); j++) {// initialize to 1 messages from syndrome node to qubit node
            m_messFromSyndToQGenZ[i][m_linksBPSyndToQGenZ[i][j]] = 1;
        }
    }

    for (int j = 0; j < m_nCode; j++) {// initialize to LLR value messages from qubit node to syndrome node
        for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); i++) {
            m_messFromQToSyndGenZ[j][m_linksBPQToSyndGenZ[j][i]] = m_LLRin[j];
        }
    }

    for (int iterBelief = 1; iterBelief < maxIterBP; iterBelief++) { // initialize messages from qubit node to syndrome node

        if (m_alphaValue == -1) {
            alpha = 1 - static_cast<double>(pow(2, -iterBelief)); // scaling factor
        }
        else {
            alpha = m_alphaValue;
        }
        for (int i = 0; i < m_nGenZ; i++) { // for each syndrome node (Z)
            for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); j++) { // for each qubit node
                minValue = std::numeric_limits<double>::max(); // initialize w to a large value
                double tmp_product = 1;
                for (int jNeighbour = 0; jNeighbour < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); jNeighbour++) { // for all the neighbours of the syndrome node i except j-th qubit node
                    if (m_linksBPSyndToQGenZ[i][j] != m_linksBPSyndToQGenZ[i][jNeighbour]) {
                        minValue = std::min(minValue, abs(m_messFromQToSyndGenZ[m_linksBPSyndToQGenZ[i][jNeighbour]][i]));
                        tmp_product *= ((m_messFromQToSyndGenZ[m_linksBPSyndToQGenZ[i][jNeighbour]][i] > 0) - (m_messFromQToSyndGenZ[m_linksBPSyndToQGenZ[i][jNeighbour]][i] < 0));
                    }
                }
                m_messFromSyndToQGenZ[i][m_linksBPSyndToQGenZ[i][j]] = static_cast<double>(pow(-1, m_syndromes[i + m_nGenX])) * alpha * minValue * tmp_product;
            }
        }
        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); i++) { // for each syndrome node
                double tmp_sum = 0;
                for (int iNeighbour = 0; iNeighbour < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); iNeighbour++) { // for all the neighbours of the qubit node j except i-th syndrome node
                    if (m_linksBPQToSyndGenZ[j][iNeighbour] != m_linksBPQToSyndGenZ[j][i]) {
                        tmp_sum += m_messFromSyndToQGenZ[m_linksBPQToSyndGenZ[j][iNeighbour]][j];
                    }
                }
                m_messFromQToSyndGenZ[j][m_linksBPQToSyndGenZ[j][i]] = m_LLRin[j] + tmp_sum; // add the initial message
            }
        }

        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            m_LLRoutGenZ[j] = 0;
            for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); i++) { // for each syndrome node
                m_LLRoutGenZ[j] += m_messFromSyndToQGenZ[m_linksBPQToSyndGenZ[j][i]][j];
            }
            m_LLRoutGenZ[j] += m_LLRin[j];
            m_errBPGenZ[j] = (m_LLRoutGenZ[j] < 0); // calculate the error vector
			
        }

        checkBPGenZ = 0;
        for (int i = 0; i < m_nGenZ; i++) { // for each syndrome node
            m_checkSyndBPGenZ[i] = 0;
            for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); j++) { // for each qubit node
                if (m_parityCheck[i + m_nGenX][m_linksBPSyndToQGenZ[i][j]] == 1) {
                    m_checkSyndBPGenZ[i] += m_errBPGenZ[m_linksBPSyndToQGenZ[i][j]];
                }
            }
            if (m_checkSyndBPGenZ[i] % 2 == m_syndromes[i + m_nGenX]) checkBPGenZ++;
        }

        m_iterBP++;

        if (checkBPGenZ == m_nGenZ) { // if all the syndromes are satisfied
            m_convergenceCheckGenZ = true; // set the convergence flag
            break; // exit the loop
        }
        else if (iterBelief == maxIterBP - 1) { // if maximum iterations reached
            m_convergenceCheckGenZ = false; // set the convergence flag to false
        }
    }
}

void DecoderSimulation::errorCorrectionBPOSD(int idxP) {

    m_naGenX = 0;
    m_naGenZ = 0;
    for (int i = 0; i < m_nGenX; ++i) {
        m_naGenX += m_syndromes[i];
    } 
    for (int i = 0; i < m_nGenZ; ++i) {
        m_naGenZ += m_syndromes[i + m_nGenX];
    } 

	std::fill(m_correctionX.begin(), m_correctionX.end(), 0);
	std::fill(m_correctionZ.begin(), m_correctionZ.end(), 0);

    if (m_naGenX != 0) {
        std::fill(m_LLRin.begin(), m_LLRin.end(), log((1 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input

        BeliefPropagationGenX(m_maxIterFirstBP); // Belief Propagation for Z errors

        if (!m_convergenceCheckGenX) {

            m_bitmaskGenX[0] = true;   // 1 ones
            std::fill(m_bitmaskGenX.begin() + 1, m_bitmaskGenX.end(), false);
            std::fill(m_HsGenX.begin(), m_HsGenX.end(), std::vector<int>(0)); // destroy Hx matrix to be built in OSD
            std::fill(m_esGenX.begin(), m_esGenX.end(), 0); // e_s -> error OSD 0, e_st -> error OSD > 0
            std::fill(m_estGenX.begin(), m_estGenX.end(), 0);
            std::fill(m_finalErrorGenX.begin(), m_finalErrorGenX.end(), 0);

            OSDGenX(); // OSD for Z errors

        }
        else m_correctionZ = m_errBPGenX;
    }

    if (m_naGenZ != 0) {

        std::fill(m_LLRin.begin(), m_LLRin.end(), log((1 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input

        BeliefPropagationGenZ(m_maxIterFirstBP); // Belief Propagation for X errors

        if (!m_convergenceCheckGenZ) {

            m_bitmaskGenZ[0] = true;   // 1 ones
            std::fill(m_bitmaskGenZ.begin() + 1, m_bitmaskGenZ.end(), false);
            std::fill(m_HsGenZ.begin(), m_HsGenZ.end(), std::vector<int>(0));
            std::fill(m_esGenZ.begin(), m_esGenZ.end(), 0);
            std::fill(m_estGenZ.begin(), m_estGenZ.end(), 0);
            std::fill(m_finalErrorGenZ.begin(), m_finalErrorGenZ.end(), 0);

            OSDGenZ(); // OSD for X errors

        }
        else m_correctionX = m_errBPGenZ;
    }


    // Apply corrections
    for (int i = 0; i < m_nCode; ++i) {
        m_errVectXCopy[i] = m_correctionX[i] ^ m_errVectX[i];
        m_errVectZCopy[i] = m_correctionZ[i] ^ m_errVectZ[i];
    }
}

void DecoderSimulation::OSDGenX(void) {

    int rankCheck; // rank matrix Hs
    int idxColumn; 
    int finalHammingWeight;
    int weight;
    int countMapEt;
    
    std::sort(m_indicesUpToNcode.begin(), m_indicesUpToNcode.end(),
		[this](int i1, int i2) { return this->m_LLRoutGenX[i1] < this->m_LLRoutGenX[i2]; }); // Sort indices based on P1 values

	rankCheck = 0;
	idxColumn = 0;
	countMapEt = 0;
    while (rankCheck < m_rankGenX) { // create Hs, the full mapping e_s, the partial mapping e_t 
        for (int i = 0; i < m_rankGenX; ++i) {
            if (static_cast<int>(m_HsGenX[i].size()) == rankCheck) {
                m_HsGenX[i].resize(rankCheck + 1, m_HxReduced[i][m_indicesUpToNcode[idxColumn]]); // adding the column of the reduced maximum rank Hx correspoding to the idxColumn-most probable error
            } else { // the last added column didnt increase the rank -> linearly dependent
                m_HsGenX[i][m_HsGenX[0].size() - 1] = m_HxReduced[i][m_indicesUpToNcode[idxColumn]]; // overwrite the last column
            }
        }
        rankCheck = rank(m_HsGenX); // evaluate rank of the building matrix Hs_x
        if (rankCheck == static_cast<int>(m_HsGenX[0].size())) { // the new column is linearly independent
            m_mapEstGenX[rankCheck - 1] = m_indicesUpToNcode[idxColumn]; // to map back e_s to its original order
        } else {
            m_mapEstGenX[m_rankGenX + countMapEt] = m_indicesUpToNcode[idxColumn]; // to map e_t to its original order
            countMapEt++;
        }
        idxColumn++;
	}  

    for (int i = countMapEt; i < m_nCode - m_rankGenX; ++i) { // complete mapping e_t
        m_mapEstGenX[m_rankGenX + i] = m_indicesUpToNcode[idxColumn];
        idxColumn++;
	}   

    for (int i = 0; i < m_nCode - m_rankGenX; ++i) { // create Ht from mapet
        for (int j = 0; j < m_rankGenX; ++j) {
            m_HtGenX[j][i] = m_HxReduced[j][m_mapEstGenX[i + m_rankGenX]];
        }
	}   
    m_HsInvGenX = inverse(m_HsGenX);    // inverse of Hs

	finalHammingWeight = 0;
    for (int i = 0; i < m_rankGenX; ++i) { // evaluate es and est initial, finalHammingWeight initialize with the weight of es, et = 0
        m_esGenX[i] = 0;
        for (int j = 0; j < m_rankGenX; ++j) {
            m_esGenX[i] += m_HsInvGenX[i][j] * m_syndromes[m_pivotHx[j]]; // !
        }
        m_esGenX[i] = m_esGenX[i] % 2;
        finalHammingWeight += m_esGenX[i];
        m_estGenX[i] = m_esGenX[i];
        m_finalErrorGenX[i] = m_estGenX[i];
	}   

    if (m_orderOSD != 0) {
        for (int i = 0; i < m_rankGenX; ++i) { // create the matrix HsInvt = HsInv * Ht used for OSD_1 all and OSD_2_lamda (referred to t)
            for (int k = 0; k < m_nCode - m_rankGenX; ++k) {
                m_HsInvtGenX[i][k] = 0;
                for (int j = 0; j < m_rankGenX; ++j) {
                    m_HsInvtGenX[i][k] += m_HsInvGenX[i][j] * m_HtGenX[j][k];
                }
            }
        }   
        do {
            weight = 1; // Start with 1 because we are forcing one error in et

            for (int i = 0; i < m_nCode - m_rankGenX; ++i) {
                m_estGenX[i + m_rankGenX] = m_bitmaskGenX[i];
            }

            for (int i = 0; i < m_rankGenX; ++i) {
                m_estGenX[i] = m_esGenX[i]; // Initialize e_st with e_s values
                for (int j = 0; j < m_nCode - m_rankGenX; ++j) {
                    m_estGenX[i] += m_HsInvtGenX[i][j] * m_bitmaskGenX[j];
                }
                m_estGenX[i] = m_estGenX[i] % 2; // Recalculate e_s based by e_t
                weight += m_estGenX[i];
            }
            if (weight < finalHammingWeight) {
                finalHammingWeight = weight;
                m_finalErrorGenX = m_estGenX; // Store the best error vector found
            }
        } while (std::prev_permutation(m_bitmaskGenX.begin(), m_bitmaskGenX.end()));    // iterate over all combinations of one possible error in et

        if (m_orderOSD == 2) {
            m_bitmaskGenX[0] = true;   // 2 ones
            m_bitmaskGenX[1] = true;
            std::fill(m_bitmaskGenX.begin() + 2, m_bitmaskGenX.end(), false);

            do {
                weight = 2;  // Start with 2 because we are forcing two errors in et

                for (int i = 0; i < m_nCode - m_rankGenX; ++i) {
                    m_estGenX[i + m_rankGenX] = m_bitmaskGenX[i];
                }

                for (int i = 0; i < m_rankGenX; ++i) {
                    m_estGenX[i] = m_esGenX[i]; // Initialize e_st with e_s values
                    for (int j = 0; j < m_nCode - m_rankGenX; ++j) {
                        m_estGenX[i] += m_HsInvtGenX[i][j] * m_bitmaskGenX[j];
                    }
                    m_estGenX[i] = m_estGenX[i] % 2; // Recalculate e_s based by e_t
                    weight += m_estGenX[i];
                }
                if (weight < finalHammingWeight) {
                    finalHammingWeight = weight;
                    m_finalErrorGenX = m_estGenX; // Store the best error vector found
                }
            } while (std::prev_permutation(m_bitmaskGenX.begin(), m_bitmaskGenX.begin() + m_lambdaOSD));// iterate over all combinations of two possible errors in et over lampda qubits
        }
    }

    for (int i = 0; i < m_nCode; ++i) {
		m_correctionZ[m_mapEstGenX[i]] = m_finalErrorGenX[i]; // Store the final correction in the correct order, according to mapest
    }
}

void DecoderSimulation::OSDGenZ(void) {
    int rankCheck;
    int idxColumn;
    int finalHammingWeight;
    int weight;
    int countMapEt;

    std::sort(m_indicesUpToNcode.begin(), m_indicesUpToNcode.end(),
		[this](int i1, int i2) { return this->m_LLRoutGenZ[i1] < this->m_LLRoutGenZ[i2]; });    // Sort indices based on P1 values

	rankCheck = 0;
	idxColumn = 0;
	countMapEt = 0;
    // Build Hs, mapes, mapet, mapest
    while (rankCheck < m_rankGenZ) {
        for (int i = 0; i < m_rankGenZ; ++i) {
            if (static_cast<int>(m_HsGenZ[i].size()) == rankCheck) {
                m_HsGenZ[i].resize(rankCheck + 1, m_HzReduced[i][m_indicesUpToNcode[idxColumn]]);
            }
            else {
                m_HsGenZ[i][m_HsGenZ[0].size() - 1] = m_HzReduced[i][m_indicesUpToNcode[idxColumn]];
            }
        }
        rankCheck = rank(m_HsGenZ);
        if (rankCheck + 1 > static_cast<int>(m_HsGenZ[0].size())) {
            m_mapEstGenZ[rankCheck - 1] = m_indicesUpToNcode[idxColumn];
        }
        else {
            m_mapEstGenZ[m_rankGenZ + countMapEt] = m_indicesUpToNcode[idxColumn];
            countMapEt++;
        }
        idxColumn++;
	} //create Hs and mapes, mapet and mapest based from the qubit that most likely have an error

    for (int i = countMapEt; i < m_nCode - m_rankGenZ; ++i) {
        m_mapEstGenZ[m_rankGenZ + i] = m_indicesUpToNcode[idxColumn];
        idxColumn++;
	}   //complete mapet and mapest indices

    for (int i = 0; i < m_nCode - m_rankGenZ; ++i) {
        for (int j = 0; j < m_rankGenZ; ++j) {
            m_HtGenZ[j][i] = m_HzReduced[j][m_mapEstGenZ[i + m_rankGenZ]];
        }
	}   //create Ht from mapet

	m_HsInvGenZ = inverse(m_HsGenZ);    //inverse of Hs

	finalHammingWeight = 0;
    for (int i = 0; i < m_rankGenZ; ++i) {
        m_esGenZ[i] = 0;
        for (int j = 0; j < m_rankGenZ; ++j) {
            m_esGenZ[i] += m_HsInvGenZ[i][j] * m_syndromes[m_pivotHz[j] + m_nGenX];
        }
        m_esGenZ[i] = m_esGenZ[i] % 2;
        finalHammingWeight += m_esGenZ[i];
        m_estGenZ[i] = m_esGenZ[i];
        m_finalErrorGenZ[i] = m_estGenZ[i];
	}   //evaluate es and est initial, finalHammingWeight initialize with the weight of es, et = 0

    if (m_orderOSD != 0) {
        for (int i = 0; i < m_rankGenZ; ++i) {
            for (int k = 0; k < m_nCode - m_rankGenZ; ++k) {
                m_HsInvtGenZ[i][k] = 0;
                for (int j = 0; j < m_rankGenZ; ++j) {
                    m_HsInvtGenZ[i][k] += m_HsInvGenZ[i][j] * m_HtGenZ[j][k];
                }
            }
		}   // create the matrix HsInvt = HsInv * Ht used for OSD1 and OSD2

        do {
            weight = 1;
            
            for (int i = 0; i < m_nCode - m_rankGenZ; ++i) {
                m_estGenZ[i + m_rankGenZ] = m_bitmaskGenZ[i];
            }
            for (int i = 0; i < m_rankGenZ; ++i) {
                m_estGenZ[i] = m_esGenZ[i];
                for (int j = 0; j < m_nCode - m_rankGenZ; ++j) {
                    m_estGenZ[i] += m_HsInvtGenZ[i][j] * m_bitmaskGenZ[j];
                }
                m_estGenZ[i] = m_estGenZ[i] % 2;
                weight += m_estGenZ[i];
            }
            if (weight < finalHammingWeight) {
                finalHammingWeight = weight;
                m_finalErrorGenZ = m_estGenZ;
            }
		} while (std::prev_permutation(m_bitmaskGenZ.begin(), m_bitmaskGenZ.end()));    // iterate over all combinations of one possible error in et

        if (m_orderOSD == 2) {
            m_bitmaskGenZ[0] = true;   // 2 ones
            m_bitmaskGenZ[1] = true;
            std::fill(m_bitmaskGenZ.begin() + 2, m_bitmaskGenZ.end(), false);

            do {
                weight = 2;
                for (int i = 0; i < m_nCode - m_rankGenZ; ++i) {
                    m_estGenZ[i + m_rankGenZ] = m_bitmaskGenZ[i];
                }
                for (int i = 0; i < m_rankGenZ; ++i) {
                    m_estGenZ[i] = m_esGenZ[i];
                    for (int j = 0; j < m_nCode - m_rankGenZ; ++j) {
                        m_estGenZ[i] += m_HsInvtGenZ[i][j] * m_bitmaskGenZ[j];
                    }
                    m_estGenZ[i] = m_estGenZ[i] % 2;
                    weight += m_estGenZ[i];
                }
                if (weight < finalHammingWeight) {
                    finalHammingWeight = weight;
                    m_finalErrorGenZ = m_estGenZ;
                }
            } while (std::prev_permutation(m_bitmaskGenZ.begin(), m_bitmaskGenZ.begin() + m_lambdaOSD));    // iterate over all combinations of two possible errors in et over lampda qubits
        }
    }

    for (int i = 0; i < m_nCode; ++i) {
        m_correctionX[m_mapEstGenZ[i]] = m_finalErrorGenZ[i];
    }
}

void DecoderSimulation::errorCorrectionRelayBP(int idxP) {

	double gamma;
    int countS; // count the number of solutions
	int weight; 
	int finalHammingWeight;

	std::fill(m_correctionX.begin(), m_correctionX.end(), 0);
	std::fill(m_correctionZ.begin(), m_correctionZ.end(), 0);

    m_naGenX = 0;
    m_naGenZ = 0;
    for (int i = 0; i < m_nGenX; ++i) {
        m_naGenX += m_syndromes[i];
    }
    for (int i = 0; i < m_nGenZ; ++i) {
        m_naGenZ += m_syndromes[i + m_nGenX];
    }

    std::uniform_real_distribution<double> gamma_distribution;
    gamma_distribution = std::uniform_real_distribution<double>(m_gammaCenter - m_gammaWidth/2, m_gammaCenter + m_gammaWidth/2);

    if (m_naGenX != 0) {

		std::fill(m_LLRin.begin(), m_LLRin.end(), log((1.0 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3)));
        
		BeliefPropagationGenX(m_maxIterFirstBP);

        countS = 0;
        if (m_convergenceCheckGenX) {
            m_correctionZ = m_errBPGenX;
            countS++;
			finalHammingWeight = 0;
            for (int i = 0; i < m_nCode; i++) {
                finalHammingWeight += m_correctionZ[i];
            }
        }
        else finalHammingWeight = m_nCode;

        for (int iterRelayBP = 0; iterRelayBP < m_legRelayBP; ++iterRelayBP) {
            if (countS == m_SRelayBP) {
                break;
            }
            weight = 0;
            gamma = gamma_distribution(m_rngEngine);

            for (int j = 0; j < m_nCode; j++) {// initialize the probabilities of decoherence channel and initilial messages
                m_LLRin[j] = (1 - gamma) * log((1.0 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3)) + gamma * m_LLRoutGenX[j];
            }

            BeliefPropagationGenX(m_maxIterOtherBP);

            if (m_convergenceCheckGenX) {
                countS++;
                for (int i = 0; i < m_nCode; i++) {
                    weight += m_errBPGenX[i];
                }
                if (weight < finalHammingWeight) {
                    finalHammingWeight = weight;
                    m_correctionZ = m_errBPGenX;
				}
            }
        }

    }

    if (m_naGenZ != 0) {

		std::fill(m_LLRin.begin(), m_LLRin.end(), log((1.0 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3)));

        BeliefPropagationGenZ(m_maxIterFirstBP);

		countS = 0;
        if (m_convergenceCheckGenZ) {
            m_correctionX = m_errBPGenZ;
            countS++;
			finalHammingWeight = 0;
            for (int i = 0; i < m_nCode; i++) {
                finalHammingWeight += m_correctionX[i];
            }
        }
		else finalHammingWeight = m_nCode;

        for (int iterRelayBP = 0; iterRelayBP < m_legRelayBP; ++iterRelayBP) {
            if (countS == m_SRelayBP) {
                break;
            }
            weight = 0;
			gamma = gamma_distribution(m_rngEngine);
            
            for (int j = 0; j < m_nCode; j++) {// initialize the probabilities of decoherence channel and initilial messages
                m_LLRin[j] = (1 - gamma) * log((1.0 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3)) + gamma * m_LLRoutGenZ[j];
            }

            BeliefPropagationGenZ(m_maxIterOtherBP);

            if (m_convergenceCheckGenZ) {
                countS++;
                for (int i = 0; i < m_nCode; i++) {
                    weight += m_errBPGenZ[i];
                }
                if (weight < finalHammingWeight) {
                    finalHammingWeight = weight;
                    m_correctionX = m_errBPGenZ;
                }
            }
        }

    }

    for (int i = 0; i < m_nCode; ++i) {
        m_errVectXCopy[i] = m_correctionX[i] ^ m_errVectX[i];
        m_errVectZCopy[i] = m_correctionZ[i] ^ m_errVectZ[i];
    }
}

void DecoderSimulation::errorCorrectionRestartBelief(int idxP) {
    int IdxFirstBP; // different BPs based on the vertical LLRs
	int finalHammingWeight; // compare current solution with best one (min weight)
    int weight; 
    int CountBP;
	int posMinProb; // position of the qubit with the lowest P1 value

	std::fill(m_correctionX.begin(), m_correctionX.end(), 0); // correction to apply
	std::fill(m_correctionZ.begin(), m_correctionZ.end(), 0);

	m_naGenX = 0;
	m_naGenZ = 0;
    for (int i = 0; i < m_nGenX; ++i) {
        m_naGenX += m_syndromes[i];
    }
    for (int i = 0; i < m_nGenZ; ++i) {
        m_naGenZ += m_syndromes[i + m_nGenX];
    }

    m_flagMoretErrors = false;
    if (m_naGenX != 0) {
        if ((m_naGenX) > m_maxQubitToGenX * m_tCode) {
            m_flagMoretErrors = true;
        }

		std::fill(m_LLRin.begin(), m_LLRin.end(), log((1.0 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input

        BeliefPropagationGenX(m_maxIterFirstBP);

		finalHammingWeight = m_nCode;
        if (m_convergenceCheckGenX) { 
            m_correctionZ = m_errBPGenX;
            finalHammingWeight = 0;
            for (int i = 0; i < m_nCode; i++) {
                if (m_correctionZ[i] == 1) {
                    finalHammingWeight++;
                }
            }
            if (!m_flagMoretErrors) { // if Na <= 2t (surface)
                if (finalHammingWeight > m_tCode) {
                    m_convergenceCheckGenX = 0;
                }
            }
        }
        if (!m_convergenceCheckGenX) { // need to further decode

            std::sort(m_indicesUpToNcode.begin(), m_indicesUpToNcode.end(),
                [this](int i1, int i2) { return this->m_LLRoutGenX[i1] < this->m_LLRoutGenX[i2]; }); // Sort indices based on P1 values

            m_errVectZCopy[m_indicesUpToNcode[0]] ^= 1; // Force an error on the qubit with the lowest P1 value

            for (IdxFirstBP = 0; IdxFirstBP < std::min(m_nCode, m_eta); IdxFirstBP++) {
                std::fill(m_LLRin.begin(), m_LLRin.end(), log((1 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input
                m_LLRin[m_indicesUpToNcode[IdxFirstBP]] = std::numeric_limits<double>::max(); // Set a very high value to the qubit with the lowest P1 value to avoid that it flips again
                for (CountBP = 0; CountBP < m_tCode; CountBP++) {
                    for (int i = 0; i < m_nGenX; i++) {
                        m_syndromes[i] = 0;
                        for (int j = 0; j < m_nCode; ++j) {
                            m_syndromes[i] += m_parityCheck[i][j] * m_errVectZCopy[j];
                        }
                        m_syndromes[i] = (m_syndromes[i] % 2) ^ m_initialSyndrome[i];
                    }

                    BeliefPropagationGenX(m_maxIterOtherBP);

                    if (m_convergenceCheckGenX) {
						weight = 1 + CountBP;
                        for (int i = 0; i < m_nCode; i++) {
                            weight += m_errBPGenX[i];
						}
                        if (weight < finalHammingWeight) {
                            finalHammingWeight = weight;
                            m_correctionZ = m_errBPGenX; // error from converged BP
                            m_correctionZ[m_indicesUpToNcode[IdxFirstBP]] ^= 1; // error from first BP
                            for (int q = 0; q < CountBP; q++) {
                                m_correctionZ[m_qubitErroneous[q]] ^= 1; // errors from the other BPs (towards right)
                            }
                        }
                        break;
                    } else {
						posMinProb = 0;
                        for (int i = 1; i < m_nCode; i++) {
							if (m_LLRoutGenX[i] < m_LLRoutGenX[posMinProb]) { // to find the minimum LLR in the current BP
                                posMinProb = i;
                            }
						}
						m_qubitErroneous[CountBP] = posMinProb;
                        m_errVectZCopy[posMinProb] ^= 1;
                        
                    }
                    m_LLRin[m_qubitErroneous[CountBP]] = std::numeric_limits<double>::max();
                }
                for (int q = 0; q < CountBP; q++) { // clean the forced errors
                    m_errVectZCopy[m_qubitErroneous[q]] ^= 1;
                }
                m_errVectZCopy[m_indicesUpToNcode[IdxFirstBP]] ^= 1; // Return the initial error on the qubit with the P1 value already visualized
                if (m_convergenceCheckGenX) {
                    if (!m_flagMoretErrors) {
                        if (finalHammingWeight <= m_tCode) {
                            break;
                        }
                    } else { // if Na > 2t (surface) -> i keep the first solution
                        break;
                    }
                }
                if (IdxFirstBP + 1 < m_nCode) {
                    m_errVectZCopy[m_indicesUpToNcode[IdxFirstBP + 1]] ^= 1; // Force an error on the qubit with the lowest after the previous one P1 value
                }
            }

        }

    }

    m_flagMoretErrors = false;

    if (m_naGenZ != 0) {
        if ((m_naGenZ) > m_maxQubitToGenZ * m_tCode) {
            m_flagMoretErrors = true;
		}
        std::fill(m_LLRin.begin(), m_LLRin.end(), log((1 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input

        BeliefPropagationGenZ(m_maxIterFirstBP);

        finalHammingWeight = m_nCode;
        if (m_convergenceCheckGenZ) {
            m_correctionX = m_errBPGenZ;
            finalHammingWeight = 0;
            for (int i = 0; i < m_nCode; i++) {
                if (m_correctionX[i] == 1) {
                    finalHammingWeight++;
                }
            }
            if (!m_flagMoretErrors) {
                if (finalHammingWeight > m_tCode) {
                    m_convergenceCheckGenZ = 0;
                }
            }
        }
        if (!m_convergenceCheckGenZ) {

            std::sort(m_indicesUpToNcode.begin(), m_indicesUpToNcode.end(),
                [this](int i1, int i2) { return this->m_LLRoutGenZ[i1] < this->m_LLRoutGenZ[i2]; }); // Sort indices based on P1 values

            m_errVectXCopy[m_indicesUpToNcode[0]] ^= 1; // Force an error on the qubit with the lowest P1 value

            for (IdxFirstBP = 0; IdxFirstBP < std::min(m_nCode, m_eta); IdxFirstBP++) {
                std::fill(m_LLRin.begin(), m_LLRin.end(), log((1 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input
                m_LLRin[m_indicesUpToNcode[IdxFirstBP]] = std::numeric_limits<double>::max(); // Set a very high value to the qubit with the lowest P1 value to avoid that it flips again]
                for (CountBP = 0; CountBP < m_tCode; CountBP++) {

                    for (int i = 0; i < m_nGenZ; i++) {
                        m_syndromes[i + m_nGenX] = 0;
                        for (int j = 0; j < m_nCode; ++j) {
                            m_syndromes[i + m_nGenX] += m_parityCheck[i + m_nGenX][j] * m_errVectXCopy[j];
                        }
                        m_syndromes[i + m_nGenX] = (m_syndromes[i + m_nGenX] % 2) ^ m_initialSyndrome[i + m_nGenX];
                    }
    
                    BeliefPropagationGenZ(m_maxIterOtherBP);

                    if (m_convergenceCheckGenZ) {
						weight = 1 + CountBP;
                        for (int i = 0; i < m_nCode; i++) {
                            weight += m_errBPGenZ[i];
						}
                        if (weight < finalHammingWeight) {
                            finalHammingWeight = weight;
                            m_correctionX = m_errBPGenZ;
                            m_correctionX[m_indicesUpToNcode[IdxFirstBP]] ^= 1;
                            for (int q = 0; q < CountBP; q++) {
                                m_correctionX[m_qubitErroneous[q]] ^= 1;
                            }
                        }
                        break;
                    }
                    else {
                        posMinProb = 0;
                        for (int i = 1; i < m_nCode; i++) {
                            if (m_LLRoutGenZ[i] < m_LLRoutGenZ[posMinProb]) posMinProb = i;
                        }
                        m_qubitErroneous[CountBP] = posMinProb;
                        m_errVectXCopy[posMinProb] ^= 1;
                    }
                    m_LLRin[m_qubitErroneous[CountBP]] = std::numeric_limits<double>::max();
                }

                for (int q = 0; q < CountBP; q++) {
                    m_errVectXCopy[m_qubitErroneous[q]] ^= 1;
                }
                m_errVectXCopy[m_indicesUpToNcode[IdxFirstBP]] ^= 1; // Return the initial error on the qubit with the P1 value already visualized
                if (m_convergenceCheckGenZ) {
                    if (!m_flagMoretErrors) {
                        if (finalHammingWeight <= m_tCode) {
                            break;
                        }
                    }
                    else break;
                }
                if (IdxFirstBP + 1 < m_nCode) {
                    m_errVectXCopy[m_indicesUpToNcode[IdxFirstBP + 1]] ^= 1; // Force an error on the qubit with the lowest after the previous one P1 value
                }
            }

        }

    }

    for (int i = 0; i < m_nCode; ++i) {
        m_errVectXCopy[i] = m_errVectX[i] ^ m_correctionX[i];
        m_errVectZCopy[i] = m_errVectZ[i] ^ m_correctionZ[i];
    }
}

void DecoderSimulation::errorCorrectionBPGD(int idxP) {

	std::fill(m_correctionX.begin(), m_correctionX.end(), 0);
	std::fill(m_correctionZ.begin(), m_correctionZ.end(), 0);

    m_naGenX = 0;
    m_naGenZ = 0;
    for (int i = 0; i < m_nGenX; ++i) {
        m_naGenX += m_syndromes[i];
    }
    for (int i = 0; i < m_nGenZ; ++i) {
        m_naGenZ += m_syndromes[i + m_nGenX];
    }
    if (m_naGenX != 0) {
        std::fill(m_LLRin.begin(), m_LLRin.end(), log((1 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input
        
		BPwithGDGenX();

    }
    if (m_naGenZ != 0) {

        std::fill(m_LLRin.begin(), m_LLRin.end(), log((1 - m_initErrProb[idxP] * 2 / 3) / (m_initErrProb[idxP] * 2 / 3))); // LLR input

        BPwithGDGenZ();

    }
    for (int i = 0; i < m_nCode; ++i) {
        m_errVectXCopy[i] = m_errVectX[i] ^ m_correctionX[i];
        m_errVectZCopy[i] = m_errVectZ[i] ^ m_correctionZ[i];
    }
}

void DecoderSimulation::BPwithGDGenX(void) {

    int checkBPGenX;
    int iterBelief;
    int numberDecimation;
    int newIndices;

    for (int i = 0; i < m_nCode; i++) {
        m_indicesUpToNcode[i] = i;
    }

    for (int i = 0; i < m_nGenX; i++) {
        for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenX[i].size()); j++) {// initialize to 1 messages from syndrome node to qubit node
            m_messFromSyndToQGenX[i][m_linksBPSyndToQGenX[i][j]] = 1;
        }
    }

    for (int j = 0; j < m_nCode; j++) {// initialize the probabilities of decoherence channel and initilial messages
        for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenX[j].size()); i++) {
            m_messFromQToSyndGenX[j][m_linksBPQToSyndGenX[j][i]] = m_LLRin[j];
        }
    }

    numberDecimation = 0;
    checkBPGenX = 0;
    iterBelief = 0;
    newIndices = 0;
    while (numberDecimation < m_nCode - 1) { // initialize messages from qubit node to syndrome node
        for (int i = 0; i < m_nGenX; i++) { // for each syndrome node
            for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenX[i].size()); j++) { // for each qubit node
               double tmp_product = 1;
                for (int jNeighbour = 0; jNeighbour < static_cast<int>(m_linksBPSyndToQGenX[i].size()); jNeighbour++) { // for all the neighbours of the syndrome node i exluded j-th qubit node
                    if (m_linksBPSyndToQGenX[i][j] != m_linksBPSyndToQGenX[i][jNeighbour]) {
                        tmp_product *= std::tanh(m_messFromQToSyndGenX[m_linksBPSyndToQGenX[i][jNeighbour]][i]);
                    }
                }
                m_messFromSyndToQGenX[i][m_linksBPSyndToQGenX[i][j]] = static_cast<double>(pow(-1, m_syndromes[i])) * std::atanh(tmp_product);
            }
        }
        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenX[j].size()); i++) { // for each syndrome node
                double sumMessagePrecedent = 0;
                for (int iNeighbour = 0; iNeighbour < static_cast<int>(m_linksBPQToSyndGenX[j].size()); iNeighbour++) { // for all the neighbours of the qubit node j exluded i-th syndrome node
                    if (m_linksBPQToSyndGenX[j][iNeighbour] != m_linksBPQToSyndGenX[j][i]) {
                        sumMessagePrecedent += m_messFromSyndToQGenX[m_linksBPQToSyndGenX[j][iNeighbour]][j];
                    }
                }
                m_messFromQToSyndGenX[j][m_linksBPQToSyndGenX[j][i]] = m_LLRin[j] + sumMessagePrecedent; // add the initial message
            }
        }

        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            m_LLRoutGenX[j] = 0;
            for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenX[j].size()); i++) { // for each syndrome node
                m_LLRoutGenX[j] += m_messFromSyndToQGenX[m_linksBPQToSyndGenX[j][i]][j];
            }
            m_LLRoutGenX[j] += m_LLRin[j];
            m_correctionZ[j] = (m_LLRoutGenX[j] < 0); // calculate the error vector
        }

        checkBPGenX = 0;
        for (int i = 0; i < m_nGenX; i++) { // for each syndrome node
            m_checkSyndBPGenX[i] = 0;
            for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenX[i].size()); j++) { // for each qubit node
                m_checkSyndBPGenX[i] += m_correctionZ[m_linksBPSyndToQGenX[i][j]];
            }
            if (m_checkSyndBPGenX[i] % 2 == m_syndromes[i]) {
                checkBPGenX++;
            }
        }
        m_iterBP++;
        
        if (checkBPGenX == m_nGenX) { // if all the syndromes are satisfied
            m_convergenceCheckGenX = true; // set the convergence flag
            break; // exit the loop
        }
        iterBelief++;
        if (iterBelief == m_maxIterFirstBP) {
            m_convergenceCheckGenX = false;
            std::sort(m_indicesUpToNcode.begin(), m_indicesUpToNcode.end(),
                [this](int i1, int i2) { return std::abs(this->m_LLRoutGenX[i1]) > std::abs(this->m_LLRoutGenX[i2]); }); // Sort indices based on P1 values
            m_LLRin[m_indicesUpToNcode[0]] = (m_LLRin[m_indicesUpToNcode[0]] > 0) ? 25 : -25;// std::numeric_limits<double>::max() : -std::numeric_limits<double>::max();
            for (int i = m_nCode - 1; i > 0; i--) {
                if (m_indicesUpToNcode[i] != m_indicesUpToNcode[0]) {
                    newIndices = m_indicesUpToNcode[i];
                    break;
                }
            }
            for (int i = m_nCode - 1; i > -1; i--) {
                if (m_indicesUpToNcode[i] == m_indicesUpToNcode[0]) {
                    m_indicesUpToNcode[i] = newIndices;
                }
            }
            m_indicesUpToNcode[0] = m_indicesUpToNcode[1];
            numberDecimation++;
            iterBelief = 0;
        }
    }
}

void DecoderSimulation::BPwithGDGenZ(void) {

    int checkBPGenZ;
	int iterBelief;
    int numberDecimation;
    int newIndices;

    for (int i = 0; i < m_nCode; i++) {
        m_indicesUpToNcode[i] = i;
    }

    for (int i = 0; i < m_nGenZ; i++) {
        for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); j++) {// initialize to 1 messages from syndrome node to qubit node
            m_messFromSyndToQGenZ[i][m_linksBPSyndToQGenZ[i][j]] = 1;
        }
    }

    for (int j = 0; j < m_nCode; j++) {// initialize the probabilities of decoherence channel and initilial messages
        for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); i++) {
            m_messFromQToSyndGenZ[j][m_linksBPQToSyndGenZ[j][i]] = m_LLRin[j];
        }
    }

    checkBPGenZ = 0;
	iterBelief = 0;
    numberDecimation = 0;
    newIndices = 0;
    while (numberDecimation < m_nCode) { // initialize messages from qubit node to syndrome node
        for (int i = 0; i < m_nGenZ; i++) { // for each syndrome node (Z)
            for (int j = 0; j < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); j++) { // for each qubit node
                double tmp_product = 1;
                for (int jNeighbour = 0; jNeighbour < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); jNeighbour++) { // for all the neighbours of the syndrome node i except j-th qubit node
                    if (m_linksBPSyndToQGenZ[i][j] != m_linksBPSyndToQGenZ[i][jNeighbour]) {
                        tmp_product *= std::tanh(m_messFromQToSyndGenZ[m_linksBPSyndToQGenZ[i][jNeighbour]][i]);
                    }
                }
                m_messFromSyndToQGenZ[i][m_linksBPSyndToQGenZ[i][j]] = static_cast<double>(pow(-1, m_syndromes[i + m_nGenX])) * std::atanh(tmp_product);
            }
        }
        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            for (int i = 0; i < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); i++) { // for each syndrome node
                double sumMessagePrecedent = 0;
                for (int iNeighbour = 0; iNeighbour < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); iNeighbour++) { // for all the neighbours of the qubit node j except i-th syndrome node
                    if (m_linksBPQToSyndGenZ[j][iNeighbour] != m_linksBPQToSyndGenZ[j][i]) {
                        sumMessagePrecedent += m_messFromSyndToQGenZ[m_linksBPQToSyndGenZ[j][iNeighbour]][j];
                    }
                }
                m_messFromQToSyndGenZ[j][m_linksBPQToSyndGenZ[j][i]] = m_LLRin[j] + sumMessagePrecedent; // add the initial message
            }
        }

        for (int j = 0; j < m_nCode; j++) { // for each qubit node
            m_LLRoutGenZ[j] = m_messFromSyndToQGenZ[m_linksBPQToSyndGenZ[j][0]][j];
            for (int i = 1; i < static_cast<int>(m_linksBPQToSyndGenZ[j].size()); i++) { // for each syndrome node
                m_LLRoutGenZ[j] += m_messFromSyndToQGenZ[m_linksBPQToSyndGenZ[j][i]][j];
            }
            m_LLRoutGenZ[j] += m_LLRin[j];
            m_correctionX[j] = (m_LLRoutGenZ[j] < 0); // calculate the error vector
        }

        checkBPGenZ = 0;
        for (int i = 0; i < m_nGenZ; i++) { // for each syndrome node
            m_checkSyndBPGenZ[i] = m_correctionX[m_linksBPSyndToQGenZ[i][0]];
            for (int j = 1; j < static_cast<int>(m_linksBPSyndToQGenZ[i].size()); j++) { // for each qubit node
                if (m_parityCheck[i + m_nGenX][m_linksBPSyndToQGenZ[i][j]] == 1) {
                    m_checkSyndBPGenZ[i] += m_correctionX[m_linksBPSyndToQGenZ[i][j]];
                }
            }
            if (m_checkSyndBPGenZ[i] % 2 == m_syndromes[i + m_nGenX]) checkBPGenZ++;
        }
        m_iterBP++;

        if (checkBPGenZ == m_nGenZ) { // if all the syndromes are satisfied
            m_convergenceCheckGenZ = true; // set the convergence flag
            break; // exit the loop
        }
        iterBelief++;
        if (iterBelief == m_maxIterFirstBP) {
	        m_convergenceCheckGenZ = false;
            std::sort(m_indicesUpToNcode.begin(), m_indicesUpToNcode.end(),
                [this](int i1, int i2) { return abs(this-> m_LLRoutGenZ[i1]) > abs(this->m_LLRoutGenZ[i2]); }); // Sort indices based on P1 values
            m_LLRin[m_indicesUpToNcode[0]] = (m_LLRin[m_indicesUpToNcode[0]] > 0) ? 25 : -25; // std::numeric_limits<double>::max() : -std::numeric_limits<double>::max();
            for (int i = m_nCode - 1; i > 0; i--) {
                if (m_indicesUpToNcode[i] != m_indicesUpToNcode[0]) {
                    newIndices = m_indicesUpToNcode[i];
                    break;
                }
            }
            for (int i = m_nCode - 1; i > -1; i--) {
                if (m_indicesUpToNcode[i] == m_indicesUpToNcode[0]) {
                    m_indicesUpToNcode[i] = newIndices;
                }
            }
            m_indicesUpToNcode[0] = m_indicesUpToNcode[1];
            numberDecimation++;
            iterBelief = 0;
        }
    }
}

//// utility functions ////
void DecoderSimulation::printOutput(void) {

    std::ofstream fileTemp;

    fileTemp.open("data/output.txt", std::ios::out);
    if (!fileTemp.is_open()) {
        std::cout << "[WARNING]: The file tmpOutput.m could not be opened. Skipping this partial output saving." << std::endl;
    }
    fileTemp << "% Recap. Simulation v1.0 --------------------------------------------------" << std::endl << std::endl;
    fileTemp << "% Error Propagation Analysis" << std::endl;
    fileTemp << "% N.Montecarlo total:                  " << m_maxIter << std::endl;
    fileTemp << std::endl;
    fileTemp << "% Code Parameter:                 [n, k, t] = [" << m_nCode << ", " << m_kCode << ", " << m_tCode << "]" << std::endl;
    fileTemp << std::endl;
    fileTemp << "% Channel                              " << std::endl;
    fileTemp << "p_Depolarazing = [";
    for (int i = 0; i < m_nPhysicalErrRate - 1; i++) {
        fileTemp << m_initErrProb[i] << ", ";
    }
    fileTemp << m_initErrProb[m_nPhysicalErrRate - 1] << "];" << std::endl;
    fileTemp << std::endl;
    fileTemp << "% Outputs                              " << std::endl;
    fileTemp << "ErrRateSim = [";
    for (int i = 0; i < m_nPhysicalErrRate - 1; i++) {    
        fileTemp << m_errorRateSim[i] << ", ";
    }
    fileTemp << m_errorRateSim[m_nPhysicalErrRate -1] << "];" << std::endl;
	
    // fileTemp << "\ngoodColors = [\"#eb9486\", \"#f3de8a\", \"#7e7f9a\", \"#d387ab\", \"#c1dbaa\", \"#ffdfec\", \"#7ec3ed\"];\n\n";
	// fileTemp << "figure;\n";
    // fileTemp << "loglog(p_Depolarazing, ErrRateSim, 'o', 'Color', goodColors(1), 'lineWidth', 1.5, 'DisplayName','Sim');\n";
    // fileTemp << "ylabel('Logical Error Rate');\n";
    // fileTemp << "xlabel('Physical Error Rate');\n";
    // fileTemp << "legend();\n";
    // fileTemp << "\n";

    fileTemp.close();
}

void DecoderSimulation::FindLogicalOperators(void) {
    int row;
    int p;
    int currentCol;
    int l;
    int temp;
    for (int i = 0; i < m_rankGenX; ++i) {
        for (int j = 0; j < m_nCode; j++) {
            if (m_HxReduced[i][j] == 1) {
                m_G1[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < m_rankGenZ; ++i) {
        for (int j = 0; j < m_nCode; j++) {
            if (m_HzReduced[i][j] == 1) {
                m_G2[i + m_rankGenX][j] = 1;
            }
        }
    }
	for (int i = 0; i < m_nCode; ++i) { // initialize column swap mapping
        m_mappingColumnSwap[i] = i;
    }

    row = 0;
	for (int col = 0; col < m_nCode; ++col) { // Gaussian elimination on G1
        if (row == m_nCode - m_kCode) {
            break;
		}
        p = row;
		while (m_G1[p][col] == 0) { // find the first non-zero entry in the current column from row 'row' upwards
            p++;
            if (p == m_nCode - m_kCode) {
				break;
            }
        }
		if (p == m_nCode - m_kCode) {   //if no non-zero entry found, move to the next column
            continue;
        }
		if (p != row) { // swap rows if there is a non-zero entry from row 'row' upwards
            for (int k = 0; k < m_nCode; k++) {
                temp = m_G1[p][k];
                m_G1[p][k] = m_G1[row][k];
                m_G1[row][k] = temp;
 
                temp = m_G2[p][k];
                m_G2[p][k] = m_G2[row][k];
                m_G2[row][k] = temp;
            }
		}
		for (int r = 0; r < m_nCode - m_kCode; ++r) { //eliminate all other non-zero entries in the current column
            if (r != row && m_G1[r][col] == 1) {
                for (int c = 0; c < m_nCode; ++c) {
                    m_G1[r][c] ^= m_G1[row][c];
                    m_G2[r][c] ^= m_G2[row][c];
                }
            }
        }
		row++;
    }
 
    currentCol = 0;
	for (int i = 0; i < m_nCode - 1; ++i) { // Column swaps to arrange G1 into [[I | A];[0 | 0]] and G2 into [[B | C];[D | E]]
        if (currentCol == m_rankGenX) {
            break;
        }        
        l = i + 1;
        p = 0;
		while (m_G1[p][i] == 0) { //find the first non-zero entry in the current column
            if (p == currentCol + 1) {
                break;
            }
            p++;
        }
		if (p == currentCol) {  // if the non-zero entry is already in the correct position, move to the next column
            currentCol++;
        } else if (p == currentCol + 1) {
            break;
        } else {    //otherwise, search for a column with a non-zero entry in the current row to swap with
            for (l = i + 1; l < m_nCode; l++) {
                p = 0;
				while (m_G1[p][l] == 0) {   //find the first non-zero entry in the l-th column
                    if (p == currentCol + 1) {
                        break;
                    }
                    p++;
                }
				if (p == currentCol) { //if a suitable column is found, perform the swap
                    for (int k = 0; k < m_nCode - m_kCode; k++) {
                        temp = m_G1[k][i];
                        m_G1[k][i] = m_G1[k][l];
                        m_G1[k][l] = temp;
 
                        temp = m_G2[k][i];
                        m_G2[k][i] = m_G2[k][l];
                        m_G2[k][l] = temp;
                    }
                    temp = m_mappingColumnSwap[i];
                    m_mappingColumnSwap[i] = m_mappingColumnSwap[l];
                    m_mappingColumnSwap[l] = temp; //update the column swap mapping
                    currentCol++;
					break;
                }
                if (l == m_nCode - 1) {
                    break;
                }
            }
        }
        if (l == m_nCode - 1) {
            break;
        }
    }
 
	for (int i = 0; i < m_rankGenX; i++) { // Separate G1 and G2 into submatrices A, B, C, D, E, I
        for (int j = 0; j < m_rankGenX; j++) {
            m_I[i][j] = m_G1[i][j];
            m_B[i][j] = m_G2[i][j];
        }
        for (int j = m_rankGenX; j < m_nCode; j++) {
            m_A[i][j - m_rankGenX] = m_G1[i][j];
            m_C[i][j - m_rankGenX] = m_G2[i][j];
        }
    }
    for (int i = m_rankGenX; i < m_nCode - m_kCode; ++i) {
        for (int j = 0; j < m_rankGenX; j++) {
            m_D[i - m_rankGenX][j] = m_G2[i][j];
        }
        for (int j = m_rankGenX; j < m_nCode; j++) {
            m_E[i - m_rankGenX][j - m_rankGenX] = m_G2[i][j];
        }
    }
 
    row = 0;
	for (int col = 0; col < m_nCode - m_rankGenX; ++col) { // Gaussian elimination on E 
        if (row == m_nCode - m_kCode - m_rankGenX) {
            break;
        }
        p = row;
		while (m_E[p][col] == 0) {  //find the first non-zero entry in the current column from row 'row' upwards
            p++;
            if (p == m_nCode - m_kCode - m_rankGenX) {
                break;
            }
        }
        if (p == m_nCode - m_kCode - m_rankGenX) {
            continue;
        }
		if (p != row) { //swap rows if there is a non-zero entry from row 'row' upwards
            for (int k = 0; k < m_rankGenX; k++) {
                temp = m_E[p][k];
                m_E[p][k] = m_E[row][k];
                m_E[row][k] = temp;
 
                temp = m_D[p][k];
                m_D[p][k] = m_D[row][k];
                m_D[row][k] = temp;
            }
            for (int k = m_rankGenX; k < m_nCode - m_rankGenX; k++) {
                temp = m_E[p][k];
                m_E[p][k] = m_E[row][k];
                m_E[row][k] = temp;
			}
        }
 
		for (int r = 0; r < m_nCode - m_kCode - m_rankGenX; ++r) {  //eliminate all other non-zero entries in the current column
            if (r != row && m_E[r][col] == 1) {
                for (int c = 0; c < m_rankGenX; ++c) {
                    m_E[r][c] ^= m_E[row][c];
                    m_D[r][c] ^= m_D[row][c];
                }
                for (int c = m_rankGenX; c < m_nCode - m_rankGenX; c++) {
                    m_E[r][c] ^= m_E[row][c];
                }
            }
        }
        row++;
    }
    currentCol = 0;
    int i;
	for (i = 0; i < m_nCode - m_rankGenX - 1; ++i) {    // Column swaps to arrange E into [I | E2]
        if (currentCol == m_rankGenX) {
            break;
        }
        l = i + 1;
        p = 0;
		while (m_E[p][i] == 0) {    //find the first non-zero entry in the current column
            if (p == currentCol + 1) {
                break;
            }
            p++;
        }
		if (p == currentCol) {  // if the non-zero entry is already in the correct position, move to the next column
            currentCol++;
        } else if (p == currentCol + 1) {
            break;
        } else {  //otherwise, search for a column with a non-zero entry in the current row to swap with
            for (l = i + 1; l < m_nCode - m_rankGenX; l++) {
                p = 0;
				while (m_E[p][l] == 0) {    //find the first non-zero entry in the l-th column
                    if (p == currentCol + 1) {
                        break;
                    }
                    p++;
                }
				if (p == currentCol) {  //if a suitable column is found, perform the swap
                    for (int k = 0; k < m_nCode - m_kCode - m_rankGenX; k++) {
                        temp = m_E[k][i];
                        m_E[k][i] = m_E[k][l];
                        m_E[k][l] = temp;
 
                        temp = m_A[k][i];
                        m_A[k][i] = m_A[k][l];
                        m_A[k][l] = temp;
 
                        temp = m_C[k][i];
                        m_C[k][i] = m_C[k][l];
                        m_C[k][l] = temp;
                    }
                    temp = m_mappingColumnSwap[i + m_rankGenX];
                    m_mappingColumnSwap[i + m_rankGenX] = m_mappingColumnSwap[l + m_rankGenX];
                    m_mappingColumnSwap[l + m_rankGenX] = temp;  //update the column swap mapping
                    currentCol++;
                    break;
                }
                if (l == m_nCode - 1) {
                    break;
                }
            }
        }
        if (l == m_nCode - 1) {
            break;
        }
    }
 
    for (int i = 0; i < m_rankGenX; ++i) { // Construct Lx matrix
        for (int j = 0; j < m_kCode; ++j) {
            m_Lz[j][i] = m_A[i][m_nCode - m_rankGenX - m_kCode + j];
        }
    }
    for (int i = m_rankGenX; i < m_nCode - m_kCode; ++i) { 
        for (int j = 0; j < m_kCode; ++j) {
            m_Lx[j][i] = m_E[i - m_rankGenX][m_nCode - m_rankGenX - m_kCode + j];
		}
	}
    for (int j = 0; j < m_kCode; ++j) {
        m_Lx[j][m_nCode - m_kCode + j] = 1;
        m_Lz[j][m_nCode - m_kCode + j] = 1;
	}
 
    for (int i = 0; i < m_nCode; ++i) { // Rearrange Lx and Lz according to the column swap mapping
        for (int j = 0; j < m_kCode; ++j) {
            m_logicalX[j][m_mappingColumnSwap[i]] = m_Lx[j][i];
            m_logicalZ[j][m_mappingColumnSwap[i]] = m_Lz[j][i];
        }
	}
}

int DecoderSimulation::rank(const std::vector<std::vector<int>>& matrix) {
    // Make a copy to work on
    int m = static_cast<int>(matrix.size());
    int k = static_cast<int>(matrix[0].size());
    std::vector<std::vector<int>> mat = matrix;

    int rank = 0;
    for (int col = 0; col < k && rank < m; ++col) {
        // Find pivot row
        int pivot = -1;
        for (int row = rank; row < m; ++row) {
            if (mat[row][col] == 1) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue;

        // Swap rows
        std::swap(mat[rank], mat[pivot]);

        // Eliminate other rows
        for (int row = 0; row < m; ++row) {
            if (row != rank && mat[row][col] == 1) {
                for (int j = 0; j < k; ++j) {
                    mat[row][j] ^= mat[rank][j]; // XOR = addition mod 2
                }
            }
        }

        rank++;
    }
    return rank;
}

std::vector<std::vector<int>> DecoderSimulation::inverse(const std::vector<std::vector<int>>& mat) {
    int n = static_cast<int>(mat.size());
    if (n == 0) return {};
    int m = static_cast<int>(mat[0].size());
    if (n != m) throw std::runtime_error("Matrix must be square for inversion.");

    std::vector<std::vector<int>> A = mat;
    std::vector<std::vector<int>> inv(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) inv[i][i] = 1;

    for (int col = 0; col < n; ++col) {
        // Find pivot
        int pivot = -1;
        for (int row = col; row < n; ++row) {
            if (A[row][col] == 1) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) throw std::runtime_error("Matrix is singular and cannot be inverted.");

        // Swap rows in both A and inv
        if (pivot != col) {
            std::swap(A[col], A[pivot]);
            std::swap(inv[col], inv[pivot]);
        }

        // Eliminate other rows
        for (int row = 0; row < n; ++row) {
            if (row != col && A[row][col] == 1) {
                for (int k = 0; k < n; ++k) {
                    A[row][k] ^= A[col][k];
                    inv[row][k] ^= inv[col][k];
                }
            }
        }
    }
    return inv;
}

long long int DecoderSimulation::BinomialCoefficient(int n, int k) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    // Calculate the binomial coefficient using a multiplicative formula
    long long int result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

void DecoderSimulation::pivotsGenX(void) {

	int m = static_cast<int>(m_HxTransv.size());
    int n = static_cast<int>(m_HxTransv[0].size());
    int row = 0;
	int idxPivot = 0;

    for (int col = 0; col < n && row < m; ++col) {
        int pivot_row = -1;
        for (int r = row; r < m; ++r) {
            if (m_HxTransv[r][col] % 2 != 0) { // nonzero mod 2
                pivot_row = r;
                break;
            }
        }
        if (pivot_row == -1) continue; // no pivot in this column

        // Swap pivot row up
        if (pivot_row != row) std::swap(m_HxTransv[pivot_row], m_HxTransv[row]);
        // Eliminate other 1's in this column (GF(2): XOR)
        for (int r = 0; r < m; ++r) {
            if (r != row && (m_HxTransv[r][col] % 2 != 0)) {
                for (int c = 0; c < n; ++c)
                    m_HxTransv[r][c] = (m_HxTransv[r][c] + m_HxTransv[row][c]) % 2; // XOR in GF(2)
            }
        }
        m_pivotHx[idxPivot] = col;
		idxPivot++;
        ++row;
    }
}

void DecoderSimulation::pivotsGenZ(void) {
    int m = static_cast<int>(m_HzTransv.size());
    int n = static_cast<int>(m_HzTransv[0].size());
    int row = 0;
	int idxPivot = 0;

    for (int col = 0; col < n && row < m; ++col) {
        int pivot_row = -1;
        for (int r = row; r < m; ++r) {
            if (m_HzTransv[r][col] % 2 != 0) { // nonzero mod 2
                pivot_row = r;
                break;
            }
        }
        if (pivot_row == -1) continue; // no pivot in this column

        // Swap pivot row up
        if (pivot_row != row) std::swap(m_HzTransv[pivot_row], m_HzTransv[row]);

        // Eliminate other 1's in this column (GF(2): XOR)
        for (int r = 0; r < m; ++r) {
            if (r != row && (m_HzTransv[r][col] % 2 != 0)) {
                for (int c = 0; c < n; ++c)
                    m_HzTransv[r][c] = (m_HzTransv[r][c] + m_HzTransv[row][c]) % 2; // XOR in GF(2)
            }
        }

        m_pivotHz[idxPivot] = col;
        idxPivot++;
        ++row;
    }
}
