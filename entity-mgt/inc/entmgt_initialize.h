#ifndef ENTMGT_INITIALIZE_H
#define ENTMGT_INITIALIZE_H

/*------------------------------ Include files -------------------------------*/
/*------------------------------ Pragma --------------------------------------*/
/*------------------------------ Macros --------------------------------------*/
/*------------------------------ Structures ----------------------------------*/
/*------------------------------ Constants -----------------------------------*/
/*------------------------------ Types ---------------------------------------*/
/*------------------------------ Functions -----------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialization function
 * @detail This function initialize the entity management first, then all entity registered
 * @param launcherList
 * @param launcherListSize
 * @return 
 */
T_com_status entmgt_initialize(T_com_entity_launch * launcherList[], size_t * launcherListSize/*, char ** rootPath, char ** scriptPath*/);

#ifdef __cplusplus
}
#endif

#endif /* ENTMGT_INITIALIZE_H */

