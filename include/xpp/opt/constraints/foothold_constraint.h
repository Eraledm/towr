/**
 @file    foothold_constraint.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Aug 8, 2016
 @brief   Declares an abstract FootholdConstraint class and one concrete derivation.
 */

#ifndef XPP_XPP_OPT_INCLUDE_XPP_OPT_FOOTHOLD_CONSTRAINT_H_
#define XPP_XPP_OPT_INCLUDE_XPP_OPT_FOOTHOLD_CONSTRAINT_H_

#include <xpp/opt/endeffectors_motion.h>
#include <xpp/constraint.h>

namespace xpp {
namespace opt {

/** Base class for constraints associated only with the foothold positions
  *
  * This class is responsible for updating the current position of the footholds
  * in order to then calculate costs based on these (move towards goal, avoid
  * obstacles, ...).
  */
class FootholdConstraint : public Constraint {
public:
  using EEMotionPtr   = std::shared_ptr<EndeffectorsMotion>;
  using NominalStance = EndeffectorsPos;

  FootholdConstraint (const EEMotionPtr&, const NominalStance&, double t);
  virtual ~FootholdConstraint ();

private:
  virtual void UpdateConstraintValues () override;
  virtual void UpdateBounds() override;

  EEMotionPtr ee_motion_;
  NominalStance desired_ee_pos_W_;
  double t_;
};

} /* namespace opt */
} /* namespace xpp */

#endif /* XPP_XPP_OPT_INCLUDE_XPP_OPT_FOOTHOLD_CONSTRAINT_H_ */