"""
Collection of linear algebra operations and CG solver
"""
from mpi4py import MPI
import numpy as np
from . import data
from . import operators

def hpc_dot(x, y):
    """Computes the inner product of x and y"""
    # ... implement ...
    domain = x.domain
    x, y = x.inner.flatten(), y.inner.flatten()

    # res = 0
    # for i in range(len(x)):
    #     res += x[i] * y[i]
    # buf = domain.comm_cart.allreduce(res, MPI.SUM)

    # fast using numpy
    res = np.dot(x, y)
    buf = np.array(0, dtype=res.dtype)
    domain.comm_cart.Allreduce(res, buf, MPI.SUM)

    return buf

def hpc_norm2(x):
    """Computes the 2-norm of x"""
    # ... implement ...
    domain = x.domain
    x = x.inner.flatten()

    # res = 0
    # for i in range(len(x)):
    #     res += x[i] * x[i]
    # buf = domain.comm_cart.allreduce(res, MPI.SUM)

    # Fast using numpy
    res = np.dot(x, x)
    buf = np.array(0, dtype=res.dtype)
    domain.comm_cart.Allreduce(res, buf, MPI.SUM)

    return np.sqrt(buf)

class hpc_cg:
    """Conjugate gradient solver class: solve the linear system A x = b"""
    def __init__(self, domain):
        self._Ap = data.Field(domain)
        self._r  = data.Field(domain)
        self._p  = data.Field(domain)

        self._xold  = data.Field(domain)
        self._v  = data.Field(domain)
        self._Fxold  = data.Field(domain)
        self._Fx  = data.Field(domain)
        self._v  = data.Field(domain)

    def solve(self, A, b, x, tol, maxiter):
        """Solve the linear system A x = b"""
        # initialize
        A(x, self._Ap)
        self._r.inner[...] = b.inner[...] - self._Ap.inner[...]
        self._p.inner[...] = self._r.inner[...]
        delta_kp = hpc_dot(self._r, self._r)
 
        # iterate
        converged = False
        for k in range(0, maxiter):
            delta_k = delta_kp
            if delta_k < tol**2:
                converged = True
                break
            A(self._p, self._Ap)
            alpha = delta_k/hpc_dot(self._p, self._Ap)
            x.inner[...] += alpha*self._p.inner[...]
            self._r.inner[...] -= alpha*self._Ap.inner[...]
            delta_kp = hpc_dot(self._r, self._r)
            self._p.inner[...] = ( self._r.inner[...]
                                  + delta_kp/delta_k*self._p.inner[...] )

        return converged, k + 1

