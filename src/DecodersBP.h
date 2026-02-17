#pragma once
#include <vector>
#include <cassert>
#include <limits>
#include <fstream>
#include <cmath>  
#include <math.h>
#include <cstdlib> 
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <random>
#include <unordered_map>
#include <algorithm>  
#include <random>  
#include <iomanip> 
#include <chrono>
#include <bitset>    
#include <cstdint>
#include <stdexcept> 
#include <numeric>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

enum class DecoderType {
    BP,
    BPOSD,
    BPGD,
	RelayBP,
    RestartBelief,
};

enum class ErrorRateType {
    LER,
    CER,
};

class DecoderSimulation {
public:
    DecoderSimulation(const char* fileName);
    ~DecoderSimulation();

    void simulate(void);
	void simulateErrorPatternTesting(std::vector<int>& errVectX_prec, std::vector<int>& errVectZ_prec);
	void simulateAverageBPIterationsTesting(int NumError);
	void simulateSyndromeBatch(std::vector<std::vector<int>> syndromeMatrix);

private:
    // initialization functions
	void readFileBeforeAllocation(void);
    void firstAllocation(void);
    void readFileAfterFirstAllocation(void);
	void secondAllocation(void);

	// channel error functions
	void errorGenerationAndSyndrome(int idxP);
    void noise(double prob_error, int pos);
	void checkSyndrome(void);
	void evaluateError(int idxP);
	void evaluateCodewordError(int idxP);
	
	// initialization of BP
	void initBP(void);

	// BP
    void errorCorrectionBP(int idxP);
	void BeliefPropagationGenX(int maxIterBP);
	void BeliefPropagationGenZ(int maxIterBP);

	// BPOSD
	void errorCorrectionBPOSD(int idxP);
	void OSDGenX(void);
	void OSDGenZ(void);

	// Relay BP
	void errorCorrectionRelayBP(int idxP);

	// Restart Belief
	void errorCorrectionRestartBelief(int idxP);

	// BPGD
	void errorCorrectionBPGD(int idxP);
	void BPwithGDGenX(void);
	void BPwithGDGenZ(void);

    // utility functions
	void printOutput(void);
	int rank(const std::vector<std::vector<int>>& matrix);
    std::vector<std::vector<int>> inverse(const std::vector<std::vector<int>>& mat);
	long long int BinomialCoefficient(int n, int k);
    void pivotsGenX(void);
    void pivotsGenZ(void);
	void FindLogicalOperators(void);

private:
	DecoderType decType{ DecoderType::RestartBelief };
	ErrorRateType errType{ ErrorRateType::CER };

	// variables for quantum code and simulation
	unsigned long long int m_maxIter{ 0 };
	int m_maxErrMC{ 0 };
	int m_nPhysicalErrRate{ 0 };
	int m_maxQubitToGenX{ 0 };
	int m_maxQubitToGenZ{ 0 };
	int m_naGenX{ 0 };
	int m_naGenZ{ 0 };
	int m_flagErrorDetected{ 0 };
	int m_tCode{ 0 };
	int m_nCode{ 0 };
	int m_kCode{ 0 };
	int m_nGenX{ 0 };
	int m_nGenZ{ 0 };
	int m_nOnesInIdealSyndrome{ 0 };
	int m_iterError{ 0 };
	int m_flagPrintErrors{ 0 };
	int m_dimensionSyndromeBatch{ 0 };

	// variables for decoders
	int m_legRelayBP{ 0 };
	int m_SRelayBP{ 0 };
	int m_rankGenX { 0 };
	int m_rankGenZ{ 0 };
	int m_lambdaOSD{ 0 }; 
	int m_orderOSD{ 0 };
	int m_maxIterFirstBP{ 0 };
	int m_maxIterOtherBP{ 0 };
	int m_eta{ 0 };
	int m_iterBP{ 0 };
	bool m_flagMoretErrors{ 0 };
	bool m_convergenceCheckGenX{ 0 };
	bool m_convergenceCheckGenZ{ 0 };
	double m_gammaCenter{ 0 };
	double m_gammaWidth{ 0 };
	double m_alphaValue{ 0 };

	// strings for simulation
	std::string m_outputFileName{ "" };
	std::string m_generatorFileName{ "" };

	// vectors for quantum code and simulation
	std::vector<int> m_syndromes;
	std::vector<int> m_errVectX;
	std::vector<int> m_errVectZ;
	std::vector<int> m_errVectXCopy;
	std::vector<int> m_errVectZCopy;
	std::vector<int> m_pivotHx;
	std::vector<int> m_pivotHz;
	std::vector<int> m_posErr;
	std::vector<double> m_initErrProb;
	std::vector<double> m_nErrorSim;
	std::vector<int> m_mappingColumnSwap;

	// vectors for decoders
	std::vector<int> m_indicesUpToNcode;

	// vectors for BP
	std::vector<int> m_errBPGenX;
	std::vector<int> m_errBPGenZ;
	std::vector<int> m_checkSyndBPGenX;
	std::vector<int> m_checkSyndBPGenZ;
	std::vector<double> m_LLRoutGenX;
	std::vector<double> m_LLRoutGenZ;
	std::vector<double> m_LLRin;

	// vectors for BPOSD
	std::vector<int> m_finalErrorGenX;
	std::vector<int> m_finalErrorGenZ;
	std::vector<int> m_mapEstGenX;
	std::vector<int> m_mapEstGenZ;
	std::vector<int> m_esGenX;
	std::vector<int> m_esGenZ;
	std::vector<int> m_estGenX;
	std::vector<int> m_estGenZ;
	std::vector<bool> m_bitmaskGenX;
	std::vector<bool> m_bitmaskGenZ;

	// vectors for Restart Belief
	std::vector<int> m_qubitErroneous;
	std::vector<int> m_initialSyndrome;

	// random number generator
	std::mt19937 m_rngEngine;
	std::uniform_real_distribution<double> m_dist01;

	// matrices for quantum code and simulation
	std::vector<std::vector<int>> m_parityCheck;
	std::vector<std::vector<int>> m_HxTransv;
	std::vector<std::vector<int>> m_HzTransv;
	std::vector<std::vector<int>> m_logicalX;
	std::vector<std::vector<int>> m_logicalZ;
	std::vector<std::vector<int>> m_HxReduced;
	std::vector<std::vector<int>> m_HzReduced;

	// matrices for codeword error rate
	std::vector<std::vector<int>> m_HxWithErr;
	std::vector<std::vector<int>> m_HzWithErr;

	//matrices for BP
	std::vector<std::vector<int>> m_linksBPQToSyndGenX;
	std::vector<std::vector<int>> m_linksBPSyndToQGenX;
	std::vector<std::vector<int>> m_linksBPQToSyndGenZ;
	std::vector<std::vector<int>> m_linksBPSyndToQGenZ;
	std::vector<std::vector<double>> m_messFromQToSyndGenX;
	std::vector<std::vector<double>> m_messFromSyndToQGenX;
	std::vector<std::vector<double>> m_messFromQToSyndGenZ;
	std::vector<std::vector<double>> m_messFromSyndToQGenZ;

	// matrices for BPOSD
	std::vector<std::vector<int>> m_HsInvGenX;
	std::vector<std::vector<int>> m_HsInvGenZ;
	std::vector<std::vector<int>> m_HtGenX;
	std::vector<std::vector<int>> m_HtGenZ;
	std::vector<std::vector<int>> m_HsInvtGenX;
	std::vector<std::vector<int>> m_HsInvtGenZ;
	std::vector<std::vector<int>> m_HsGenX;
	std::vector<std::vector<int>> m_HsGenZ;

	// utility matrices
	std::vector<std::vector<int>> m_G1;
	std::vector<std::vector<int>> m_G2;
	std::vector<std::vector<int>> m_I;
	std::vector<std::vector<int>> m_A;
	std::vector<std::vector<int>> m_B;
	std::vector<std::vector<int>> m_C;
	std::vector<std::vector<int>> m_D;
	std::vector<std::vector<int>> m_E;
	std::vector<std::vector<int>> m_Lx;
	std::vector<std::vector<int>> m_Lz;
 
 

public:
	double m_NAverageBPIterations{ 0 };
	int m_flagCorrected { 0 };

	std::vector<std::vector<int>> m_errorsX;
	std::vector<std::vector<int>> m_errorsZ;
	std::vector<std::vector<int>> m_correctionsX;
	std::vector<std::vector<int>> m_correctionsZ;
	std::vector<int> m_correctionX;
	std::vector<int> m_correctionZ;
	std::vector<double> m_errorRateSim;
	std::vector<int> m_totalSimForPhysicalErrorRate;
	std::vector<std::vector<int>> m_correctionMatrixX;
	std::vector<std::vector<int>> m_correctionMatrixZ;

};