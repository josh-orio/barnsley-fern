#include <Python.h>
#include <iostream>
#include <random>

// RANDOM NUMBER GENERATOR
std::random_device rd;
std::default_random_engine re = std::default_random_engine(rd());
std::uniform_real_distribution<double> urd =
    std::uniform_real_distribution<double>(0, 1);
float rng() { return urd(re); }
////

int main() {
  int iterations = 1e4;

  float x = 0, y = 0, xn, yn, r;

  float *xs = new float[iterations];
  float *ys = new float[iterations];

  // function tables
  float *f1 = new float[]{0, 0, 0, 0.16, 0, 0, 0.01};
  float *f2 = new float[]{0.85, 0.04, -0.04, 0.85, 0, 1.60, 0.85};
  float *f3 = new float[]{0.20, -0.26, 0.23, 0.22, 0, 1.60, 0.07};
  float *f4 = new float[]{-0.15, 0.28, 0.26, 0.24, 0, 0.44, 0.07};

  int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, p = 6; // subscripts, see 'Function Table' in README.md

  for (int i = 0; i < iterations; i++) {
    r = rng();

    if (r < f1[p]) {
        xn = (f1[a] * x) + (f1[b] * y) + f1[e];
        yn = (f1[c] * x) + (f1[d] * y) + f1[f];
    }
    else if (r < f1[p] + f2[p]) {
        xn = (f2[a] * x) + (f2[b] * y) + f2[e];
        yn = (f2[c] * x) + (f2[d] * y) + f2[f];
    }
    else if (r < f1[p] + f2[p] + f3[p]) {
        xn = (f3[a] * x) + (f3[b] * y) + f3[e];
        yn = (f3[c] * x) + (f3[d] * y) + f3[f];
    }
    else { // f4[p]
        xn = (f4[a] * x) + (f4[b] * y) + f4[e];
        yn = (f4[c] * x) + (f4[d] * y) + f4[f];
    }

    xs[i] = xn;
    ys[i] = yn;
    x = xn;
    y = yn;
  }

  Py_Initialize();
  PyObject *py_x = PyList_New(iterations); // create python list same size as xs

  // iteratively transfer xs to py_x
  for (size_t i = 0; i < iterations; i++) {
    PyObject *py_float = PyFloat_FromDouble(static_cast<double>(xs[i]));
    PyList_SetItem(py_x, i, py_float); // py_x[i] = py_float
  }

  // assignment of py list to variable in main module
  PyObject *main_module = PyImport_AddModule("__main__");
  PyObject *main_dict = PyModule_GetDict(main_module);
  PyDict_SetItemString(main_dict, "x", py_x);

  // same process for ys
  PyObject *py_y = PyList_New(iterations);

  for (size_t i = 0; i < iterations; i++) {
    PyObject *py_float = PyFloat_FromDouble(static_cast<double>(ys[i]));
    PyList_SetItem(py_y, i, py_float); // py_y[i] = py_float
  }

  PyDict_SetItemString(main_dict, "y", py_y);

  // check a list is transferred properly
  // PyRun_SimpleString("print(y)");

  // plt script
  PyRun_SimpleString("import matplotlib.pyplot as plt\n"
                     "plt.scatter(x, y, s=0.25)\n"
                     "plt.savefig('../fern.png')\n" // store image in repo dir
  );

  Py_Finalize(); // close python interpreter

  return 0;
}