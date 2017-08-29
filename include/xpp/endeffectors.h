/**
 @file    endeffectors.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jan 2, 2017
 @brief   Declares the generic Endeffector class
 */

#ifndef XPP_INCLUDE_XPP_ENDEFFECTORS_H_
#define XPP_INCLUDE_XPP_ENDEFFECTORS_H_

#include <deque>
#include <iostream>
#include <map>
#include <vector>
#include <Eigen/Dense>

namespace xpp {

enum EndeffectorID { E0, E1, E2, E3, E4, E5 };

/** @brief Data structure to assign values to each endeffector.
 *
 *  Common values are xyz-positions (Vector3d), contact-flag (bool).
 */
template<typename T>
class Endeffectors {
public:
  using Container     = std::deque<T>; // to avoid faulty std::vector<bool>
  using EndeffectorsT = Endeffectors<T>;

  Endeffectors (int n_ee = 0);
  virtual ~Endeffectors ();

  /** @brief Defines the number of endeffectors.
   */
  void SetCount(int n_ee);

  /** @brief Sets each endeffector to value.
   */
  void SetAll(const T& value);

  /** @returns The number of endeffectors this structure holds.
   */
  int GetCount() const;

  /** @returns all endeffector IDs from E0->EN.
   */
  std::vector<EndeffectorID> GetEEsOrdered() const;

  T& At(EndeffectorID ee);
  const T& At(EndeffectorID ee) const;

  const EndeffectorsT operator-(const EndeffectorsT& rhs) const;
  const EndeffectorsT operator/(double scalar) const;
  bool operator!=(const Endeffectors& other) const;

  /** @returns a returns a read-only copy of the underlying STL-container.
   */
  const Container ToImpl() const;


private:
  Container ee_;
};

using EndeffectorsPos  = Endeffectors<Eigen::Vector3d>;
using EndeffectorsVel  = EndeffectorsPos;


template <typename T>
std::ostream& operator<<(std::ostream& stream, Endeffectors<T> endeffectors)
{
  for (EndeffectorID ee : endeffectors.GetEEsOrdered())
    stream << endeffectors.At(ee) << ", ";

  return stream;
}


class EndeffectorsBool : public Endeffectors<bool> {
public:
  EndeffectorsBool (int n_ee = 0) :Endeffectors(n_ee) {};
  virtual ~EndeffectorsBool () {};

  /** @brief returns a copy with flipped boolean values. */
  EndeffectorsBool Invert() const
  {
    EndeffectorsBool ret(GetCount());
    for (auto ee : GetEEsOrdered())
      ret.At(ee) = !At(ee);

    return ret;
  }

  /** @brief number of endeffectors with flag set to TRUE */
  int GetTrueCount() const
  {
    int count = 0;
    for (auto ee : GetEEsOrdered())
      if (At(ee))
        count++;

    return count;
  }
};



// some specific morphologies
namespace biped {
enum FootID { L, R };

static const std::map<EndeffectorID, FootID> kMapOptToBiped {
  { EndeffectorID::E0,  L},
  { EndeffectorID::E1,  R},
};
} // namespace biped

namespace quad {
enum FootID { RF, LF, LH, RH };

static const std::map<EndeffectorID, FootID> kMapOptToQuad {
  { EndeffectorID::E0,  LH},
  { EndeffectorID::E1,  LF},
  { EndeffectorID::E2,  RH},
  { EndeffectorID::E3,  RF},
};

enum RotorID { L, R, F, H };
static const std::map<EndeffectorID, RotorID> kMapOptToRotor {
  { EndeffectorID::E0,  L},
  { EndeffectorID::E1,  F},
  { EndeffectorID::E2,  R},
  { EndeffectorID::E3,  H},
};

} // namespace quadruped

template<typename T>
static std::map<T, EndeffectorID> Reverse(std::map<EndeffectorID, T> map) {

  std::map<T, EndeffectorID> reverse;
  for (auto pair : map) {
    auto ee = pair.first;
    reverse[map.at(ee)] = ee;
  }

  return reverse;
}


} /* namespace xpp */

#include "impl/endeffectors-impl.h"

#endif /* XPP_INCLUDE_XPP_ENDEFFECTORS_H_ */