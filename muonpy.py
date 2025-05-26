import numpy as np

K = 0.307075 # MeV mol^-1 cm^2  Constante de masa y radio de electrion 4 pi Na re^2 mec^2
me = 0.51099895000 # MeV/c2     Masa del electrón.
z = -1 #                        Número de carga de la partícula incidente.
m_mu = 105.6583755 # MeV/c2        Masa de un muón.

# Clase para definir los materiales:
class Material:
    def __init__ (self, name, ZA, rho, I, a, m0, x0, x1, C, del0, conductor, L, k, E0):
        self.name = name # Nombre del material.
        # Constantes de la ecuación de Bethe-Bloch definidas por material:
        self.ZA = ZA # Z/A [mol/g]
        self.rho = rho # rho [g/cm3]
        self.I = I # I [eV]
        self.a = a # a
        self.m = m0# m
        self.x0 = x0 # x0
        self.x1 = x1 # x1
        self.C = C # -C
        self.del0 = del0
        self.conductor = conductor #booleano, True si el material es conductor, False si el material no es conductor
        

        # Constantes de la función de radiación definidas por material
        self.L = L # 1e-6 cm^2/g
        self.k = k
        self.E0 = E0

    # Función del efecto de la densidad del material en la ionización:
    def dens(self, beta, gamma):
        x = np.log10(beta*gamma)
        f = 0.
        if x >= self.x1:
            f = 2*np.log(10)*x - self.C
        elif (self.x0 <= x )*(x < self.x1):
            f = 2*np.log(10)*x - self.C + self.a*(self.x1-x)**self.m
        elif (x<self.x0)*self.conductor:
            f = self.del0*10**(2*(x-self.x0))
        return(f)

    # A continuación se asumirá que la energía ingresada a las funciones se encuentra en MeV
    # Función de Bethe-Bloch:
    def Bethe_Bloch(self, E):
        gamma = E/m_mu
        gamma2 = gamma**2
        beta2 = 1 + -1/gamma2
        beta = np.sqrt(beta2)
        Wmax = (2*me*beta2*gamma2)/(1+ 2*gamma*me/m_mu +(me/m_mu)**2) # MeV
        A = (K*(z**2)*self.ZA/beta2) # MeV cm^2 / g
        squares = 0.5*np.log((2*me*beta2*gamma2*Wmax/self.I**2)*1e12) - beta2 - 0.5*self.dens(beta,gamma)
        return(-A* squares) # MeV cm^2 / g

    def b(self,E):
        x = np.log10(E/1000) #      Se divide por 1000 para convertirlos en GeV
        return(-self.L/(1+np.exp(-self.k*(x-self.E0)))*1e-6) # cm^2/g

    def dE(self,E):
        return(self.Bethe_Bloch(E) + self.b(E)*E) # MeV cm^2 / g

# se definen los materiales:
# Rock = Material("Piedra estándar",0.5000, 2.650, 136.40, 0.08301, 3.4120, 0.0492, 3.0549, 3.7738,0.00, False, 4.6435, 1.069, 1.4100)

Aire = Material("Aire (1 atm)", 0.49919, 1.205e-03, 85.7, 0.10914, 3.3994, 1.7418, 4.2759, 10.5961, 0.00, False, 3.5395151, 0.97224287 ,1.41290616)

Fe = Material("Fe",26/55.845, 7.874, 286.0, 0.14680, 2.99632, -0.0012, 3.1531, 4.2911, 0.12, True, 8.4133, 1.2199, 1.4015)
# H2 = Material("H (gas)",1/1.008, 8.376e-05, 19.2, 0.14092, 5.7273, 1.8639, 3.2718, 9.5834, 0.00, False, 2.5592, 0.6685, 1.8149)
# U = Material("U", 92/238.02891, 18.950, 890.0, 0.19677, 2.8171, 0.2260, 3.3721, 5.8694, 0.14, True, 19.91048889, 1.42154277, 1.44873242)
Pb = Material("Pb", 82/207.2, 11.350, 823.0, 0.09359, 3.1608, 0.3776, 3.8073, 6.2018, 0.14, True, 18.41851429, 1.38841605, 1.42736329)

# PVT = Material("Poliviniltolueno", 0.54141, 1.032, 64.7, 0.16101, 3.2393, 0.1464, 2.4855, 3.1997, 0.00, False, 3.07573289, 0.90073632, 1.43252999)


def Construct(x,y,z): #                   distancia en metros.
    mat_ = Aire
    if (((x >= -5) * (x<=5)) )*((y >= -5) * (y<=5))*((z >= 0) * (z<=10)):
        if ((x >= -3) * (x<=3))*((y >= -3) * (y<=3)):
            mat_ = Aire
        else:
            mat_ = Pb
    return mat_

# Se lleva a cabo la integración sobre la distancia recorrida por el muón:
d = 10 #m                       distancia máxima a calcular.
pas = d*100 #                 número de pasos para integrar.
h = d/pas #                     distancia recorrida en cada paso.

muones = np.loadtxt("energies")
muones[:,0] = muones[:,0]*1000 # Para convertir la energía a MeV

muonesFin = np.zeros((len(muones[:,0]),3))

def paso(E0,x,y):
    Es = [E0]
    dEs = []
    Z = [0.]
    for i in range(pas -1):
        Z.append(h + Z[i])
        Mat = Construct(x,y,Z[i])
        dE = Mat.dE(Es[i])*Mat.rho*100# MeV/m
        B =  Es[i]+ h*dE
        # print(B)
        if B <= m_mu:
            Es.append(0.0)
            break
        Es.append(B)
        dEs.append(dE)
    return Es[-1]
    # fEnergies[u,w] = Es[-1]

for i in range(len(muones[:,0])):
    muonesFin[i,0] = paso(muones[i,0],muones[i,1],muones[i,2])
    muonesFin[i,1] = muones[i,1]
    muonesFin[i,2] = muones[i,2]

np.savetxt("eFinal.txt", muonesFin)
