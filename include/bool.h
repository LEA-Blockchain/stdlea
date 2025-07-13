#ifndef STDBOOL_H
#define STDBOOL_H
#include "feature_check.h"

#ifndef __bool_true_false_are_defined

/**
 * @brief Defines 'bool' as a more convenient alias for the built-in '_Bool' keyword.
 */
#define bool _Bool

/**
 * @brief Defines the boolean 'true' constant, which expands to the integer 1.
 */
#define true 1

/**
 * @brief Defines the boolean 'false' constant, which expands to the integer 0.
 */
#define false 0

/**
 * @brief A guard macro to signal that bool, true, and false have been defined.
 *
 * This allows other code to check if boolean types are available and prevents
 * redefinition errors if the standard <stdbool.h> is also included.
 */
#define __bool_true_false_are_defined 1

#endif /* __bool_true_false_are_defined */

#endif // STDBOOL_H
