cmake_minimum_required(VERSION 3.16)
project(MuonTomographySim)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

add_executable(muonSim main.cc
    src/DetectorConstruction.cc
    src/PrimaryGeneratorAction.cc
    src/ScintillatorSD.cc
    src/RunAction.cc
	src/ActionInitialization.cc
)

target_include_directories(muonSim PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(muonSim ${Geant4_LIBRARIES})
