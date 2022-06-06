#ifndef TOOLS_CONVERTION_H
#define TOOLS_CONVERTION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Convert a string to the corresponding unsigned int
 * \details This Function follows the CERT-INT06 rules concerning string conversions.
 * \param[in] str A string .
 * \param[out] convertedUint The pointer to the uint that should contain the converted value
 * \return True (1) if success, false if function error
 */
T_tools_boolean tools_text2unsigned(const char* str, uint32_t* convertedUint);

/**
 * \brief Convert a string to the corresponding int
 * \details This Function follows the CERT-INT06 rules concerning string conversions.
 * \param[in] str A string .
 * \param[out] convertedInt The pointer to the int that should contain the converted value
 * \return True (1) if success, false if function error
 */
T_tools_boolean tools_text2integer(const char* str, int32_t* convertedInt);


#ifdef __cplusplus
}
#endif

#endif /* TOOLS_CONVERTION_H */

