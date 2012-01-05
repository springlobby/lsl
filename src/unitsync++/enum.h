#ifndef LSL_USYNC_ENUMS_H
#define LSL_USYNC_ENUMS_H

namespace LSL {
namespace Enum {

//! enum that lets us differentiate option types at runtime
/*! opt_undefined will be returned/set if the type could not be determined, others respectively */
enum OptionType {
	opt_undefined  = 0,
	opt_bool       = 1,
	opt_list       = 2,
	opt_float      = 3,
	opt_string     = 4,
	opt_section    = 5
};

enum OptionControlType{
	oct_undefined,
	oct_someothers
};


} //namespace Enum {
} //namespace LSL {


#endif // LSL_USYNC_ENUMS_H
