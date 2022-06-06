/**
 * \file
 * \brief:  CRYPTO_AL sub-module API: only start/stop fir initialization of library
 * \details
 *
 * \date 04/2020  Creation
 *
 */

#ifndef CRYPTOAL_MGT_H
#define CRYPTOAL_MGT_H

/*------------------------------ Definitions -------------------------------*/


/*------------------------------ externs -----------------------------------*/



/*------------------------------ Functions -----------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------- API for the sub-module launcher  --------------- */

/* \brief Start of sub-module */
T_com_status cryptoal_initialize
(
    void
);

/* \brief Stop of sub-module */
T_com_status cryptoal_reset
(
    void
);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTOAL_MGT_H */
