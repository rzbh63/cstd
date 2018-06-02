/*
 * Copyright (c) 2009 Samit Basu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "Print.h"
#include "IEEEFP.h"
#include "FunctionDef.h"
#include "interpreter_t.h"
#include "Algorithms.h"
#include "Struct.h"
#include <cmath>

#define MSGBUFLEN 2048

static FMFormatMode formatMode = format_short;

void SetPrintFormatMode(FMFormatMode m)
{
  formatMode = m;
}

FMFormatMode GetPrintFormatMode()
{
  return formatMode;
}

class ArrayFormatInfo
{
public:
  int width;
  BOOL floatasint;
  int decimals;
  BOOL expformat;
  double scalefact;
  ArrayFormatInfo(int w = 0, BOOL f = FALSE,
      int d = 0, BOOL e = FALSE,
      double s = 1.0) :
    width(w), floatasint(f),
    decimals(d), expformat(e),
    scalefact(s) {}
};

template <typename T>
static QString TSummarizeArrayCellEntryScalar(const mxArray* dp)
{
  if (dp mxIsAllReal()) {
    return QString("%1").arg(dp.constRealScalar<T>());
  }
  else if (dp.constImagScalar<T>() > 0) {
    return QString("%1+%2i").arg(dp.constRealScalar<T>()).arg(dp.constImagScalar<T>());
  }
  else {
    return QString("%1%2i").arg(dp.constRealScalar<T>()).arg(dp.constImagScalar<T>());
  }
}

#define MacroSummarize(ctype,cls) \
  case cls: return TSummarizeArrayCellEntryScalar<ctype>(a);

QString SummarizeArrayCellEntry(const mxArray* a)
{
  // Special cases
  if (a.isEmpty()) {
    return QString("");
  }

  if (a.isString() && a.rows() == 1) {
    return a.asString();
  }

  if (a mxIsScalar() && !a.isReferenceType()) {
    switch (a->classID) {
      MacroExpandCases(MacroSummarize);

    default:   // fall through
      ;
    }
  }

  QString sparseFlag;

  if (a->issparse) {
    sparseFlag = " sparse";
  }

  return a->dims.toString() + " " +
      a.className() + sparseFlag + " array";
}

template <class T>
static int GetNominalWidthInteger(const mxArray* qp)
{
  char buffer[MSGBUFLEN];
  int count = int(qp.length());
  const T* array = qp.constData();
  int maxdigit = 0;

  for (i = 0; i < count; i++) {
    memset(buffer, 0, MSGBUFLEN);
    sprintf(buffer, "%lld", (int64)array[i]);
    int j = maxdigit;

    while (buffer[j] && j >= maxdigit) {
      j++;
    }

    maxdigit = j;
  }

  return maxdigit;
}

template <class T>
static void ComputeScaleFactor(const mxArray* qp, ArrayFormatInfo& format,
    BOOL complex = FALSE)
{
  int count = int(qp.length());
  const T* array = qp.constData();
  T max_amplitude = 0;

  if (count == 0) {
    return;
  }

  if (format.expformat) {
    return;
  }

  BOOL finiteElementFound = FALSE;

  for (i = 0; i < count; i++) {
    if (IsFinite(array[i]) && !finiteElementFound) {
      max_amplitude = array[i];
      finiteElementFound = TRUE;
    }

    if ((IsFinite(array[i])) &&
        (fabs((double) array[i]) > fabs((double) max_amplitude))) {
      max_amplitude = array[i];
    }
  }

  if (!finiteElementFound) {
    return;
  }

  T maxval = complex ? 100 : 1000;

  if (max_amplitude >= maxval) {
    format.scalefact = pow(double(10.0), double(floor(log10(max_amplitude))));
  }
  else if (max_amplitude <= -maxval) {
    format.scalefact = pow(double(10.0), double(floor(log10(-max_amplitude))));
  }
  else if ((max_amplitude <= .1) && (max_amplitude > 0)) {
    format.scalefact = pow(double(10.0), double(floor(log10(max_amplitude))));
  }
  else if ((max_amplitude >= -.1) && (max_amplitude < 0)) {
    format.scalefact = pow(double(10.0), double(floor(log10(-max_amplitude))));
  }
  else {
    format.scalefact = 1.0;
  }
}


template <typename T>
static ArrayFormatInfo GetArrayFormatReal(T x = 0);

template <typename T>
static ArrayFormatInfo GetArrayFormatComplex(T x = 0);

//Visually Tuned
template <>
ArrayFormatInfo GetArrayFormatReal(float)
{
  if (formatMode == format_short) {
    return ArrayFormatInfo(9, FALSE, 4);
  }
  else if (formatMode == format_long) {
    return ArrayFormatInfo(11, FALSE, 7);
  }
  else if (formatMode == format_short_e) {
    return ArrayFormatInfo(11, FALSE, 4, TRUE);
  }
  else if (formatMode == format_long_e) {
    return ArrayFormatInfo(14, FALSE, 7, TRUE);
  }

  throw_Exception("Unhandled format type");
}

//Visually Tuned
template <>
ArrayFormatInfo GetArrayFormatReal(double)
{
  if (formatMode == format_short) {
    return ArrayFormatInfo(9, FALSE, 4);
  }
  else if (formatMode == format_long) {
    return ArrayFormatInfo(18, FALSE, 14);
  }
  else if (formatMode == format_short_e) {
    return ArrayFormatInfo(11, FALSE, 4, TRUE);
  }
  else if (formatMode == format_long_e) {
    return ArrayFormatInfo(21, FALSE, 14, TRUE);
  }

  throw_Exception("Unhandled format type");
}

//Visually Tuned
template <>
ArrayFormatInfo GetArrayFormatComplex(float)
{
  if (formatMode == format_short) {
    return ArrayFormatInfo(20, FALSE, 4);
  }
  else if (formatMode == format_long) {
    return ArrayFormatInfo(24, FALSE, 7);
  }
  else if (formatMode == format_short_e) {
    return ArrayFormatInfo(26, FALSE, 4, TRUE);
  }
  else if (formatMode == format_long_e) {
    return ArrayFormatInfo(32, FALSE, 7, TRUE);
  }

  throw_Exception("Unhandled format type");
}

//Visually Tuned
template <>
ArrayFormatInfo GetArrayFormatComplex(double)
{
  if (formatMode == format_short) {
    return ArrayFormatInfo(20, FALSE, 4);
  }
  else if (formatMode == format_long) {
    return ArrayFormatInfo(40, FALSE, 14);
  }
  else if (formatMode == format_short_e) {
    return ArrayFormatInfo(26, FALSE, 4, TRUE);
  }
  else if (formatMode == format_long_e) {
    return ArrayFormatInfo(46, FALSE, 14, TRUE);
  }

  throw_Exception("Unhandled format type");
}

template <typename T>
static ArrayFormatInfo ComputeArrayFormatInfo(const mxArray* rp)
{
  if (rp mxIsAllReal()) {
    if (IsInteger(rp.constReal<T>())) {
      return ArrayFormatInfo(GetNominalWidthInteger(rp.constReal<T>()));
    }

    ArrayFormatInfo ret = GetArrayFormatReal(T(0));
    ComputeScaleFactor(rp.constReal<T>(), ret);
    return ret;
  }

  BasicArray<T> combined(rp.fortran<T>());
  ArrayFormatInfo ret = GetArrayFormatComplex(T(0));
  ComputeScaleFactor(combined, ret, TRUE);
  return ret;
}

#define MacroComputeArrayFormat(ctype,cls)  \
  case cls: return ComputeArrayFormatInfo<ctype>(ref);

static ArrayFormatInfo ComputeArrayFormatInfo(const mxArray* ref)
{
  if (ref->issparse) {
    return ArrayFormatInfo(1);
  }

  switch (ref->classID) {
  default:
    throw_Exception("unexpected class for ComputeArrayFormatInfo");

  case Float:
    return ComputeArrayFormatInfo<float>(ref);

  case Double:
    return ComputeArrayFormatInfo<double>(ref);

  case Bool:
    return ArrayFormatInfo(1);

  case StringArray:
    return ArrayFormatInfo(0);

  case CellArray: {
    int maxwidth = 1;

    for (index_t i = 1; i <= ref.length(); i++) {
      int len = SummarizeArrayCellEntry(ref.get(i).constRealScalar<mxArray>()).size();
      maxwidth = qMax(maxwidth, len);
    }

    return ArrayFormatInfo(maxwidth);
  }
  }
}

#undef MacroComputeArrayFormat

template <class T>
static inline void emitIntegerReal(interpreter_t* io, T val,
    const ArrayFormatInfo& format,
    BOOL sgned)
{
  if (sgned) {
    io->outputMessage("%*lld", format.width, (int64)val);
  }
  else {
    io->outputMessage("%*llu", format.width, (uint64)val);
  }
}

template <class T>
static inline void emitFloatReal(interpreter_t* io, T val, const ArrayFormatInfo& format)
{
  if (IsNaN(val)) {
    io->outputMessage("NaN");
    return;
  }

  if (!IsFinite(val)) {
    if (val > 0) {
      io->outputMessage("Inf");
    }
    else {
      io->outputMessage("-Inf");
    }

    return;
  }

  if (val != 0)
    if (format.expformat) {
      io->outputMessage("%*.*e", format.width, format.decimals, val);
    }
    else {
      io->outputMessage("%*.*f", format.width, format.decimals, val / format.scalefact);
    }
  else {
    io->outputMessage("%*d", format.width, 0);
  }
}

template <class T>
static inline void emitFloatComplex(interpreter_t* io, T real, T imag,
    const ArrayFormatInfo& format)
{
  int width = format.width / 2 - 2;

  if ((real != 0) || (imag != 0)) {
    if (IsNaN(real)) {
      io->outputMessage("NaN");
    }
    else if (!IsFinite(real)) {
      if (real > 0) {
        io->outputMessage("Inf");
      }
      else {
        io->outputMessage("-Inf");
      }
    }
    else if (format.expformat) {
      io->outputMessage("%*.*e", width, format.decimals, real);
    }
    else {
      io->outputMessage("%*.*f", width, format.decimals, real / format.scalefact);
    }

    if (IsNaN(imag)) {
      io->outputMessage(" + NaNi");
    }
    else if (!IsFinite(imag)) {
      if (imag > 0) {
        io->outputMessage(" + Infi");
      }
      else {
        io->outputMessage(" - Infi");
      }
    }
    else if (imag < 0) {
      if (format.expformat) {
        io->outputMessage(" -%*.*ei", width, format.decimals, -imag);
      }
      else {
        io->outputMessage(" -%*.*fi", width, format.decimals, -imag / format.scalefact);
      }
    }
    else {
      if (format.expformat) {
        io->outputMessage(" +%*.*ei", width, format.decimals, imag);
      }
      else {
        io->outputMessage(" +%*.*fi", width, format.decimals, imag / format.scalefact);
      }
    }
  }
  else {
    io->outputMessage("%*d%*c", width, 0, width + 2, ' ');
  }
}

template <typename T>
static inline void EmitInteger(interpreter_t* io, const mxArray* rp,
    const ArrayFormatInfo& format,
    BOOL complex, BOOL sgned)
{
  emitIntegerReal(io, rp.constRealScalar<T>(), format, sgned);
}

template <typename T>
static inline void EmitFloat(interpreter_t* io, const mxArray* rp,
    const ArrayFormatInfo& format,
    BOOL complex)
{
  if (format.floatasint) {
    return EmitInteger<T>(io, rp, format, complex, TRUE);
  }

  if (complex) {
    emitFloatComplex(io, rp.constRealScalar<T>(), rp.constImagScalar<T>(), format);
  }
  else {
    emitFloatReal(io, rp.constRealScalar<T>(), format);
  }
}

#define MacroEmitSignedInt(ctype,cls) \
  case cls: return EmitInteger<ctype>(io,rp,format,complex,TRUE);

#define MacroEmitUnsignedInt(ctype,cls) \
  case cls: return EmitInteger<ctype>(io,rp,format,complex,FALSE);

#define MacroEmitFloat(ctype,cls) \
  case cls: return EmitFloat<ctype>(io,rp,format,complex);

static void Emit(interpreter_t* io, const mxArray* rp,
    const ArrayFormatInfo& format, BOOL complex)
{
  switch (rp->classID) {
  default:
    throw_Exception("unexpected class for Emit");
    MacroExpandCasesIntSigned(MacroEmitSignedInt);
    MacroExpandCasesIntUnsigned(MacroEmitUnsignedInt);
    MacroEmitFloat(float, Float);
    MacroEmitFloat(double, Double);

  case StringArray:
    io->outputMessage(QString(rp.constReal<QChar>().get(1)));
    return;

  case CellArray:
    io->outputMessage("[" +
        SummarizeArrayCellEntry(rp.constRealScalar<mxArray>()) +
        "]");
    return;
  }
}

#undef MacroEmitFloat
#undef MacroEmitUnsignedInt
#undef MacroEmitSignedInt


static inline void PrintSheet(interpreter_t* io, const ArrayFormatInfo& format,
    index_t offset, const mxArray* rp, int termWidth,
    int& printlimit, BOOL complex)
{
  if (printlimit == 0) {
    return;
  }

  // Determine how many columns will fit across
  // the terminal width
  int columns = int(rp.cols());
  int rows = int(rp.rows());

  if (rp->classID == StringArray) {
    for (i = 0; i < rows; i++) {
      for (j = 0; j < columns; j++) {
        Emit(io, rp.get(i + j * rows + offset), format, complex);
        printlimit--;

        if (printlimit <= 0) {
          return;
        }
      }

      io->outputMessage("\n");
    }

    return;
  }

  int colsPerPage = (int) floor((termWidth - 1) / ((double) format.width + 2));
  colsPerPage = (colsPerPage < 1) ? 1 : colsPerPage;
  int pageCount = (int) ceil(columns / ((double)colsPerPage));

  for (k = 0; k < pageCount; k++) {
    int colsInThisPage;
    colsInThisPage = columns - colsPerPage * k;
    colsInThisPage = (colsInThisPage > colsPerPage) ?
        colsPerPage : colsInThisPage;

    if ((rows * columns > 1) && (pageCount > 1))
      io->outputMessage("\n Columns %d to %d\n\n",
          k * colsPerPage + 1, k * colsPerPage + colsInThisPage);

    //    else
    //      io->outputMessage("\n");
    for (i = 0; i < rows; i++) {
      if (rp->classID != StringArray) {
        io->outputMessage(" ");
      }

      for (j = 0; j < colsInThisPage; j++) {
        Emit(io, rp.get(i + (k * colsPerPage + j)*rows + offset), format, complex);
        printlimit--;

        if (printlimit <= 0) {
          return;
        }

        io->outputMessage(" ");
      }

      io->outputMessage("\n");
    }
  }

  io->outputMessage("\n");
}


template <class T>
static void PrintSparse(const SparseMatrix<T>& A, interpreter_t* io, const ArrayFormatInfo& format)
{
  ConstSparseIterator<T> i(&A);

  if (!i.isValid()) {
    io->outputMessage("  <empty>\n");
  }

  while (i.isValid()) {
    io->outputMessage(QString(" %1 %2 %3\n").arg(i.row()).arg(i.col()).arg(i.value()));
    i.next();
  }
}

template <class T>
static void PrintSparse(const SparseMatrix<T>& Areal, const SparseMatrix<T>& Aimag, interpreter_t* io, const ArrayFormatInfo& format)
{
  ConstComplexSparseIterator<T> i(&Areal, &Aimag);

  if (!i.isValid()) {
    io->outputMessage("  <empty>\n");
  }

  while (i.isValid()) {
    if (i.imagValue() >= 0)
      io->outputMessage(QString(" %1 %2 %3+%4i").arg(i.row()).arg(i.col()).
          arg(i.realValue()).arg(i.imagValue()));
    else
      io->outputMessage(QString(" %1 %2 %3%4i").arg(i.row()).arg(i.col()).
          arg(i.realValue()).arg(i.imagValue()));

    io->outputMessage("\n");
    i.next();
  }
}

template <class T>
static void PrintSparseMatrix(const mxArray* A, interpreter_t* io, const ArrayFormatInfo& format)
{
  if (A mxIsAllReal()) {
    PrintSparse(A.constRealSparse<T>(), io, format);
  }
  else {
    PrintSparse(A.constRealSparse<T>(), A.constImagSparse<T>(), io, format);
  }
}

static void PrintStructArray(const mxArray* A, interpreter_t* io)
{
  const StructArray& rp(A.constStructPtr());

  if (A mxIsScalar()) {
    for (i = 0; i < rp.fieldCount(); i++) {
      io->outputMessage("    ");
      io->outputMessage(rp.fieldName(i));
      io->outputMessage(": ");
      io->outputMessage(SummarizeArrayCellEntry(rp[i].get(1)));
      io->outputMessage("\n");
    }
  }
  else {
    io->outputMessage(A->dims.toString() + " struct array with fields:\n");

    for (i = 0; i < rp.fieldCount(); i++) {
      io->outputMessage("    ");
      io->outputMessage(rp.fieldName(i));
      io->outputMessage("\n");
    }
  }
}

#define MacroSparse(ctype,cls) \
  case cls: return PrintSparseMatrix<ctype>(A,io,format);

// width=3, fai = FALSE, decimals=0 expformat = FALSE, scalefact = 1

void PrintArrayClassic(mxArray A, int printlimit, interpreter_t* io)
{
  if (printlimit == 0) {
    return;
  }

  int termWidth = io->getTerminalWidth();
  NTuple Adims(A->dims);

  if (A.mxIsUserClass())  {
    if (!A mxIsScalar()) {
      io->outputMessage("  " + A->dims.toString() + " array of");
    }

    io->outputMessage("  " + A.className() + "\n");
    return;
  }

  if (A.isEmpty()) {
    if (A->dims == NTuple(0, 0)) {
      io->outputMessage("  []\n");
    }
    else {
      io->outputMessage("  Empty array ");
      io->outputMessage(A->dims.toString());
      io->outputMessage("\n");
    }

    return;
  }

  if (A mxIsScalar()) {
    A = A.asDenseArray();
  }

  if (A->classID == Struct) {
    return PrintStructArray(A, io);
  }

  if ((A->classID != CellArray) && (A->classID != StringArray) && (A->classID != Float)) {
    A = A.toClass(Double);
  }

  ArrayFormatInfo format(ComputeArrayFormatInfo(A));

  if (!A mxIsScalar() && (format.scalefact != 1)) {
    io->outputMessage("\n   %.1e * \n", format.scalefact);
  }

  if (A mxIsScalar() && !format.floatasint) {
    if (fabs(log10(format.scalefact)) > 3) {
      format.expformat = TRUE;
    }
    else {
      format.scalefact = 1;
    }
  }

  if (A->issparse) {
    switch (A->classID) {
    default:
      throw_Exception("Unknown sparse array");

    case Float:
      return PrintSparseMatrix<float>(A, io, format);

    case Double:
      return PrintSparseMatrix<double>(A, io, format);
    }

    return;
  }

  BOOL complexFlag = !A mxIsAllReal();

  if (A.is2D()) {
    PrintSheet(io, format, index_t(1), A, termWidth, printlimit, complexFlag);
  }
  else {
    /**
     * For N-ary arrays, data slice  -  start with
     * [1,1,1,...,1].  We keep doing the matrix
     * print , incrementing from the highest dimension,
     * and rolling downwards.
     */
    io->outputMessage("\n");
    index_t offset = 1;
    NTuple wdims(1, 1);
    index_t rows = A.rows();
    index_t columns = A.cols();

    while (wdims <= A->dims) {
      io->outputMessage("(:,:");

      for (m = 2; m < A->dims.lastNotOne(); m++) {
        io->outputMessage(QString(",%1").arg(wdims[m]));
      }

      io->outputMessage(") = \n");
      PrintSheet(io, format, offset, A, termWidth, printlimit, complexFlag);
      offset += rows * columns;
      wdims[2]++;
      A->dims.ripple(wdims);
    }
  }

  if (printlimit == 0) {
    io->outputMessage("\nPrint limit has been reached.  Use setprintlimit function to enable longer printouts\n");
  }
}

#undef MacroSparse

QString ArrayToPrintableString(const mxArray* a)
{
  if (a.isEmpty()) {
    return QString("[]");
  }

  if (a->issparse) {
    return QString("");
  }

  if (a.isString()) {
    return a.asString();
  }

  if (a.isReferenceType()) {
    return QString("");
  }

  if (!a mxIsScalar() && !a.isString()) {
    return QString("");
  }

  switch (a->classID) {
  default:
    return QString("?");
    MacroExpandCases(MacroSummarize);
  }
}

#undef MacroSummarize

