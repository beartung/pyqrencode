#include <Python.h>
#include <stdlib.h>
#include <qrencode.h>

static PyObject *encode(PyObject *self, PyObject *args)
{
    char *str;
    int i, version, level, hint, case_sensitive, num_pixels;
    QRcode *code;
    PyObject *ret;
    
    if(!PyArg_ParseTuple(args, "siiii", &str, &version, &level, &hint,
                         &case_sensitive))
        return NULL;
    
    code = QRcode_encodeString(str, version, level, hint, case_sensitive);
    
    num_pixels = code->width * code->width;
    for(i = 0; i < num_pixels; i++)
      code->data[i] = 255 - (code->data[i] & 0x1) * 0xFF;
    
    ret = Py_BuildValue("(iis#)", code->version, code->width,
                        code->data, num_pixels);
    QRcode_free(code);
    return ret;
};

static PyMethodDef qrencode_methods[] =
{
    {"encode", encode, METH_VARARGS, "Encodes a string as a QR-code. Returns a tuple of (version, width, data)"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
init_qrencode(void)
{
    PyObject *m = Py_InitModule("qrencode._qrencode", qrencode_methods);
    if(m == NULL)
      return;
}
