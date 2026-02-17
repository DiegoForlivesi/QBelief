#include "DecodersBP.h"

int main() {
    DecoderSimulation sim("data/fileIn.txt");
    sim.simulate();
    return 0;
}

//////////////////////////////////////////////////////////////
// FUNCTIONALITIES FOR LAUNCHING CPP IN PYTHON USING PYBIND //
//////////////////////////////////////////////////////////////

std::vector<double> MonteCarloSimulation() {
    DecoderSimulation sim("data/fileIn.txt");
    sim.simulate();
    return sim.m_errorRateSim;
}

std::tuple<std::vector<std::vector<int>>,
           std::vector<std::vector<int>>,
           std::vector<std::vector<int>>,
           std::vector<std::vector<int>>,
           std::vector<double>,
           std::vector<int>> MonteCarloSimulationDebug() {
    DecoderSimulation sim("data/fileIn.txt");
    sim.simulate();
    return std::make_tuple(
        std::move(sim.m_errorsX),
        std::move(sim.m_correctionsX),
        std::move(sim.m_errorsZ),
        std::move(sim.m_correctionsZ),
        std::move(sim.m_errorRateSim),
        std::move(sim.m_totalSimForPhysicalErrorRate)
    );
}

double AverageBPIterationsTesting(int NumError) {
    DecoderSimulation sim("data/fileIn.txt");
    sim.simulateAverageBPIterationsTesting(NumError);
    return sim.m_NAverageBPIterations;
}

std::tuple<std::vector<int>,
           std::vector<int>,
           bool> ErrorPatternTesting(std::vector<int> errorsX, std::vector<int> errorsZ) {
    DecoderSimulation sim("data/fileIn.txt");
    sim.simulateErrorPatternTesting(errorsX, errorsZ);
    return std::make_tuple(
        std::move(sim.m_correctionX),
        std::move(sim.m_correctionZ),
        sim.m_flagCorrected
    );
}

std::tuple<std::vector<std::vector<int>>,
           std::vector<std::vector<int>>> SyndromeBatchSimulation(std::vector<std::vector<int>> syndromeMatrix) {
    DecoderSimulation sim("data/fileIn.txt");
    sim.simulateSyndromeBatch(syndromeMatrix);
    return std::make_tuple(
        std::move(sim.m_correctionMatrixX),
        std::move(sim.m_correctionMatrixZ)
    );
}

PYBIND11_MODULE(Simulation, m) {
    m.def("MonteCarloSimulation", &MonteCarloSimulation);
    m.def("MonteCarloSimulationDebug", &MonteCarloSimulationDebug);
    m.def("ErrorPatternTesting", &ErrorPatternTesting);
    m.def("AverageBPIterationsTesting", &AverageBPIterationsTesting);
    m.def("SyndromeBatchSimulation", &SyndromeBatchSimulation);
}