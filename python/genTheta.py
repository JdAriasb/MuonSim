import numpy as np

# Constantes
E_min = 1.0   # GeV     Energía mínima a generar
E_max = 1000.0  # GeV   Energía máxima a generar
theta_max_deg = 85 # deg Ángulo theta máximo a generar
gamtheta = 2  # Para una distribución cos^n(theta)


# Función de flujo de Gaisser
def gaisser_flux(E, theta):
    theta_rad = np.deg2rad(theta)
    cos_theta = np.cos(theta_rad)
    return 0.14 * (E ** -2.7) * (
        1 / (1 + 1.1 * E * cos_theta / 115) +
        0.054 / (1 + 1.1 * E * cos_theta / 850)
    )

# Generación de ángulos theta por medio de rejection sampling
def sample_theta(gam=gamtheta, theta_max=theta_max_deg):
    theta_max_rad = np.deg2rad(theta_max)
    while True:
        theta = np.random.uniform(0, theta_max_rad)
        y = np.random.uniform(0, 1)
        if y < np.cos(theta)**gam:
            return theta

# Generación Energía por medio de rejection sampling
# def sample_energy(theta, e_min=E_min, e_max=E_max):
#     while True:
#         E = np.random.uniform(e_min, e_max)
#         y = np.random.uniform(0, 0.14 * (e_min ** -2.7))  # Upper bound of flux
#         if y < gaisser_flux(E, theta):
#             return E
            
# Generación Energía por medio de su función cumulativa
def sample_energy(theta, e_min=E_min, e_max=E_max):
    U = np.random.uniform(size=1)
    return(np.power(((U*8.503863413817225325e-02) -0.00029)/(-0.0848)+e_min**(-1.7),-1/1.7))

def sample_phi():
    return np.random.uniform(0, 2 * np.pi)

def generate_muon():
    theta = sample_theta()
    phi = sample_phi()
    energy = sample_energy(theta)

    dir_x = np.sin(theta) * np.cos(phi)
    dir_y = np.sin(theta) * np.sin(phi)
    dir_z = np.cos(theta)

        
    return {
        'theta_deg': np.rad2deg(theta),
        'phi_deg': np.rad2deg(phi),
        'energy_GeV': energy,
        'direction': (dir_x, dir_y, dir_z)
    }

# Generate multiple muons
def generate_muons(n=1000):
    return [generate_muon() for _ in range(n)]

print(generate_muons(n=10000))
