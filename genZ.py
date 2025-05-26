import numpy as np

# Función generadora de energías
def energies(num_muons):
    U = np.random.uniform(size=num_muons)
    return(np.power(((U*8.503863413817225325e-02) -0.00029)/(-0.0848)+1**(-1.7),-1/1.7))

# Función generadora de posición
def pos(num_muons):
    U = np.random.uniform(low = -7.5, high = 7.5, size=num_muons)
    return(U)


num_muons = 10000 # Número de muones
E_min = 1 # GeV #Energía mínima a generar
E_max = 1000 # GeV # Energía máxima a generar
alpha = 2.7

list_energies = np.zeros((num_muons,3))

list_energies[:,0] = energies(num_muons)
list_energies[:,1] = pos(num_muons)
list_energies[:,2] = pos(num_muons)

np.savetxt("energies",list_energies) # Documento con Energías, coordenada en x , coordenada en y

